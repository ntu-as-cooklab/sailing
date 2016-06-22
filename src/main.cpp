#include <iostream>
#include <thread>
#include <windows.h>

#include "voyage.hpp"
#include "interface.hpp"
#include "browser.hpp"

/* interface.cpp */
extern Voyage* voyage;
extern WsServer* wsServer;

int main()
{
	SetConsoleOutputCP(65001);

	std::cout <<
	"\n"
	"*************** 帆船軌跡計算程式 ***************\n"
	"                  2016/6/22\n"
	"                 En Shih (石恩)\n"
	"\n";

	std::thread wsThread = launchWsServer(wsServer);
	launchURL("http://127.0.0.1:8000");

	voyage = new Voyage(); // origin, destination

	std::string msg;
	while (std::getline (std::cin, msg))
		std::cout << execCmd(msg) << "\n";

	wsThread.join();

	//delete voyage;

   	return 0;
}
