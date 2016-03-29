#include <command.hpp>
#include <string>
#include <iostream>
#include <sstream>
#include <boost/algorithm/string/replace.hpp>

void recvCmd(connection_hdl hdl, std::string cmd)
{
	std::stringstream ss(cmd);
	std::string word;
	ss >> word;
	std::stringstream response;

	/** Commands **/

	if 		(word == "echo") {
		response << ss.str();
	}

	/** Variables **/

	else if (word == "orig") {
		response << voyage->orig;
	}
	else if (word == "dest") {
		response << voyage->dest;
	}
	else if (word == "timestep") {
		response << voyage->timestep;
	}
	else if (word == "movement_factor") {
		response << voyage->movement_factor;
	}
	else if (word == "alpha") {
		response << voyage->alpha;
	}
	else if (word == "altitude") {
		response << voyage->altitude;
	}
	else if (word == "range") {
		response << voyage->range;
	}
	else if (word == "sail_open") {
		response << voyage->sail_open;
	}
	else if (word == "dir") {
		if (voyage->dir) response << *voyage->dir;
	}

	/** Assignment **/

	else if (word == "orig=") {
		ss >> voyage->orig;
		response << word << voyage->orig;
	}
	else if (word == "dest=") {
		ss >> voyage->dest;
		response << word << voyage->dest;
	}
	else if (word == "timestep=") {
		ss >> voyage->timestep;
		response << word << voyage->timestep;
	}
	else if (word == "movement_factor=") {
		ss >> voyage->movement_factor;
		response << word << voyage->movement_factor;
	}
	else if (word == "alpha=") {
		ss >> voyage->alpha;
		response << word << voyage->alpha;
	}
	else if (word == "altitude=") {
		ss >> voyage->altitude;
		response << word << voyage->altitude;
	}
	else if (word == "range=") {
		ss >> voyage->range;
		response << word << voyage->range;
	}
	else if (word == "sail_open=") {
		ss >> voyage->sail_open;
		response << word << voyage->sail_open;
	}
	else if (word == "dir=") {
		if (voyage->dir) {
			ss >> *voyage->dir;
			response << word << *voyage->dir;
		}
	}

	/** **/

	else if (word == "help") {
		response <<
		"\n******** Variable commands:\n"
		"orig\n"
		"dest\n"
		"timestep\n"
		"movement_factor\n"
		"alpha\n"
		"altitude\n"
		"range\n"
		"sail_open\n"
		"dir\n"
		"******** Assignment commands:\n"
		"orig= (lat) (lon)\n"
		"dest= (lat) (lon)\n"
		"timestep= (size of timestep in seconds)\n"
		"movement_factor= (movement_factor)\n"
		"alpha= (alpha)\n"
		"altitude= (altitude in m)\n"
		"range= (range)\n"
		"sail_open= (0/1)\n"
		"dir= (u) (v)\n"
		;
	}

	else {
		response << "Invalid command: \"" << word << "\"";
	}

	std::string response_str = response.str();
	std::cout << "\n[" << hdl.lock().get() << "] " << response_str;
	boost::replace_all(response_str, "\n", "</br>");
	wsServer->sendMsg(hdl, response_str);
}
