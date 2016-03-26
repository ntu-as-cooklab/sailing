#include <stdio.h>
#include <thread>

#include <ws_server.hpp>
#include <voyage.hpp>
#include <browser.hpp>

int main()
{
	WsServer* wsServer = new WsServer;
	std::thread wsThread(std::ref(*wsServer));
	launchURL("http://127.0.0.1");

	Voyage* voyage = new Voyage(LatLon(24.804306, 122.086688), LatLon(33.823205, 134.887155)); // origin, destination
	std::thread voyageThread(std::ref(*voyage));

	char c;
	while(1) { std::cin >> c; }

	if (voyageThread.joinable()) voyageThread.join();
	if (wsThread.joinable()) wsThread.join();

	delete wsServer;
	delete voyage;

   	return 0;
}
