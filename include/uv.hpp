#ifndef UV_HPP
#define UV_HPP

#include <float2.hpp>

struct UV : Float2<UV>
{
	using Float2::Float2;

	float heading()
	{
		if (x > 0)
		    return -atan(y/x)*180/PI+90;
		else if (x < 0)
		    return -atan(y/x)*180/PI+270;
		else if (x == 0 && y >= 0)
		    return 0;
		else if (x == 0 && y < 0)
		    return 180;
	}
};

#endif
