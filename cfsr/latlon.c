#include "latlon.h"
#include <math.h>

// 計算該緯度的每一緯度應該是多少距離(km)
double lat2km(double lat)
{ 
	return 110.59 + 4e-12*lat + 0.0003*pow(lat,2) + 3e-17*pow(lat,3) - 2e-8*pow(lat,4);
}

// 計算該緯度的每一經度應該是多少距離(km)
double lon2km(double lat)
{ 
	return 111.28 + 1e-12*lat - 0.0168*pow(lat,2) - 2e-17*pow(lat,3) + 4e-7*pow(lat,4);
}