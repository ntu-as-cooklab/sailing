#ifndef UV_HPP
#define UV_HPP

#include "float2.hpp"

struct UV : Float2<UV>
{
	using Float2::Float2;

	float heading()
	{
		float h = 90 - atan2(y, x) * 180/PI;
		return h < 0 ? h+360 : h;
	}
};

#endif
