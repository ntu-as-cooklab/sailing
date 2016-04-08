#include <iostream>
#include <string>
#include <sstream>
#include <boost/algorithm/string/replace.hpp>
#include <iomanip> // for std::setprecision

#include "cfsr.hpp"
#include "voyage.hpp"
#include "interface.hpp"

Voyage* 	voyage;
WsServer* 	wsServer;
CfsrReader* cfsrReader = new CfsrReader;

std::string execCmd(std::string cmd)
{
	std::stringstream ss(cmd);
	std::string word;
	ss >> word;
	std::stringstream response;

	/** Commands **/

	if 		(word == "echo") {
		std::string msg;
		getline(ss, msg);
		response << msg;
	}
	else if (word == "run") {
		(*voyage)();
		//std::thread voyageThread(std::ref(*voyage));
		//voyageThread.detach();
	}
	else if (word == "send") {
		std::string msg;
		getline(ss, msg);
		sendAll(wsServer, msg);
	}
	else if (word == "OU") {
		int year, month, day;
		float lat, lon;
		ss >> year >> month >> day >> lat >> lon;
		int ouid = openCFSR(CFSR_OU, year, month);
		response << getOUV(ouid, day, lat, lon);
		closeCFSR(ouid);
	}
	else if (word == "OV") {
		int year, month, day;
		float lat, lon;
		ss >> year >> month >> day >> lat >> lon;
		int ovid = openCFSR(CFSR_OU, year, month);
		response << getOUV(ovid, day, lat, lon);
		closeCFSR(ovid);
	}
	else if (word == "OUV") {
		Date date;
		LatLon latlon;
		ss >> date >> latlon;
		response << cfsrReader->OUV(date, latlon);
	}

	/** Variables **/

	else if (word == "orig") {
		response << voyage->orig;
	}
	else if (word == "dest") {
		response << voyage->dest;
	}
	else if (word == "curr") {
		response << voyage->curr;
	}
	else if (word == "timestep") {
		response << voyage->timestep;
	}
	else if (word == "movement_factor") {
		response << voyage->movement_factor;
	}
	else if (word == "alpha") {
		response << std::fixed << std::setprecision(2) << voyage->alpha;
	}
	else if (word == "altitude") {
		response << std::fixed << std::setprecision(2) << voyage->altitude;
	}
	else if (word == "range") {
		response << std::fixed << std::setprecision(2) << voyage->range;
	}
	else if (word == "sail_open") {
		response << voyage->sail_open;
	}
	else if (word == "dir") {
		response << voyage->dir;
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
		response << word << std::fixed << std::setprecision(2) << voyage->alpha;
	}
	else if (word == "altitude=") {
		ss >> voyage->altitude;
		response << word << std::fixed << std::setprecision(2) << voyage->altitude;
	}
	else if (word == "range=") {
		ss >> voyage->range;
		response << word << std::fixed << std::setprecision(2) << voyage->range;
	}
	else if (word == "sail_open=") {
		ss >> voyage->sail_open;
		response << word << voyage->sail_open;
	}
	else if (word == "dir=") {
		ss >> voyage->dir;
		response << word << voyage->dir;
	}

	/** **/

	else if (word == "help") {
		response <<
		"####################################\n"
		"############ General commands:\n"
		"echo				(message)\n"
		"send				(message)\n"
		"run\n"
		"help\n"
		"############ Variables:\n"
		"orig\n"
		"dest\n"
		"timestep\n"
		"movement_factor\n"
		"alpha\n"
		"altitude\n"
		"range\n"
		"sail_open\n"
		"dir\n"
		"############ Assignment commands:\n"
		"orig=          	(lat) (lon)\n"
		"dest=          	(lat) (lon)\n"
		"timestep=			(size of timestep in seconds)\n"
		"movement_factor=	(movement_factor)\n"
		"alpha=				(alpha)\n"
		"altitude= 			(altitude in m)\n"
		"range= 			(range)\n"
		"sail_open= 		(0/1)\n"
		"dir= 				(u) (v)\n"
		"####################################"
		;
	}

	else response << "Invalid command: \"" << word << "\"";

	return response.str();
}

void recvCmd(connection_hdl hdl, std::string cmd)
{
	std::string response = execCmd(cmd);
	std::cout << "[" << hdl.lock().get() << "] " << response << "\n";
	boost::replace_all(response, "\n", "</br>");
	boost::replace_all(response, "\t", " ");
	sendMsg(wsServer, hdl, response);
}
