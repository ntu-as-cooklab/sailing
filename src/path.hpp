#ifndef PATH_HPP
#define PATH_HPP

#include <time.h>
#include "latlon.h"
#include <vector>
#include <stdint.h>
#include <stdbool.h>
#include "model.hpp"
#include "vec2.h"

typedef struct pathpt_t
{
    struct tm date;
    latlon_t loc;
    bool sailopen;
    vec2 ocean;
    vec2 wind;
    vec2 wind_gain;
} pathpt_t;

typedef struct path_t
{
    uint32_t id;
    uint32_t user;
    struct tm startdate;
    struct tm enddate;
    latlon_t startloc;
    bool land_collision;
    DATASET dataset;
    MODE mode;
    std::vector<pathpt_t> pts;

    /* Parameters */
	float 	altitude 		= 2;
	float 	windlimit 		= 8;
	int 	sailopenhours 	= 12;
	float 	alpha 			= 0.11;				// parameter for wind profile power law
} path_t;

#endif