#include <stdio.h>
#include <stdlib.h>
#include "cfsr_grib2.h"

int main (void)
{
    codes_handle* h = grib2_open("ocnu5.gdas.197901.grb2");
    grib2_getkeys(h);
    codes_handle_delete(h);
    return 0;
}