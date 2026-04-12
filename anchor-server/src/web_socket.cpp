
#include "web_socket.hpp"
#include "App.h"
#include "WebSocket.h"
#include "WebSocketProtocol.h"
#include "ws_protocol.hpp"
#include <cstdio>
#include <vector>

#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))


struct AppContext
{
    int counter;
};

typedef uWS::WebSocket<false, true, AppContext> AppWebSocket;


static std::vector<AppWebSocket *> websockets;

static void wsOpen(AppWebSocket *ws)
{
    std::cout << "Client connected\n";
    websockets.push_back(ws);
}

static void wsMessage(AppWebSocket *ws, std::string_view msg, uWS::OpCode opCode)
{

    if (msg.size() < sizeof(PktHeader))
    {
        return;
    }

    WsProtocolPacket packet;
    memcpy(&packet, msg.data(), MIN(sizeof(WsProtocolPacket), msg.size()));

    switch (packet.header.cmdId)
    {
        case WS_CMD_ID_REGISTER_BEACON:
            std::cout << "Register\n";
            break;
        case WS_CMD_ID_REQUEST_BEACONS:
            std::cout << "Request\n";
            // auto cmd = packet.requestBeacons;
            // Send all the beacons
            break;
        case WS_CMD_ID_UNKNOWN:
        default:
            std::cout << "Unknown\n";
            break;
    }
}

static void wsClose(AppWebSocket *ws, int code, std::string_view message)
{
    std::cout << "Client disconnected\n";
    websockets.erase(std::remove_if(websockets.begin(), websockets.end(), [ws](AppWebSocket *wsElement) {
        return ws == wsElement;
    }));
}

void SetupAndRunWebSocket()
{
    uWS::App().ws<AppContext>("/", {.open = [](auto *ws) {
                                        wsOpen(ws);
                                    },
                                    .message = [](auto *ws, std::string_view msg, uWS::OpCode opCode) {
                                        wsMessage(ws, msg, opCode);
                                    },
                                    .close = [](auto *ws, int code, std::string_view message) {
                                        wsClose(ws, code, message);
                                    }})
        .any("/*", [](auto *res, auto *req) {
            std::cout << "Hit fallback: " << req->getUrl() << std::endl;
            res->end("fallback");
        })
        .listen(11001, [](auto *listenSocket) {
            if (listenSocket)
            {
                std::cout << "Listening for connections on port 11001..." << std::endl;
            }
            else
            {
                std::cerr << "FAILED to listen on port 9000!" << std::endl;
            }
        })
        .run();
}
