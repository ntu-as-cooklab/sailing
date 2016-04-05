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
	"*************** �|��y��p��{�� ***************\n"
	"                  2016/4/5\n"
	"                 En Shih (�ۮ�)\n"
	"\n";

	std::thread wsThread = launchWsServer();
	launchURL("http://127.0.0.1");

	voyage = new Voyage(); // origin, destination

	std::string msg;
	while (std::getline (std::cin, msg))
		std::cout << execCmd(msg) << "\n";

	wsThread.join();

	delete voyage;

   	return 0;
}
