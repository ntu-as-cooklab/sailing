#include "cfsr.h"
#include <stdint.h>
#include <string.h>
#include "util_ftp.h"
#include <sys/stat.h>
#include <eccodes.h>

int cfsr_fetch(const char* dataset, uint16_t year, uint8_t month)
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
    printf("Extracting file: %s\n", filename);

    codes_handle* h;
    int err;
    while (h = codes_grib_handle_new_from_file(0, in, &err)) {
        char dataDate_str[16];
        char dataTime_str[16];
        size_t dataDate_len = sizeof(dataDate_str);
        size_t dataTime_len = sizeof(dataTime_str);
        codes_get_string(h, "dataDate", dataDate_str, &dataDate_len);
        codes_get_string(h, "dataTime", dataTime_str, &dataTime_len);
        int dataDate = atoi(dataDate_str);
        int dataTime = atoi(dataTime_str);
        long startStep;
        codes_get_long(h, "startStep", &startStep);
        dataTime += startStep * 100;
        //printf("dataDate: %08d\n", dataDate);
        //printf("dataTime: %04d\n", dataTime);

        char out_filename[128];
        snprintf(out_filename, sizeof(out_filename), "%s.gdas.%04u%02u/%08d%04d.grb2", 
                    dataset, year, month, dataDate, dataTime);
        codes_write_message(h, out_filename, "wb");
    }

    codes_handle_delete(h);   
    fclose(in);
    return 0;
}

char* cfsr_filename(const char* dataset, struct tm date)
{
    static char filename[128];
    snprintf(filename, sizeof(filename), "%s.gdas.%04u%02u/%04u%02u%02u%02u%02u.grb2", 
                dataset, date.tm_year, date.tm_mon+1, 
                date.tm_year, date.tm_mon+1, date.tm_mday, date.tm_hour, date.tm_min);
    return filename;
}

codes_nearest* nearest;

int cfsr_ou(struct tm date, double lat, double lon)
{
    char* dataset = "ocnu5";
    char* filename = cfsr_filename(dataset, date);
    struct stat st;
    if (stat(filename, &st)) {
        printf("cfsr_fetch %s\n", filename);
        cfsr_fetch(dataset, date.tm_year, date.tm_mon+1);
    }

    int err;
    FILE* in = fopen(filename, "r");
    if (!in) return -1;
    printf("Opened file: %s\n", filename);
    codes_handle* h = codes_grib_handle_new_from_file(0, in, &err);
    fclose(in);

    if(!nearest) nearest = codes_grib_nearest_new(h, &err);
    static double lats[4]={0,};
    static double lons[4]={0,};
    static double values[4]={0,};
    static double distances[4]={0,};
    static int indexes[4]={0,};
    static size_t size=4;
    codes_grib_nearest_find(nearest, h, lat, lon, CODES_NEAREST_SAME_GRID, 
                            lats, lons, values, distances, indexes, &size);
    printf("\nIdx\tlat\tlon\tdist\tval\n");
    for (int i=0;i<4;i++) printf("%d\t%.2f\t%.2f\t%g\t%g\n",
            (int)indexes[i],lats[i],lons[i],distances[i],values[i]);
    printf("\n");

    codes_handle_delete(h);
    //codes_grib_nearest_delete(nearest);

    return 0;
}