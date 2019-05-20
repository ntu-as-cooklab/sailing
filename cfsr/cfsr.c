#include "cfsr.h"
#include <stdint.h>
#include <string.h>
#include "util_ftp.h"
#include <sys/stat.h>
#include <eccodes.h>
#include <math.h>

static const char* dataset_str[] = {
    "ocnu5",
    "ocnv5",
};

static codes_nearest* nearest[CFSR_DATASET_MAX];
static codes_handle* handle[CFSR_DATASET_MAX][CFSR_END_YEAR-CFSR_START_YEAR][31][24];

int cfsr_fetch(cfsr_dataset_t dataset, struct tm date)
{
    char* filename = cfsr_filename(dataset, date);
    char url[512];
    snprintf(url, sizeof(url), "ftp://nomads.ncdc.noaa.gov/CFSR/HP_time_series/%04u%02u/%s",
            date.tm_year, date.tm_mon+1, filename);
    return ftp_getfile(url);
}

int cfsr_load(cfsr_dataset_t dataset, struct tm date)
{
    char* filename = cfsr_filename(dataset, date);
    struct stat st;
    if (stat(filename, &st)) cfsr_fetch(dataset, date);
    FILE* in = fopen(filename, "r");
    if (!in) return -1;

    codes_handle* h;
    int err;
    while (h = codes_grib_handle_new_from_file(0, in, &err)) {
        char dataDate_str[9];
        char dataTime_str[5];
        size_t dataDate_len = sizeof(dataDate_str);
        size_t dataTime_len = sizeof(dataTime_str);
        codes_get_string(h, "dataDate", dataDate_str, &dataDate_len);
        codes_get_string(h, "dataTime", dataTime_str, &dataTime_len);
        long startStep;
        codes_get_long(h, "startStep", &startStep);
        int dataDate = atoi(dataDate_str);
        int dataTime = atoi(dataTime_str);
        date.tm_mday = dataDate%100;
        date.tm_hour = dataTime/100 + startStep;
        date.tm_min = dataTime%100;
        handle[dataset][date.tm_year-CFSR_START_YEAR][date.tm_mday-1][date.tm_hour] = h;
    }
    fclose(in);
    return 0;
}

char* cfsr_filename(cfsr_dataset_t dataset, struct tm date)
{
    static char filename[128];
    snprintf(filename, sizeof(filename), "%s.gdas.%04u%02u.grb2", dataset_str[dataset], date.tm_year, date.tm_mon+1);
    return filename;
}

codes_handle** cfsr_handle(cfsr_dataset_t dataset, struct tm date)
{
    return &handle[dataset][date.tm_year-CFSR_START_YEAR][date.tm_mday-1][date.tm_hour];
}

double cfsr_value(cfsr_dataset_t dataset, struct tm date, double lat, double lon)
{
    codes_handle* h = *cfsr_handle(dataset, date);
    if (!h) {
        cfsr_load(dataset, date);
        h = *cfsr_handle(dataset, date);
    }
    
    int err;
    if(!nearest[dataset]) nearest[dataset] = codes_grib_nearest_new(h, &err);
    static double lats[4]={0,};
    static double lons[4]={0,};
    static double values[4]={0,};
    static double distances[4]={0,};
    static int indexes[4]={0,};
    static size_t size=4;
    codes_grib_nearest_find(nearest[dataset], h, lat, lon, CODES_NEAREST_SAME_GRID, 
                            lats, lons, values, distances, indexes, &size);
    // printf("\nIdx\tlat\tlon\tdist\tval\n");
    // for (int i=0;i<4;i++) printf("%d\t%.2f\t%.2f\t%g\t%g\n",
    //         (int)indexes[i],lats[i],lons[i],distances[i],values[i]);
    // printf("\n");

    //codes_handle_delete(h);
    //codes_grib_nearest_delete(nearest);

    return cfsr_idw(values, distances, size);
}

double cfsr_idw(double* values, double* distances, size_t size)
{
    double weight[size]; 
    for (int i = 0; i < size; i++) weight[i] = 1./distances[i];
    double result = 0;
    for (int i = 0; i < size; i++) result += values[i] * weight[i];
    return result;
} 