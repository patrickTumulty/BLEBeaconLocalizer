
#ifndef UDP_SERVER_HPP
#define UDP_SERVER_HPP

#include <asio.hpp>

class AnchorServer
{
public:
    AnchorServer(int port, asio::io_context *ioContext);
    ~AnchorServer();

private:
    int _port;
    asio::io_context *_ioContext;
};

#endif // UDP_SERVER_HPP
