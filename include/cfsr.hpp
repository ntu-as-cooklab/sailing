#ifndef CFSR_HPP
#define CFSR_HPP

#include <cfsr.h>
#include <latlon.hpp>
#include <uv.hpp>

UV getOUV(int ouid, int ovid, int day, LatLon pos);
UV getAUV(int auid, int avid, int day, LatLon pos);

#endif
