#include <iostream>
#include <fstream>
#include <ws_server.hpp>

typedef websocketpp::connection_hdl connection_hdl;
typedef websocketpp::server<websocketpp::config::asio>::message_ptr message_ptr;	// pull out the type of messages sent by our config

// Define a callback to handle incoming messages
void WsServer::on_message(connection_hdl hdl, message_ptr msg)
{
    std::cout 	<< "[on_message]"
				<< "\thdl: " << hdl.lock().get()
              	<< "\tmessage: " << msg->get_payload()
              	<< "\n";

    // check for a special command to instruct the server to stop listening so it can be cleanly exited.
    if (msg->get_payload() == "stop-listening")
		{ stop_listening(); return; }
}

void WsServer::on_http(connection_hdl hdl)
{
	std::string m_docroot = "html/";

	// TODO: non-blocking, asynchronous

	// Upgrade our connection handle to a full connection_ptr
    server::connection_ptr con = get_con_from_hdl(hdl);

    con->defer_http_response();

	std::ifstream file;
	std::string filename = con->get_resource();
	std::string response;

	if (filename == "/") {
		filename = m_docroot+"index.html";
	} else {
		filename = m_docroot+filename.substr(1);
	}

	file.open(filename.c_str(), std::ios::in);
	if (!file) {
		std::stringstream ss;
		ss << "404 (Resource not found)";

		con->set_body(ss.str());
		con->set_status(websocketpp::http::status_code::not_found);
		return;
	}

	file.seekg(0, std::ios::end);
	response.reserve(file.tellg());
	file.seekg(0, std::ios::beg);

	response.assign((std::istreambuf_iterator<char>(file)),
					std::istreambuf_iterator<char>());

	con->set_body(response);
	con->set_status(websocketpp::http::status_code::ok);

	con->send_http_response();
}

void WsServer::sendMsg(connection_hdl hdl, message_ptr msg)
{
	try
	{
        //send(hdl, msg->get_payload(), msg->get_opcode());
    }
	catch (const websocketpp::lib::error_code& e)
	{ std::cout << "Send failed: " << e << "(" << e.message() << ")\n"; }
}
