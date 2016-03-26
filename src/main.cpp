#include <stdio.h>
#include <thread>

#include <voyage.hpp>
#include <ws_server.hpp>
#include <browser.hpp>

int main()
{
	WsServer wsServer;
	std::thread wsThread(std::ref(wsServer));

	launchURL("http://127.0.0.1");

	Voyage voyage(LatLon(24.804306, 122.086688), LatLon(33.823205, 134.887155)); // origin, destination
	std::thread voyageThread(std::ref(voyage));
	voyageThread.join();

	wsThread.join();

   	return 0;
}
