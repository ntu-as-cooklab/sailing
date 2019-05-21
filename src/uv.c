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

// 計算該緯度的每一緯度應該是多少距離(km)
double lat2km(double lon)
{ 
	return 110.59 + 4e-12*lon + 0.0003*pow(lon,2) + 3e-17*pow(lon,3) - 2e-8*pow(lon,4);
}

// 計算該緯度的每一經度應該是多少距離(km)
double lon2km(double lat)
{ 
	return 111.28 + 1e-12*lat - 0.0168*pow(lat,2) - 2e-17*pow(lat,3) + 4e-7*pow(lat,4);
} 