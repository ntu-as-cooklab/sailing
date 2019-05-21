#ifndef UV_HPP
#define UV_HPP

#include <math.h> 

double heading(vec2 v)
{
	double h = 90 - atan2(v.y, v.x) * 180/PI;
	return norm(v) > 0 ? (h < 0 ? h+360 : h) : 0;
}

#endif
