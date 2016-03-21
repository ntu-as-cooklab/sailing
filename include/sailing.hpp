#ifndef SAILING_HPP
#define SAILING_HPP

#include <latlon.hpp>
#include <uv.hpp>
#include <kml.hpp>

struct Voyage
{
	/* Parameters */

	LatLon orig, dest;

	int timestep = 3600; 		// size of timestep = 1 hr = 3600 sec
	int movement_factor = 1; 	// movement factor (forward = 1, reverse = -1)

	float alpha = 0.11;			// parameter for wind profile power law

	float range = 1.1; 				// for checking if within range of destination
	LatLon bounds = { -32, 255 }; 	// { -16.5, 177.5 }; // beyond --> out of range

	/* Constructor */

	Voyage(LatLon orig, LatLon dest) : orig(orig), dest(dest), kml("voyage.kml") {}

	/* Functions */

	float anglediff(UV dir1, UV dir2) { return acos(dot(dir1, dir2)) * 180/PI; } // 計算兩角度間的夾角
	// 計算該緯度的每一緯度應該是多少距離(km)
	float lat2km(float lat) { return 110.59 + 4e-12*lat + 0.0003*pow(lat,2) + 3e-17*pow(lat,3) - 2e-8*pow(lat,4); }
	// 計算該緯度的每一經度應該是多少距離(km)
	float lon2km(float lat) { return 111.28 + 1e-12*lat - 0.0168*pow(lat,2) - 2e-17*pow(lat,3) + 4e-7*pow(lat,4); }

	UV adj_direction(LatLon curr, LatLon dest);
	float boat_speed(UV wind, UV dir);
	LatLon calcu_next_place(LatLon curr, UV speed);

	bool sail();

	KML kml;
};

#endif
