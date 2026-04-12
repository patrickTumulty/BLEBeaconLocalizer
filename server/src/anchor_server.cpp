
#include "anchor_server.hpp"

AnchorServer::AnchorServer(int port, asio::io_context *ioContext) : _port(port), _ioContext(ioContext)
{
}

AnchorServer::~AnchorServer()
{
}
