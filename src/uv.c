#include "uv.h"
#include "vec2.h"
#define _USE_MATH_DEFINES
#include <math.h>

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