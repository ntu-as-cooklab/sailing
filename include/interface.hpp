#ifndef COMMAND_HPP
#define COMMAND_HPP

#include <string>
#include <thread>

typedef class std::weak_ptr<void> connection_hdl;
//typedef class std::weak_ptr<void> message_ptr;

std::thread launchWsServer();
std::string parseCmd(std::string cmd);
void recvCmd(connection_hdl hdl, std::string cmd);

#endif
