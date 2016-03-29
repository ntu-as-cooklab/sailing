#include <iostream>
#include <fstream>
#include <ws_server.hpp>
#include <command.hpp>

typedef websocketpp::connection_hdl connection_hdl;
typedef websocketpp::server<websocketpp::config::asio>::message_ptr message_ptr;	// pull out the type of messages sent by our config
typedef std::set<connection_hdl,std::owner_less<connection_hdl>> con_list;

void WsServer::on_open(connection_hdl hdl)
{
	connections.insert(hdl);
}

void WsServer::on_close(connection_hdl hdl)
{
	connections.erase(hdl);
}

// Define a callback to handle incoming messages
void WsServer::on_message(connection_hdl hdl, message_ptr msg)
{
	recvCmd(hdl, msg->get_payload());

    // check for a special command to instruct the server to stop listening so it can be cleanly exited.
    if (msg->get_payload() == "stop-listening")
		stop_listening();
}

void WsServer::on_http(connection_hdl hdl)
{
	// Upgrade our connection handle to a full connection_ptr
    server::connection_ptr con = get_con_from_hdl(hdl);

    con->defer_http_response();
	std::string filename = con->get_resource();

	if (filename == "/")
		filename = docroot + "index.html";
	else
		filename = docroot + filename.substr(1);

	std::ifstream file;
	file.open(filename.c_str(), std::ios::in);
	if (!file)
	{
		//con->set_body("404");
		//con->set_status(websocketpp::http::status_code::not_found);
		filename = docroot + "index.html";
		file.open(filename.c_str(), std::ios::in);
	}

	std::string response;
	file.seekg(0, std::ios::end);
	response.reserve(file.tellg());
	file.seekg(0, std::ios::beg);

	response.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

	con->set_body(response);
	con->set_status(websocketpp::http::status_code::ok);

	con->send_http_response();
}

void WsServer::sendMsg(connection_hdl hdl, std::string msg)
{
	try
	{
		send(hdl, msg.c_str(), websocketpp::frame::opcode::text);
    }
	catch (const websocketpp::lib::error_code& e)
	{ std::cout << "Send failed: " << e << "(" << e.message() << ")\n"; }
}

void WsServer::sendAll(std::string msg)
{
	for (con_list::iterator it = connections.begin(); it != connections.end(); ++it)
		sendMsg(*it, msg);
}
