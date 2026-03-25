#include "App.h"
#include <iostream>
#include <string_view>

struct PerSocketData
{
};

int main()
{
    uWS::App()
        .ws<struct PerSocketData>("/*", {.open = [](auto *ws) { std::cout << "Client connected\n"; },
                                         .message = [](auto *ws, std::string_view message, uWS::OpCode opCode) {
                // Echo the message back
                ws->send(message, opCode); },
                                         .close = [](auto *ws, int code, std::string_view message) { std::cout << "Client disconnected\n"; }})
        .listen(9001, [](auto *listenSocket) {
            if (listenSocket)
            {
                std::cout << "Listening on port 9001\n";
            }
            else
            {
                std::cerr << "Failed to listen on port 9001\n";
            }
        })
        .run();

    return 0;
}
