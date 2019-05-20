#include "cfsr.h"
#include <stdint.h>
#include <string.h>
#include "util_ftp.h"
#include <sys/stat.h>
#include <eccodes.h>
#include <math.h>

typedef struct cfsr_dataset_t
{
    codes_nearest* nearest;
    codes_handle* handle[CFSR_END_YEAR-CFSR_START_YEAR][31][24];
    char* str;
    long Ni;
    long Nj;
    double lon0;
    double lat0;
    double dx;
    double dy;
} cfsr_dataset_t;

cfsr_dataset_t  cfsr_ocnu5 = {.str = "ocnu5"},
                cfsr_ocnv5 = {.str = "ocnv5"};
cfsr_dataset_t *CFSR_OCNU5 = &cfsr_ocnu5;
cfsr_dataset_t *CFSR_OCNV5 = &cfsr_ocnv5;

int cfsr_fetch(cfsr_dataset_t* dataset, struct tm date)
{
    char* filename = cfsr_filename(dataset, date);
    char url[512];
    snprintf(url, sizeof(url), "ftp://nomads.ncdc.noaa.gov/CFSR/HP_time_series/%04u%02u/%s",
            date.tm_year, date.tm_mon+1, filename);
    return ftp_getfile(url);
}

int cfsr_load(cfsr_dataset_t* dataset, struct tm date)
{
    char* filename = cfsr_filename(dataset, date);
    struct stat st;
    if (stat(filename, &st)) cfsr_fetch(dataset, date);
    FILE* in = fopen(filename, "r");
    if (!in) return -1;

    codes_handle* h;
    int err;
    while (h = codes_grib_handle_new_from_file(0, in, &err)) {

        codes_get_long(h,"Ni",&dataset->Ni);
        codes_get_long(h,"Nj",&dataset->Nj);
        codes_get_double(h,"latitudeOfFirstGridPointInDegrees",&dataset->lat0);
        codes_get_double(h,"longitudeOfFirstGridPointInDegrees",&dataset->lon0);
        codes_get_double(h,"jDirectionIncrementInDegrees",&dataset->dy);
        codes_get_double(h,"iDirectionIncrementInDegrees",&dataset->dx);

        long dataDate, dataTime, startStep;
        codes_get_long(h, "dataDate", &dataDate);
        codes_get_long(h, "dataTime", &dataTime);
        codes_get_long(h, "startStep", &startStep);
        date.tm_mday = dataDate%100;
        date.tm_hour = dataTime/100 + startStep;
        date.tm_min = dataTime%100;
        dataset->handle[date.tm_year-CFSR_START_YEAR][date.tm_mday-1][date.tm_hour] = h;
    }
    fclose(in);
    return 0;
}

char* cfsr_filename(cfsr_dataset_t* dataset, struct tm date)
{
    static char filename[128];
    snprintf(filename, sizeof(filename), "%s.gdas.%04u%02u.grb2", dataset->str, date.tm_year, date.tm_mon+1);
    return filename;
}

static codes_handle* cfsr_handle(cfsr_dataset_t* dataset, struct tm date)
{
    return dataset->handle[date.tm_year-CFSR_START_YEAR][date.tm_mday-1][date.tm_hour];
}

double cfsr_value(cfsr_dataset_t* dataset, struct tm date, double lat, double lon)
{
    codes_handle* h = cfsr_handle(dataset, date);
    if (!h) {
        cfsr_load(dataset, date);
        h = cfsr_handle(dataset, date);
    }
    
    int err;
    if(!dataset->nearest) dataset->nearest = codes_grib_nearest_new(h, &err);
    static double lats[4]={0,};
    static double lons[4]={0,};
    static double values[4]={0,};
    static double distances[4]={0,};
    static int indexes[4]={0,};
    static size_t size=4;
    codes_grib_nearest_find(dataset->nearest, h, lat, lon, CODES_NEAREST_SAME_GRID, 
                            lats, lons, values, distances, indexes, &size);
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

void cfsr_free(cfsr_dataset_t* dataset)
{
    codes_grib_nearest_delete(dataset->nearest);

    for (int j = 0; j < CFSR_END_YEAR-CFSR_START_YEAR; j++)
        for (int k = 0; k < 31; k++)
            for (int l = 0; l < 24; l++)
                codes_handle_delete(dataset->handle[j][k][l]);
}

int cfsr_bilinear(cfsr_dataset_t* dataset)
{
}