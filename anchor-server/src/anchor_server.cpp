
#include "anchor_server.hpp"

AnchorServer::AnchorServer(int port, asio::io_context *ioContext) : _port(port), _ioContext(ioContext)
{
        //   asio::io_context io_context;
        //
        // udp::socket socket(io_context, udp::endpoint(udp::v4(), 9000));
        //
        // std::cout << "Listening on port 9000...\n";
        //
        // char data[1024];
        // udp::endpoint sender_endpoint;
        //
        // while (true) {
        //     size_t length = socket.receive_from(
        //         asio::buffer(data), sender_endpoint);
        //
        //     std::cout << "Received: "
        //               << std::string(data, length)
        //               << " from "
        //               << sender_endpoint.address().to_string()
        //               << ":" << sender_endpoint.port()
        //               << "\n";
        // }

}

AnchorServer::~AnchorServer()
{
}
