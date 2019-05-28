#ifndef PATH_HPP
#define PATH_HPP

#include <time.h>
#include "latlon.h"
#include <vector>
#include <stdint.h>

typedef struct pathpt_t
{
    struct tm date;
    latlon_t loc;
} pathpt_t;

typedef struct path_t
{
    uint32_t id;
    uint32_t user;
    struct tm startdate;
    struct tm enddate;
    latlon_t startloc;
    std::vector<pathpt_t> pts;
} path_t;

#endif