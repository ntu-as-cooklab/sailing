#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cfsr_grib2.h"
#include "cfsr.h"

int main (void)
{
    struct tm date = {.tm_year=1979,.tm_mon=0,.tm_mday=1};

    for (int i = 0; i < 30; i++) {
        double val = cfsr_value(CFSR_OCNU5, date, 25.8, 123.2);
        printf("val: %f\n", val);
        date.tm_mday++;
    }
    
    // grib2_t grib2;
    // grib2_open(&grib2, "ocnu5.gdas.197901.grb2");
    // grib2_open(&grib2, "ocnu5.gdas.197902.grb2");
    // grib2_open(&grib2, "ocnu5.gdas.197903.grb2");
    // while(1);
    // grib2_free(&grib2);

    return 0;
}