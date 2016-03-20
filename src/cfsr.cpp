#include <cfsr.hpp>

UV getOUV(int ouid, int ovid, int day, LatLon pos)
{
	return UV
	(
		getOUV(ouid, day, pos.lat(), pos.lon()),
		getOUV(ovid, day, pos.lat(), pos.lon())
	);
}

UV getAUV(int auid, int avid, int day, LatLon pos)
{
	return UV
	(
		getAUV(auid, day, pos.lat(), pos.lon()),
		getAUV(avid, day, pos.lat(), pos.lon())
	);
}
