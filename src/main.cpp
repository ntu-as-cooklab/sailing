#include <stdio.h>
#include <thread>

#include <voyage.hpp>
#include <server.hpp>
#include <browser.hpp>

int main()
{

	// Initialise the server.
	launchURL("http://127.0.0.1:1234");

	http::server::server httpServer("127.0.0.1", "1234", "html/");
	std::thread httpServerThread(std::ref(httpServer));

	Voyage voyage(LatLon(24.804306, 122.086688), LatLon(33.823205, 134.887155)); // origin, destination
	std::thread voyageThread(std::ref(voyage));
	voyageThread.join();

	httpServerThread.join();

   	return 0;
}
