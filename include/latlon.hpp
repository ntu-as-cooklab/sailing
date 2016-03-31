#ifndef LATLON_HPP
#define LATLON_HPP

#include <float2.hpp>

struct LatLon : Float2<LatLon>
{
	LatLon() {}
	LatLon(float lat, float lon) { x = lon; y = lat; }
	float lat() { return y; }
	float lon() { return x; }
};

#endif
