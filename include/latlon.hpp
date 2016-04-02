#ifndef LATLON_HPP
#define LATLON_HPP

#include "float2.hpp"

struct LatLon : Float2<LatLon>
{
	using Float2::Float2;
	float lat() { return x; }
	float lon() { return y; }
};

#endif
