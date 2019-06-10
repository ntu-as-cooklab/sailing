#include <time.h>
#include "path.hpp"
#include "cfsr/cfsr_data.h"
#include <stdio.h>
#include "model.hpp"
#include <cmath>
#include "sailing.h"

using namespace std;

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
	pathpt_t* pt = &path->pts.back();

	// Ocean current
	pt->ocean = cfsr_ocn(pt->date, pt->loc);
	if (isnan(norm2(pt->ocean))) return -1;
	pt->gain = pt->ocean;

	// Sailing
	if (path->mode != MODE_DRFT && 
			((path->sailstarthour < path->sailendhour && pt->date.tm_hour > path->sailstarthour && pt->date.tm_hour < path->sailendhour) 
			|| (path->sailstarthour > path->sailendhour && pt->date.tm_hour > path->sailstarthour) 
		))
	{
		pt->wind = cfsr_wnd(pt->date, pt->loc); // * pow(altitude/10.0bu,alpha); // calculate wind speed at (2m) from wind speed at 10m using wind profile power law
		if (!isnan(norm2(pt->wind)))
		{
			// adjust boat direction and calculate boat speed gain due to wind
			vec2 sail_dir = {0, 0};
			switch (path->mode)
			{
				case MODE_WIND: sail_dir = normalize(pt->wind); break;
				case MODE_DIRN: sail_dir = normalize(heading2dir(path->destheading)); break;
				case MODE_DEST: sail_dir = normalize(adj_direction(pt->loc, path->destloc)); break;
			}
			pt->wind_gain = calc_sail_gain(pt->wind, sail_dir); // boat speed gain due to wind
			pt->gain.x += pt->wind_gain.x; pt->gain.y += pt->wind_gain.y;
		}
	}

	// calculate next place
	struct tm next_date = pt->date;
	next_date.tm_hour += 1;
	mktime(&next_date);
	latlon_t next_loc = calc_next_place(pt->loc, pt->gain);
	pathpt_t next_pt = {.date = next_date, .loc = next_loc};
	path->pts.push_back(next_pt);
	char next_date_str[30];
    strftime(next_date_str, sizeof(next_date_str), "%Y-%m-%d %Hhr", &next_date); 

	// printf("%s %f,%f (%f,%f)\n", next_date_str, next_loc.lat, next_loc.lon, pt.gain.x, pt.gain.y);
	return 0;
}