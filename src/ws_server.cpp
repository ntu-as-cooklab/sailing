#include <iostream>
#include <ws_server.hpp>

typedef websocketpp::server<websocketpp::config::asio>::message_ptr message_ptr;	// pull out the type of messages sent by our config

// Define a callback to handle incoming messages
void WsServer::on_message(websocketpp::connection_hdl hdl, message_ptr msg)
{
    std::cout 	<< "[on_message]"
				<< "\thdl: " << hdl.lock().get()
              	<< "\tmessage: " << msg->get_payload()
				//<< "\topcode: " << msg->get_opcode()
              	<< "\n";

    // check for a special command to instruct the server to stop listening so it can be cleanly exited.
    if (msg->get_payload() == "stop-listening")
		{ stop_listening(); return; }
}

void WsServer::on_http(websocketpp::connection_hdl hdl)
{
    endpoint::connection_ptr con = get_con_from_hdl(hdl);

    con->defer_http_response();

    // Begin an asynchronous operation and pass as a callback a lambda that sets
    // up the appropriate response and sends it.
    /*asyncronous_function([con](int status)
		{
	        if (status == -1)
			{
	            con->set_body("An error occurred");
	            con->set_status(websocketpp::http::status_code::internal_server_error);
	        }
			else
			{
	            con->set_body("Hello World!");
	            con->set_status(websocketpp::http::status_code::ok);
	        }
	        con->send_http_response();
    	}
	);*/
}

void WsServer::sendMsg(websocketpp::connection_hdl hdl, message_ptr msg)
{
	try
	{
        //send(hdl, msg->get_payload(), msg->get_opcode());
    }
	catch (const websocketpp::lib::error_code& e)
	{ std::cout << "Send failed: " << e << "(" << e.message() << ")\n"; }
}
