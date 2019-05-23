#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cfsr_grb2.h"

int main (void)
{
    struct tm date = {.tm_year=1979,.tm_mon=0,.tm_mday=1};

    cfsr_grb2_load(CFSR_GRB2_OCNU5, date);
    cfsr_grb2_load(CFSR_GRB2_OCNV5, date);

    for (int i = 0; i < 48; i++) {
        double ou = cfsr_grb2_bilinear(CFSR_GRB2_OCNU5, date, 22.495, 129.989);
        printf("val: %f\n", ou);
        date.tm_hour++;
        mktime(&date);
    }

    return 0;
}