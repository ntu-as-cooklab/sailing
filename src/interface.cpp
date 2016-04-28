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
	int cmdbegin = cmd.find_first_not_of(" ");
	if (cmdbegin != std::string::npos) cmd.erase(0, cmdbegin);
	int cmdbreak = cmd.find_first_of("= ");
	std::string word = cmd.substr(0, cmdbreak);
	std::string params = (cmdbreak != std::string::npos) ? cmd.substr(cmdbreak+1) : "";

	int paramsbegin = params.find_first_not_of("= ");
	if (paramsbegin != std::string::npos) params.erase(0, paramsbegin);
	params.erase(params.find_last_not_of(" ") + 1);

	std::stringstream response;

	/** Commands **/

	if (word == "run") {
		(*voyage)();
		//std::thread voyageThread(std::ref(*voyage));
		//voyageThread.detach();
	}
	else if (word == "send")				sendAll(wsServer, params);
	else if (word == "alert")				;
	else if (word == "reset")				new (voyage) Voyage;
	else if (word == "OU") {
		int year, month, day;
		float lat, lon;
		std::stringstream(params) >> year >> month >> day >> lat >> lon;
		int ouid = openCFSR(CFSR_OU, year, month);
		response << bilinearOUV(ouid, day, lat, lon);
		closeCFSR(ouid);
	}
	else if (word == "OV") {
		int year, month, day;
		float lat, lon;
		std::stringstream(params) >> year >> month >> day >> lat >> lon;
		int ovid = openCFSR(CFSR_OU, year, month);
		response << bilinearOUV(ovid, day, lat, lon);
		closeCFSR(ovid);
	}
	else if (word == "OUV") {
		Date date;
		LatLon latlon;
		std::stringstream(params) >> date >> latlon;
		response << cfsrReader->OUV(date, latlon);
	}

	#define PARAM(param) response << (params.length() ? (std::stringstream(params) >> param, word + " = ") : ""), response << param

	/** Parameters **/

	else if (word == "dataset") 			PARAM(voyage->dataset);
	else if (word == "startdate") 			{ boost::replace_all(params, "-", " "); PARAM(voyage->startdate); }
	else if (word == "enddate") 			{ boost::replace_all(params, "-", " "); PARAM(voyage->enddate); }
	else if (word == "days") 				PARAM(voyage->days);
	else if (word == "mode") 				PARAM(voyage->mode);
	else if (word == "orig") 				{ boost::replace_all(params, ",", " "); PARAM(voyage->orig); }
	else if (word == "dest") 				{ boost::replace_all(params, ",", " "); PARAM(voyage->dest); }
	else if (word == "altitude")			PARAM(voyage->altitude);
	else if (word == "windlimit")			PARAM(voyage->windlimit);
	else if (word == "sailopenhours")		PARAM(voyage->sailopenhours);

	else if (word == "range") 				PARAM(voyage->range);
	else if (word == "sailopen")  			PARAM(voyage->sailopen);
	else if (word == "dir") 				{ boost::replace_all(params, "-", " "); PARAM(voyage->dir); }

	else if (word == "timestep") 			PARAM(voyage->timestep);
	else if (word == "movement_factor") 	PARAM(voyage->movement_factor);
	else if (word == "alpha") 				PARAM(voyage->alpha);

	/** **/

	else if (word == "help") {
		response <<
		"####################################\n"
		"############ General commands:\n"
		"send				(message)\n"
		"run				(run simulation)\n"
		"help				(display this help message)\n"
		"new				(new voyage)\n"
		"############ Parameters:\n"
		"orig				(lat) (lon)\n"
		"dest				(lat) (lon)\n"
		"timestep			(size of timestep in seconds)\n"
		"movement_factor	(movement_factor)\n"
		"alpha				(alpha)\n"
		"altitude			(altitude in m)\n"
		"range				(range)\n"
		"sailopen			(0/1)\n"
		"dir				(u) (v)\n"
		"####################################"
		;
	}

	else response << "Invalid " << (params.length()?"parameter":"command") << ": \"" << word << "\"";

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
