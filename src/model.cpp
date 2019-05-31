#include <time.h>
#include "path.hpp"
#include "cfsr/cfsr_data.h"
#include <stdio.h>
#include "model.hpp"
#include <cmath>
#include "sailing.h"

using namespace std;

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
	pathpt_t pt = path->pts.back();

	// Ocean current
	vec2 ocean = cfsr_ocn(pt.date, pt.loc);
	if (isnan(norm2(ocean))) return -1;
	vec2 gain = ocean;

	// Sailing
	if (path->mode != MODE_DRFT)
	//if (sailopen)
	{
		vec2 wind = cfsr_wnd(pt.date, pt.loc); // * pow(altitude/10.0bu,alpha); // calculate wind speed at (2m) from wind speed at 10m using wind profile power law
		if (!isnan(norm2(wind)))
		{
			// adjust boat direction and calculate boat speed gain due to wind
			vec2 sail_dir = {0, 0};
			switch (path->mode)
			{
				case MODE_WIND: sail_dir = normalize(wind); break;
				case MODE_DIRN: sail_dir = normalize(path->destdir); break;
				case MODE_DEST: sail_dir = normalize(adj_direction(pt.loc, path->destloc)); break;
			}
			vec2 sail_gain = calc_sail_gain(wind, sail_dir); // boat speed gain due to wind
			gain.x += sail_gain.x; gain.y += sail_gain.y;
		}
	}

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