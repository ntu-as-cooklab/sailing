#include <stdio.h>
#include <thread>

#include <voyage.hpp>
#include <server.hpp>

int main()
{
	Voyage voyage(LatLon(30.791412, 122.269901), LatLon(33.823205, 134.887155)); // origin, destination

	TCPserver tcpServer(1234);
	std::thread tcpSeverThread(std::ref(tcpServer));
	std::thread voyageThread(std::ref(voyage));
	voyageThread.join();
	printf("\nInput anything to quit.\n");
	char c[1];
	scanf("%c", c);
	tcpServer.stop();
	tcpSeverThread.join();

   	return 0;
}
