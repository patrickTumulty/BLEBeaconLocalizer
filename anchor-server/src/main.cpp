#include "App.h"
#include "anchor_server.hpp"
#include "web_socket.hpp"
#include <asio.hpp>
#include <thread>
#include <unistd.h>

int main()
{
    uWS::App app = setupWebSocket();

    asio::io_context ioContext;
    AnchorServer *server;

    try
    {
        server = new AnchorServer(11000, &ioContext);
    }
    catch (std::exception &e)
    {
        delete server;
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    std::thread wsThread([&app]() {
        std::cout << "Starting WebServer\n";
        app.run();
    });

    std::thread ioThread([&ioContext]() {
        std::cout << "Starting IO Context\n";
        ioContext.run();
    });

    ioThread.join();
    wsThread.join();

    std::cout << "Exit\n";

    return 0;
}
