#include "App.h"
#include "anchor_server.hpp"
#include "web_socket.hpp"
#include <asio.hpp>
#include <thread>
#include <unistd.h>

int main()
{
    uWS::App app = setupWebSocket();

    // std::thread thread([]()
    //                    {
    //                        int counter = 0;
    //                        while (1)
    //                        {
    //                            sleep(1);
    //
    //                            counter++;
    //
    //                            std::string_view sv(reinterpret_cast<char *>(&counter), 4);
    //
    //                            for (auto ws : websockets)
    //                            {
    //                                ws->send(sv);
    //                            }
    //                        }
    //                    });


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

    std::thread wsThread([&app]()
                         {
                             app.run();
                         });

    std::thread ioThread([&ioContext]()
                         {
                             ioContext.run();
                         });

    ioThread.join();
    wsThread.join();
}
