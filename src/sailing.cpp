#include <time.h>
#include "path.hpp"
#include "cfsr/cfsr_data.h"
#include <stdio.h>
#include "sailing.hpp"
#include <cmath>

void sail(path_t* path)
{
    // path->pts.push_back((pathpt_t){path->startdate, path->startloc});
	// printpt(path->pts.back());
	// while(mktime(&path->pts.back().date) < mktime(&path->enddate))
	// 	sail_step(path);
	// printpt(path->pts.back());
}

int movement_factor = 1;
int timestep = 3600; 		// size of timestep = 1 hr = 3600 sec
latlon_t calc_next_place(latlon_t loc, vec2 speed)
{
	// speed: in (m/s)
	loc.lat += movement_factor * timestep/1000 * speed.y/lat2km(loc.lat);
	loc.lon += movement_factor * timestep/1000 * speed.x/lon2km(loc.lat);
	return loc;
}

int sail_step(path_t* path)
{
	// Ocean current:

	pathpt_t pt = path->pts.back();

	vec2 ocean = cfsr_ocn(pt.date, pt.loc);
	//printf("ocean: %f %f (%f) %f\n", ocean.x, ocean.y, norm(ocean), norm2(ocean));
	if (std::isnan(norm2(ocean)) || norm2(ocean) < 0 || (norm2(ocean) > 1e3)) {
		printf("Land collision!\n");
		return -1;
	}

	// vec2 wind = {0, 0};
	// vec2 sail_gain = {0, 0};

	// if (sailopen)
	// {
	// 	wind = cfsr_wind(date, loc) // * pow(altitude/10.0bu,alpha); // calculate wind speed at (2m) from wind speed at 10m using wind profile power law
	// 	if (norm(wind) > 1e3) return;

	// 	// adjust boat direction and calculate boat speed gain due to wind
	// 	sail_dir = normalize(
	// 					mode == WIND ? wind :
	// 					mode == DIRN ? dir :
	// 					mode == DEST ? adj_direction(curr, dest) :
	// 					0
	// 				);
	// 	sail_gain = calc_sail_gain(wind, sail_dir); // boat speed gain due to wind
	// }

	// total speed
	vec2 gain = ocean; // + sail_gain;

	// calculate next place
	struct tm next_date = pt.date;
	next_date.tm_hour += 1;
	mktime(&next_date);
	latlon_t next_loc = calc_next_place(pt.loc, gain);
	pathpt_t next_pt = {.date = next_date, .loc = next_loc};
	path->pts.push_back(next_pt);
	char next_date_str[30];
    strftime(next_date_str, sizeof(next_date_str), "%Y-%m-%d %Hhr", &next_date); 

	// printf("%s %f,%f (%f,%f)\n", next_date_str, next_loc.lat, next_loc.lon, gain.x, gain.y);
	return 0;
}