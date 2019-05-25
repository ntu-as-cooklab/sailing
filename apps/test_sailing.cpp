#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cfsr/cfsr_nc.h"
#include "path.hpp"
#include "sailing.hpp"

int main (void)
{
    path_t path;
    path.pts.push_back((pathpt_t){{.tm_mday = 1,.tm_mon = 0,.tm_year = 79}, {22.495, 129.989}});

    for (int i = 0; i < 4800; i++)
        sail_step(path);

    return 0;
}