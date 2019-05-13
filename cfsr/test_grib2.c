#include <stdio.h>
#include <stdlib.h>
#include "cfsr_grib2.h"

int main (void)
{
    codes_handle* h = grib2_open("ocnu5.gdas.197901.grb2");
    printf("=========== ls ==========\n");
    grib2_printkeys(h, "ls");
    printf("=========== time ==========\n");
    grib2_printkeys(h, "time");
    printf("=========== parameter ==========\n");
    grib2_printkeys(h, "parameter");
    printf("=========== geography ==========\n");
    grib2_printkeys(h, "geography");
    printf("=========== statistics ==========\n");
    grib2_printkeys(h, "statistics");
    codes_handle_delete(h);
    return 0;
}