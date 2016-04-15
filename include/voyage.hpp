#ifndef VOYAGE_HPP
#define VOYAGE_HPP

#include <map>

#include "latlon.hpp"
#include "uv.hpp"
#include "kml.hpp"
#include "date.hpp"

struct Voyage
{
	static bool debug;
	static const int AVG=0, ALL=1, SINGLE=2;
	static const int DRIFT=0, WIND=1, DEST=2, DIR=3, RANDOM=4;

	/* Parameters */

	int 	dataset			= AVG;
	Date 	startdate 		= {1979, 1,  1,  0};
	Date	enddate 		= {1979, 1, 31, 24};

	int 	mode 			= DEST;
	LatLon 	orig			= {24.0, 122.0};
	LatLon	dest			= {34.0, 135.0};

	float 	altitude 		= 2;
	float 	windlimit 		= 8;
	int 	sailopenhours 	= 12;
	float 	alpha 			= 0.11;				// parameter for wind profile power law

	int 	timestep 		= 3600 * 24; 		// size of timestep = 1 hr = 3600 sec
	int 	movement_factor = 1; 				// movement factor (forward = 1, reverse = -1)

	float 	range 			= 1.1; 				// for checking if within range of destination
	LatLon 	bounds 			= {-32.0, 255.0}; 	// { -16.5, 177.5 }; // beyond --> out of range

	/* Status */

	Date 	date 			= startdate;
	LatLon 	curr 			= orig;
	bool 	sailopen 		= true;
	UV 		dir				= 0;
	int		runhour 		= 0; // count simulation time
	int		sailhour 		= 0; // count simulation time

	/* Functions */

	// start simulation
	void operator()() { sail(); }

	float anglediff(UV dir1, UV dir2) { return acos(dot(dir1, dir2)) * 180/PI; } // 計算兩角度間的夾角
	float lat2km(float lat) { return 110.59 + 4e-12*lat + 0.0003*pow(lat,2) + 3e-17*pow(lat,3) - 2e-8*pow(lat,4); } // 計算該緯度的每一緯度應該是多少距離(km)
	float lon2km(float lat) { return 111.28 + 1e-12*lat - 0.0168*pow(lat,2) - 2e-17*pow(lat,3) + 4e-7*pow(lat,4); } // 計算該緯度的每一經度應該是多少距離(km)

	UV adj_direction(LatLon curr, LatLon dest);
	UV calc_sail_gain(UV wind, UV dir);
	LatLon calc_next_place(LatLon curr, UV speed);

	void step();
	bool sail();

	KML kml;

private:
	// Sums --> used for averages
	float 	Total_Ocean_Sp    = 0,
			Total_Ocean_Angle = 0,
			Total_Wind_Sp     = 0,
			Total_Wind_Angle  = 0,
			Total_Wind_Gain   = 0, // speed gain due to wind
			Total_Ocean_Gain  = 0; // speed gain due to ocean
};

#endif
