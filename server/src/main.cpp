#include <iostream>
#include <asio.hpp>
#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

typedef websocketpp::server<websocketpp::config::asio> server;

void on_message(server *s, websocketpp::connection_hdl hdl, server::message_ptr msg)
{
    s->send(hdl, msg->get_payload(), msg->get_opcode());
}

int main()
{
    server echo_server;

    try
    {
        echo_server.init_asio();
        echo_server.set_message_handler(bind(&on_message, &echo_server, std::placeholders::_1, std::placeholders::_2));

        echo_server.listen(9002);
        echo_server.start_accept();

        std::cout << "Server started on port 9002..." << std::endl;
        echo_server.run();
    }
    catch (websocketpp::exception const &e)
    {
        std::cout << e.what() << std::endl;
    }
}
