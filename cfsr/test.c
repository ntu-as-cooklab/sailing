#include <stdio.h>
#include <stdlib.h>
#include "cfsr_ftp.h"

int main (void)
{
    getCFSR("ftp://nomads.ncdc.noaa.gov/CFSR/HP_time_series/197901/ocnu5.gdas.197901.grb2");
    return 0;
}