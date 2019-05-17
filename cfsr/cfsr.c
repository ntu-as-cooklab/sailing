#include "cfsr.h"
#include <stdint.h>
#include <string.h>
#include "util_ftp.h"
#include <sys/stat.h>
#include <eccodes.h>

int cfsr_fetch(char* dataset, uint16_t year, uint8_t month)
{
	char url[512];
    char filename[128];
    snprintf(filename, sizeof(filename), "%s.gdas.%04u%02u.grb2", dataset, year, month);

    struct stat st;
    if (stat(filename, &st)) {
        snprintf(url, sizeof(url), "ftp://nomads.ncdc.noaa.gov/CFSR/HP_time_series/%04u%02u/%s", year, month, filename);
	    if (ftp_getfile(url) != 0) return -1;
    }

    char basename[128];
    snprintf(basename, sizeof(basename), "%s.gdas.%04u%02u", dataset, year, month);
    mkdir(basename, 0755);

    FILE* in = fopen(filename, "r");
    if (!in) return -1;
    printf("Opened file: %s\n", filename);

    codes_handle* h;
    int err;
    int count = 0;
    while (h = codes_grib_handle_new_from_file(0, in, &err)) {
        char out_filename[128];
        snprintf(out_filename, sizeof(out_filename), "%s.gdas.%04u%02u/%d.grb2", dataset, year, month, count);
        codes_write_message(h, out_filename, "wb");
        printf("count: %d\n", ++count);
    }

    codes_handle_delete(h);   
    fclose(in);
    return 0;
}