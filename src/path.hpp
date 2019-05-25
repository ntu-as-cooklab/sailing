#ifndef PATH_HPP
#define PATH_HPP

#include <time.h>
#include "latlon.h"
#include <vector>

typedef struct pathpt_t
{
    struct tm date;
    latlon_t loc; 
} pathpt_t;

typedef struct path_t
{
    std::vector<pathpt_t> pts;
} path_t;

#endif