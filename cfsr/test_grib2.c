#include <stdio.h>
#include <stdlib.h>
#include "cfsr_grib2.h"

int main (void)
{
    grib2_t grib2;
    grib2_open(&grib2, "ocnu5.gdas.197901.grb2");
    grib2_free(&grib2);
    return 0;
}