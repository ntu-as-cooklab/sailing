#ifndef VOYAGE_HPP
#define VOYAGE_HPP

#include <map>

#include "latlon.hpp"
#include "uv.hpp"
#include "kml.hpp"
#include "csv.hpp"
#include "date.hpp"
#include "json.hpp"

struct Voyage
{
	static bool debug;
	static const int AVG=0, ALL=1, SIN=2;
	static const constexpr char* DATASET[3] = {"AVG", "ALL", "SIN"};
	static const int DRFT=0, WIND=1, DEST=2, DIRN=3, RAND=4;
	static const constexpr char* MODE[5] = {"DRFT", "WIND", "DEST", "DIRN", "RAND"};

	std::string name;
	std::string project;

	std::string loginID;
	int runId;

	/* Parameters */

	int 	dataset			= SIN;
	Date 	startdate 		= {1979, 1,  1,  0};
	Date	enddate 		= {1979, 1, 16, 0};
	int 	days			= 15;

	int 	mode 			= DEST;
	LatLon 	orig			= {-10.0, 160.0};
	LatLon	dest			= {-14.0, 188.5};

	float 	altitude 		= 2;
	float 	windlimit 		= 8;
	int 	sailopenhours 	= 12;
	float 	alpha 			= 0.11;				// parameter for wind profile power law

	/* Status */

	Date 	date 			= startdate;
	LatLon 	curr 			= orig;
	bool 	sailopen 		= true;
	UV 		dir				= 0;
	int		runstep 		= 0; // count simulation time
	int		sailstep 		= 0; // count simulation time

	/* Functions */
	
	UV adj_direction(LatLon curr, LatLon dest);
	UV calc_sail_gain(UV wind, UV dir);
	LatLon calc_next_place(LatLon curr, UV speed);

	KML kml;
	CSV csv;
	JSON json;
};

#endif
