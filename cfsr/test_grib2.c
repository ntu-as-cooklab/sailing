#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cfsr_grb2.h"

int main (void)
{
    struct tm date = {.tm_year=1979,.tm_mon=0,.tm_mday=1};

    cfsr_grb2_load(CFSR_OCNU5, date);
    cfsr_grb2_load(CFSR_OCNV5, date);

    for (int i = 0; i < 48; i++) {
        double ou_bi = cfsr_grb2_bilinear(CFSR_OCNU5, date, 22.495, 129.989);
        //double ou = cfsr_grb2_value(CFSR_OCNU5, date, 22.495, 129.989);
        //double ov = cfsr_grb2_bilinear(CFSR_OCNV5, date, 25.8, 123.2);
        printf("val: %f\n", ou_bi);
        date.tm_hour++;
        mktime(&date);
    }

    return 0;
}