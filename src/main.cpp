#include <iostream>
#include <thread>

#include "voyage.hpp"
#include "interface.hpp"
#include "browser.hpp"

/* interface.cpp */
extern Voyage* voyage;
extern WsServer* wsServer;

int main()
{
	std::cout <<
	"\n"
	"*************** �|��y��p��{�� ***************\n"
	"                  2016/4/28\n"
	"                 En Shih (�ۮ�)\n"
	"\n";

	std::thread wsThread = launchWsServer(wsServer);
	launchURL("http://127.0.0.1");

	voyage = new Voyage(); // origin, destination

	std::string msg;
	while (std::getline (std::cin, msg))
		std::cout << execCmd(msg) << "\n";

	wsThread.join();

	//delete voyage;

   	return 0;
}
