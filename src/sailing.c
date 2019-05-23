#include <time.h>
#include "cfsr_nc.h"
#include "vec2.h"

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

void sail_step()
{
	// Ocean current:
	// vec2 ocean = cfsr_ocn(date, loc);
	// if (norm(ocean) > 1e3) return;

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
	// gain = ocean + sail_gain;
	// calculate next place
	// curr = calc_next_place(curr, gain);
}