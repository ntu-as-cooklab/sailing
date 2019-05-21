#include <iostream>
#include <thread>

#include "voyage.hpp"
#include "interface.hpp"
#include "browser.hpp"

/* interface.cpp */
extern Voyage* voyage;
extern WsServer* wsServer;

#define Q(x) #x
#define QUOTE(x) Q(x)

int main()
{
	std::cout <<
	"\n"
	"*************** 帆船軌跡計算程式 ***************\n"
	"                  " QUOTE(BUILD_DATE) "\n"
	"\n";

	std::thread wsThread = launchWsServer(wsServer);

	std::string msg;
	while (std::getline (std::cin, msg))
	; //	std::cout << execCmd(msg) << "\n";

	wsThread.join();

   	return 0;
}
