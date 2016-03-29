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
	"********** ¦|²î­y¸ñ­pºâµ{¦¡ **********\n"
	"             2016/3/29\n"
	"           En Shih (¥Û®¦)\n"
	"\n";

	wsServer = new WsServer;
	std::thread wsThread(std::ref(*wsServer));
	launchURL("http://127.0.0.1");

	voyage = new Voyage(LatLon(24.804306, 122.086688), LatLon(33.823205, 134.887155)); // origin, destination
	std::thread voyageThread(std::ref(*voyage));

	std::string msg;
	while (std::cin >> msg)
	{
		wsServer->sendAll(msg);
	}

	voyageThread.join();
	wsThread.join();

	delete wsServer;
	delete voyage;

   	return 0;
}
