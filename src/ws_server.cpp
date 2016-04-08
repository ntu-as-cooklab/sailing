#include <iostream>
#include <fstream>
#include <thread>

#include "interface.hpp"
#include "ws_server.hpp"

void WsServer::on_open(connection_hdl hdl)
	{ connections.insert(hdl); }

void WsServer::on_close(connection_hdl hdl)
	{ connections.erase(hdl); }

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
		filename = docroot + "html/index.html";
	else if (filename == "/console")
		filename = docroot + "html/console.html";
	else
		filename = docroot + filename.substr(1);

	std::ifstream file;
	file.open(filename.c_str(), std::ios::binary);
	if (!file)
	{
		filename = docroot + "html/index.html";
		file.open(filename.c_str(), std::ios::in);
		con->set_status(websocketpp::http::status_code::not_found);
	}
	else con->set_status(websocketpp::http::status_code::ok);

	std::string response;
	file.seekg(0, std::ios::end);
	response.reserve(file.tellg());
	file.seekg(0, std::ios::beg);
	response.assign((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	con->set_body(response);

	con->send_http_response();
}

void WsServer::sendMsg(connection_hdl hdl, std::string msg)
{
	try
		{ send(hdl, msg.c_str(), websocketpp::frame::opcode::text); }
	catch (const websocketpp::lib::error_code& e)
		{ std::cout << "Send failed: " << e << "(" << e.message() << ")\n"; }
}

void WsServer::sendAll(std::string msg)
{
	for (con_list::iterator it = connections.begin(); it != connections.end(); ++it)
		sendMsg(*it, msg);
}

void sendMsg(WsServer* server, connection_hdl hdl, std::string msg)
	{ server->sendMsg(hdl, msg); }

void sendAll(WsServer* server, std::string msg)
	{ server->sendAll(msg); }

std::thread launchWsServer(WsServer *& server)
{
	return server ? NULL : (server = new WsServer), std::thread(std::ref(*server));
}
