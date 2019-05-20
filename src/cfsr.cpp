#include "cfsr.hpp"

UV OUV(struct tm date, LatLon pos)
{
	return UV (	bilinearOUV(ouid[date.month] ?: ouid[date.month] = openCFSR(CFSR_OU, date.year, date.month), date.day+date.hour/24.0, pos.lat(), pos.lon()),
				bilinearOUV(ovid[date.month] ?: ovid[date.month] = openCFSR(CFSR_OV, date.year, date.month), date.day+date.hour/24.0, pos.lat(), pos.lon())	);
}

UV AUV(struct tm, LatLon pos)
{
	return UV ( bilinearAUV(auid[date.month] ?: auid[date.month] = openCFSR(CFSR_U, date.year, date.month), date.day+date.hour/24.0, pos.lat(), pos.lon()),
				bilinearAUV(avid[date.month] ?: avid[date.month] = openCFSR(CFSR_V, date.year, date.month), date.day+date.hour/24.0, pos.lat(), pos.lon())	);
}
