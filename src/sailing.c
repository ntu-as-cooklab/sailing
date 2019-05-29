#include "sailing.h"
#include "vec2.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include "latlon.h"

double heading(vec2 a)
{
	double h = 90 - atan2(a.y, a.x) * 180/M_PI;
	return norm(a) > 0 ? (h < 0 ? h+360 : h) : 0;
}

// 計算兩角度間的夾角
double anglediff(vec2 dir1, vec2 dir2)
{ 
	return acos(dot(normalize(dir1), normalize(dir2))) * 180/M_PI;
}

vec2 adj_direction(latlon_t curr, latlon_t dest)
{
	return normalize((vec2){lon2km((curr.lat+dest.lat)/2) * (dest.lon-curr.lon), lat2km((curr.lat+dest.lat)/2) * (dest.lat-curr.lat)});
}

double windlimit = 8;

vec2 calc_sail_gain(vec2 wind, vec2 dir)
{
	double wind_speed = norm(wind)<windlimit ? norm(wind) : 0;
	double angle_diff = anglediff(wind, dir);
	double boat_speed = 0;

	if (angle_diff <= 30)
	    boat_speed = (-0.0008*pow(wind_speed,3) - 0.0037*pow(wind_speed,2) + 0.5548*wind_speed ) * ( -0.0333*angle_diff+1 ) +
	              ( 0.0023*pow(wind_speed,3) - 0.0699*pow(wind_speed,2) + 0.9546*wind_speed ) * ( 0.0333*angle_diff );
	else if (angle_diff <= 60 && angle_diff > 30)
	    boat_speed = ( 0.0023*pow(wind_speed,3) - 0.0699*pow(wind_speed,2) + 0.9546*wind_speed ) * ( -0.0333*angle_diff+2 ) +
	              ( 0.0065*pow(wind_speed,3) - 0.1715*pow(wind_speed,2) + 1.5687*wind_speed ) * ( 0.0333*angle_diff-1 ) ;
	else if (angle_diff <= 90 && angle_diff > 60)
	    boat_speed = ( 0.0065*pow(wind_speed,3) - 0.1715*pow(wind_speed,2) + 1.5687*wind_speed ) * ( -0.0333*angle_diff+3 ) +
	              ( 0.0057*pow(wind_speed,3) - 0.1596*pow(wind_speed,2) + 1.5091*wind_speed ) * ( 0.0333*angle_diff-2 );
	else if (angle_diff <= 120 && angle_diff > 90)
	    boat_speed = ( 0.0057*pow(wind_speed,3) - 0.1596*pow(wind_speed,2) + 1.5091*wind_speed  ) * ( -0.0333*angle_diff+4 ) +
	              ( 0.0062*pow(wind_speed,3) - 0.1684*pow(wind_speed,2) + 1.4957*wind_speed ) * ( 0.0333*angle_diff-3 );
	else if (angle_diff <= 150 && angle_diff > 120)
	    boat_speed = ( 0.0062*pow(wind_speed,3) - 0.1684*pow(wind_speed,2) + 1.4957*wind_speed ) * ( -0.0333*angle_diff+5 ) +
	              ( 0.0022*pow(wind_speed,3) - 0.0780*pow(wind_speed,2) + 0.9223*wind_speed ) * ( 0.0333*angle_diff-4 );
	else if (angle_diff <= 180 && angle_diff > 150)
	    boat_speed = ( 0.0022*pow(wind_speed,3) - 0.0780*pow(wind_speed,2) + 0.9223*wind_speed ) * ( -0.0333*angle_diff+6 ) +
	              ( 0.0030*pow(wind_speed,3) - 0.0871*pow(wind_speed,2) + 0.8549*wind_speed ) * ( 0.0333*angle_diff-5 );
	return (vec2){boat_speed * dir.x, boat_speed * dir.y};
}