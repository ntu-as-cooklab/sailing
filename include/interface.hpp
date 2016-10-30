#ifndef INTERFACE_CPP
#define INTERFACE_CPP

#include <string>
#include <thread>

/* ws_server.cpp */
class WsServer;
typedef class std::weak_ptr<void> connection_hdl;
void sendMsg(WsServer* server, connection_hdl hdl, std::string msg);
void sendAll(WsServer* server, std::string msg);
void stop(WsServer* server);
std::thread launchWsServer(WsServer*& server);

/* command.cpp */
std::string execCmd(std::string cmd);
void recvCmd(connection_hdl hdl, std::string cmd);

#endif
