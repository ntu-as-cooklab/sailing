#include "cfsr_interp.h"
#include "cfsr_netcdf.h"
#include <stdint.h>
#include <math.h>

float bilinearDayOUV(int ncid, int day, float i, float j)
{
	int i0 = floorf(i), i1 = ceilf(i),
		j0 = floorf(j), j1 = ceilf(j);

	return 	(i-i0) * (j-j0) * rawOUV(ncid, day, i0, j0) +
			(i-i0) * (j1-j) * rawOUV(ncid, day, i0, j1) +
			(i1-i) * (j-j0) * rawOUV(ncid, day, i1, j0) +
			(i1-i) * (j1-j) * rawOUV(ncid, day, i1, j1);
}

float bilinearDayAUV(int ncid, int day, float i, float j)
{
	int i0 = floorf(i), i1 = ceilf(i),
		j0 = floorf(j), j1 = ceilf(j);

	return 	(i-i0) * (j-j0) * rawAUV(ncid, day, i0, j0) +
			(i-i0) * (j1-j) * rawAUV(ncid, day, i0, j1) +
			(i1-i) * (j-j0) * rawAUV(ncid, day, i1, j0) +
			(i1-i) * (j1-j) * rawAUV(ncid, day, i1, j1);
}

float bilinearOUV(int ncid, float day, float lat, float lon)
{
	float i = (89.75-lat)/0.5, j = ((lon<0.25?lon+360:lon)-0.25)/0.5; // TODO: fix range hack
	int d0 = floorf(day), d1 = ceilf(day);
	return d0 == d1 ?
		bilinearDayOUV(ncid, day, i, j) :
		bilinearDayOUV(ncid, d0, i, j) * (day-d0) + bilinearDayOUV(ncid, d1, i, j) * (d1-day);
}

float bilinearAUV(int ncid, float day, float lat, float lon)
{
	float i = (89.761-lat)/0.31249958, j = (lon<0?lon+360:lon)/0.31249958; // TODO: fix range hack
	int d0 = floorf(day), d1 = ceilf(day);
	return d0 == d1 ?
		bilinearDayAUV(ncid, day, i, j) :
		bilinearDayAUV(ncid, d0, i, j) * (day-d0) + bilinearDayAUV(ncid, d1, i, j) * (d1-day);
}
