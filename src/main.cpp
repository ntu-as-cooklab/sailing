#include <iostream>
#include <thread>

#include "voyage.hpp"
#include "interface.hpp"
#include "browser.hpp"

Voyage* voyage;

int main()
{
	std::cout <<
	"\n"
	"*************** ¦|²î­y¸ñ­pºâµ{¦¡ ***************\n"
	"                  2016/3/29\n"
	"                 En Shih (¥Û®¦)\n"
	"\n";

	std::thread wsThread = launchWsServer();
	launchURL("http://127.0.0.1");

	voyage = new Voyage(); // origin, destination

	std::string msg;
	while (std::getline (std::cin, msg))
	{
		std::cout << parseCmd(msg) << "\n";
	}

	//voyageThread.join();
	wsThread.join();

	//delete wsServer;
	delete voyage;

   	return 0;
}
