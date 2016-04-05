#ifndef VOYAGE_HPP
#define VOYAGE_HPP

#include "latlon.hpp"
#include "uv.hpp"
#include "kml.hpp"
#include "date.hpp"

struct Voyage
{
	/* Status */

	/* Parameters */

	static bool debug;

	LatLon orig, dest, curr;

	int timestep = 3600 * 24; 		// size of timestep = 1 hr = 3600 sec
	int movement_factor = 1; 	// movement factor (forward = 1, reverse = -1)

	float alpha = 0.11;			// parameter for wind profile power law
	float altitude = 2;

	float range = 1.1; 				// for checking if within range of destination
	LatLon bounds = { -32, 255 }; 	// { -16.5, 177.5 }; // beyond --> out of range

	bool sail_open = true;
	UV dir;

	typedef enum { DRIFT, WIND, DIR, DEST } SAILMODE;
	SAILMODE sailmode;

	int	runday = 0, runhour = 0, sailhour = 0; // count simulation time
	Date 	date 	= {1979, 1,  1,  0},
			enddate = {1979, 1, 31, 24};

	/* Constructor */

	Voyage() {}
	Voyage(LatLon orig, LatLon dest) : orig(orig), dest(dest), curr(orig) {}

	// start thread
	void operator()() { sail(); }

	/* Functions */

	float anglediff(UV dir1, UV dir2) { return acos(dot(dir1, dir2)) * 180/PI; } // 計算兩角度間的夾角
	float lat2km(float lat) { return 110.59 + 4e-12*lat + 0.0003*pow(lat,2) + 3e-17*pow(lat,3) - 2e-8*pow(lat,4); } // 計算該緯度的每一緯度應該是多少距離(km)
	float lon2km(float lat) { return 111.28 + 1e-12*lat - 0.0168*pow(lat,2) - 2e-17*pow(lat,3) + 4e-7*pow(lat,4); } // 計算該緯度的每一經度應該是多少距離(km)

	UV adj_direction(LatLon curr, LatLon dest);
	UV calc_sail_gain(UV wind, UV dir);
	LatLon calc_next_place(LatLon curr, UV speed);

	void sail_timestep();
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
