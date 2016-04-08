#include "cfsr.hpp"

// TODO: space and time interpolation

UV CfsrReader::OUV(Date date, LatLon pos)
{
	return UV (	getOUV(ouid[0] ?: ouid[0] = openCFSR(CFSR_OU, date.year, date.month), date.day, pos.lat(), pos.lon()),
				getOUV(ovid[0] ?: ovid[0] = openCFSR(CFSR_OV, date.year, date.month), date.day, pos.lat(), pos.lon())	);
}

UV CfsrReader::AUV(Date date, LatLon pos)
{
	return UV ( getAUV(auid[0] ?: auid[0] = openCFSR(CFSR_U, date.year, date.month), date.day, pos.lat(), pos.lon()),
				getAUV(avid[0] ?: avid[0] = openCFSR(CFSR_V, date.year, date.month), date.day, pos.lat(), pos.lon())	);
}
