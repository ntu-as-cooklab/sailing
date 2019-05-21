#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cfsr_nc.h"

int main (void)
{
    struct tm date = {.tm_year=1979,.tm_mon=0,.tm_mday=1};

    cfsr_nc_load(CFSR_NC_OCNU5, date);

    return 0;
}