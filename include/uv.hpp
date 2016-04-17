#ifndef UV_HPP
#define UV_HPP

#include "float2.hpp"
#include <iomanip> // for std::setprecision

struct UV : Float2<UV>
{
	using Float2::Float2;

	float heading()
	{
		float h = 90 - atan2(y, x) * 180/PI;
		return norm() > 0 ? (h < 0 ? h+360 : h) : 0;
	}

	friend std::ostream& operator<<(std::ostream& os, const UV& v)
	{ return os << std::fixed << std::setprecision(3) << v.x << ", " << v.y; }
};

#endif
