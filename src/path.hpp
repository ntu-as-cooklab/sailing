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
    std::vector<pathpt_t> pts;
    uint32_t user;
    uint32_t id;
    struct tm startdate;
    struct tm enddate;
    latlon_t startloc;
} path_t;

#endif