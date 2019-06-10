#ifndef PATH_HPP
#define PATH_HPP

#include <time.h>
#include "latlon.h"
#include <vector>
#include <stdint.h>
#include <stdbool.h>
#include "model.hpp"
#include "vec2.h"
#include <string>

typedef struct pathpt_t
{
    struct tm date;
    latlon_t loc;
    bool sailopen;
    vec2 ocean;
    vec2 wind;
    vec2 wind_gain;
    vec2 gain;
} pathpt_t;

typedef struct path_t
{
    /* Identification */
    uint32_t id;
    uint32_t runId;
    std::string loginID;
    std::string color;

    /* Basic parameters */
    DATASET dataset;
    MODE mode;
    struct tm startdate;
    struct tm enddate;
    latlon_t startloc;

    /* Sailing parameters */
    latlon_t destloc;
    double  destheading;
    float 	altitude 		= 2;
	float 	windlimit 		= 8;
	int 	sailstarthour 	= 6;
    int 	sailendhour 	= 18;
	float 	alpha 			= 0.11; // parameter for wind profile power law
    
    /* Generated data */
    std::vector<pathpt_t> pts;
    bool land_collision;
} path_t;

#endif