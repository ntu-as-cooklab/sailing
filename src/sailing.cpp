#include <time.h>
#include "cfsr_nc.h"
#include "vec2.h"
#include "path.hpp"
#include "cfsr_data.h"
#include <stdio.h>

void sail(struct tm startdate, struct tm enddate)
{
    // for (runstep = 0, sailstep = 0, curr = orig, date = startdate; ++date < enddate; runstep++)
	// {
	// 	if ((mode == SIN || mode == AVG) && date.month == 1 && date.day == 1 && date.hour == 0)
	// 		date.year--, enddate.year--;
	// 	sailstep += (sailopen = date.hour<=sailopenhours); // only open sail for sailopenhours
	// 	step();
	// 	if (ocean.norm() > 1e3 || wind.norm() > 1e3) break;
	// }
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

void sail_step(path_t& path)
{
	// Ocean current:

	pathpt_t pt = path.pts.back();

	vec2 ocean = cfsr_ocn(pt.date, pt.loc);
	if (norm(ocean) > 1e3) return;

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
	path.pts.push_back(next_pt);
	printf("%u-%02u-%02u %02uhr %f,%f\n", next_date.tm_year, next_date.tm_mon+1, next_date.tm_mday, next_date.tm_hour, next_loc.lat, next_loc.lon);
}