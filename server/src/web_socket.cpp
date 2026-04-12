
#include "web_socket.hpp"
#include "PerMessageDeflate.h"
#include "WebSocket.h"
#include "WebSocketProtocol.h"
#include <algorithm>
#include <vector>

struct PerSocketData
{
    int counter;
};

typedef uWS::WebSocket<false, true, PerSocketData> websocket;

static std::vector<websocket *> websockets;

uWS::App setupWebSocket()
{
    uWS::App::WebSocketBehavior<PerSocketData> config = (uWS::App::WebSocketBehavior<PerSocketData>) {
        .compression = uWS::DISABLED,
        .maxPayloadLength = 16 * 1024,
        .idleTimeout = 10,
        .open = [](auto *ws)
        {
            websockets.push_back(ws);
        },
        .message = [](auto *ws, std::string_view message, uWS::OpCode opCode)
        {
            // ws->send(message, opCode);
        },
        .drain = [](auto *ws)
        {
            // Not implemented
        },
        .ping = [](auto *ws, std::string_view message)
        {
            // Not implemented
        },
        .pong = [](auto *ws, std::string_view message)
        {
            // Not implemented
        },
        .close = [](auto *ws, int code, std::string_view message)
        {
            auto predicate = [ws](websocket *wsElement)
            {
                return wsElement == ws;
            };
            websockets.erase(std::remove_if(websockets.begin(), websockets.end(), predicate));
        },
    };

    auto app = uWS::App().ws<PerSocketData>("/", std::move(config));

    app.any("/*", [](auto *res, auto *req)
            {
                std::cout << "Hit fallback: " << req->getUrl() << std::endl;
                res->end("fallback");
            });

    app.listen(9000,
               [](auto *listenSocket)
               {
                   if (listenSocket)
                   {
                       std::cout << "Listening for connections..." << std::endl;
                   }
               });

    return app;
}
