#include <iostream>
#include <thread>

#include <ws_server.hpp>
#include <voyage.hpp>
#include <command.hpp>
#include <os.h>
#include <browser.hpp>

WsServer* wsServer;
Voyage* voyage;

int main()
{
	std::cout <<
	"\n"
	"*************** ¦|²î­y¸ñ­pºâµ{¦¡ ***************\n"
	"                  2016/3/29\n"
	"                 En Shih (¥Û®¦)\n"
	"\n";

	wsServer = new WsServer;
	std::thread wsThread(std::ref(*wsServer));
	launchURL("http://127.0.0.1");

	voyage = new Voyage(); // origin, destination

	std::string msg;
	while (std::getline (std::cin, msg))
	{
		std::cout << parseCmd(msg) << "\n";
	}

	//voyageThread.join();
	wsThread.join();

	delete wsServer;
	delete voyage;

   	return 0;
}
