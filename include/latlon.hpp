#ifndef LATLON_HPP
#define LATLON_HPP

#include "float2.hpp"
#include <iomanip> // for std::setprecision

struct LatLon : Float2<LatLon>
{
	using Float2::Float2;
	float lat() { return x; }
	float lon() { return y; }

	friend std::ostream& operator<<(std::ostream& os, const LatLon& v)
		{ return os << std::fixed << std::setprecision(6) << v.x << ", " << v.y; }
};

#endif
