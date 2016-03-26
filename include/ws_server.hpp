#ifndef WSSERVER_HPP
#define WSSERVER_HPP

#include <websocketpp/server.hpp>
#include <websocketpp/config/asio_no_tls.hpp>

#include <os.h>

typedef websocketpp::connection_hdl connection_hdl;
typedef websocketpp::server<websocketpp::config::asio>::message_ptr message_ptr;	// pull out the type of messages sent by our config

class WsServer : protected websocketpp::server<websocketpp::config::asio>
{
public:

	void operator()()
	{
		try
		{
			// Set logging settings
			set_access_channels(websocketpp::log::alevel::all);
			clear_access_channels(websocketpp::log::alevel::frame_payload);

			init_asio();			// Initialize Asio
			using websocketpp::lib::placeholders::_1;
			using websocketpp::lib::placeholders::_2;
			set_message_handler(websocketpp::lib::bind(&on_message, this, _1, _2));
			set_http_handler(websocketpp::lib::bind(&on_http, this, _1));
			listen(80); 			// Listen on port
			start_accept(); 		// Start the server accept loop
			run();					// Start the ASIO io_service run loop
		}
		catch (websocketpp::exception const & e) { std::cout << e.what() << "\n"; }
		catch (...) { std::cout << "other exception" << "\n"; }
	}

	void on_message(connection_hdl hdl, message_ptr msg);
	void on_http(connection_hdl hdl);
	void sendMsg(connection_hdl hdl, message_ptr msg);
};

#endif
