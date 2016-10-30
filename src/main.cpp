#include <iostream>
#include <thread>
#ifdef _WIN32
	#include <windows.h>
#endif

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
	#ifdef _WIN32
		SetConsoleOutputCP(65001);
	#endif

	std::cout <<
	"\n"
	"*************** 帆船軌跡計算程式 ***************\n"
	"                  " QUOTE(BUILD_DATE) "\n"
	"                 En Shih (石恩)\n"
	"              r01222055@ntu.edu.tw\n"
	"\n";

	std::thread wsThread = launchWsServer(wsServer);
	//launchURL("http://127.0.0.1:8000");

	voyage = new Voyage(); // origin, destination

	std::string msg;
	while (std::getline (std::cin, msg))
		std::cout << execCmd(msg) << "\n";

	//execCmd("stop");
	wsThread.join();

	//wsServer->stop();

	//delete voyage;

   	return 0;
}
