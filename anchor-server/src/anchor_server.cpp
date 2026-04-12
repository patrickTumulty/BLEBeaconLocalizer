
#include "anchor_server.hpp"
#include <cstdint>
#include <iostream>

extern "C" {
#include "message_types.h"
}

using namespace std::chrono_literals;
using asio::ip::udp;

AnchorServer::AnchorServer(int port, asio::io_context *ioContext)
    : _port(port),
      _ioContext(ioContext),
      _socket(*_ioContext, udp::endpoint(udp::v4(), _port)),
      _timer(*_ioContext),
      _bcastEndpoint(asio::ip::make_address("10.10.0.255"), port)
{
    asio::socket_base::broadcast option(true);
    _socket.set_option(option);

    asyncReceive();

    scheduleTimer();
}

AnchorServer::~AnchorServer()
{
}

void AnchorServer::asyncReceive()
{
    udp::endpoint sender;
    _socket.async_receive_from(asio::buffer(_receiveBuffer), sender,
                               [&](std::error_code ec, std::size_t bytes_recvd) {
                                   if (ec)
                                   {
                                       // Error
                                       return;
                                   }

                                   std::cout << "Sender: " << sender.address()
                                             << "\n";

                                   asyncReceive();
                               });
}


void AnchorServer::scheduleTimer()
{
    _timer.expires_after(1s);
    _timer.async_wait([&](const std::error_code &ec) {
        if (ec)
        {
            // Error
            return;
        }

        timerCallback();

        scheduleTimer();
    });
}


void AnchorServer::timerCallback()
{
    AnchorPacket packet{0};

    packet.header.magic = 0xAB;
    packet.header.pkt_id = CMD_ID_PING;
    packet.header.anchor_id = 0;
    packet.header.pkt_size = sizeof(packet.header);
    packet.header.pkt_id = _packetCounter++;

    std::cout << "Sending ping\n";

    _socket.async_send_to(asio::buffer(&packet, packet.header.pkt_size),
                          _bcastEndpoint,
                          [this](std::error_code ec, std::size_t bytes_sent) {
                              if (!ec)
                              {
                                  std::cout << "UDP sent " << bytes_sent << " bytes\n";
                              }
                              else
                              {
                                  std::cerr << "Send error: " << ec.message() << "\n";
                              }
                          });
}
