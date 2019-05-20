#ifndef CFSR_HPP
#define CFSR_HPP

#include "latlon.hpp"
#include "uv.hpp"
#include <time.h>

UV OUV(struct tm date, LatLon pos);
UV AUV(struct tm date, LatLon pos);

#endif
