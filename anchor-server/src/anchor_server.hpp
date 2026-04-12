
#ifndef UDP_SERVER_HPP
#define UDP_SERVER_HPP

#include <asio.hpp>
#include <cstdint>

using asio::ip::udp;

class AnchorServer
{
public:
    AnchorServer(int port, asio::io_context *ioContext);
    ~AnchorServer();

private:
    void asyncReceive();
    void scheduleTimer();
    void timerCallback();

    int _port;
    asio::io_context *_ioContext;
    udp::socket _socket;
    uint8_t _receiveBuffer[1024]{0};
    asio::steady_timer _timer;
    uint32_t _packetCounter = 0;
    udp::endpoint _bcastEndpoint;

};

#endif // UDP_SERVER_HPP
