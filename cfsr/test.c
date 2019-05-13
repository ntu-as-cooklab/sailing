#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "cfsr_ftp.h"

int main (void)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    getCFSR("ftp://nomads.ncdc.noaa.gov/CFSR/HP_time_series/197901/ocnu5.gdas.197901.grb2");
    curl_global_cleanup();
    return 0;
}