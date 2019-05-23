#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cfsr_nc.h"

int main (void)
{
    struct tm date = {.tm_year=1979,.tm_mon=0,.tm_mday=1};

    for (int i = 0; i < 48; i++) {
        double ou = cfsr_nc_bilinear(CFSR_NC_OCNU5, date, 22.495, 129.989);
        printf("val: %f\n", ou);
        date.tm_hour++;
        mktime(&date);
    }

    return 0;
}