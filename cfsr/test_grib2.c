#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cfsr.h"

int main (void)
{
    struct tm date = {.tm_year=1979,.tm_mon=0,.tm_mday=1};

    cfsr_load(CFSR_OCNU5, date);
    cfsr_load(CFSR_OCNV5, date);

    for (int i = 0; i < 3; i++) {
        double ou = cfsr_value(CFSR_OCNU5, date, 25.8, 123.2);
        double ov = cfsr_value(CFSR_OCNV5, date, 25.8, 123.2);
        printf("val: %f %f\n", ou, ov);
        date.tm_hour++;
        mktime(&date);
    }

    return 0;
}