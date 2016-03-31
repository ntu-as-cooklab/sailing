#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <ws_server.hpp>
#include <voyage.hpp>

extern WsServer* wsServer;
extern Voyage* voyage;

std::string parseCmd(std::string cmd);
void recvCmd(connection_hdl hdl, std::string cmd);

#endif
