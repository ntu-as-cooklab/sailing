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
    double lon1;
    double lat1;
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

        if (!dataset->Ni) {
            codes_get_long(h,"Ni",&dataset->Ni);
            codes_get_long(h,"Nj",&dataset->Nj);
            codes_get_double(h,"latitudeOfFirstGridPointInDegrees",&dataset->lat0);
            codes_get_double(h,"longitudeOfFirstGridPointInDegrees",&dataset->lon0);
            codes_get_double(h,"latitudeOfLastGridPointInDegrees",&dataset->lat1);
            codes_get_double(h,"longitudeOfLastGridPointInDegrees",&dataset->lon1);
            codes_get_double(h,"jDirectionIncrementInDegrees",&dataset->dy);
            codes_get_double(h,"iDirectionIncrementInDegrees",&dataset->dx);
            if (dataset->lon1 - dataset->lon0 < 0) dataset->dx *= -1;
            if (dataset->lat1 - dataset->lat0 < 0) dataset->dy *= -1;
        }

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
    //for (int i = 0; i < 4; i++) printf("idx: %d\n", indexes[i]);
    return cfsr_idw(values, distances, size);
}

double cfsr_idw(double* values, double* distances, size_t size)
{
    double weight_sum = 0;
    double result = 0;
    for (int i = 0; i < size; i++) {
        double weight = 1./distances[i];
        result += values[i] * weight;
        weight_sum += weight;
    }
    result /= weight_sum;
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

int cfsr_ij2n(cfsr_dataset_t* dataset, int i, int j)
{
    return j * dataset->Ni + i;
}

double mod(double x, double n)
{
    return fmod(fmod(x,n)+n, n);
}

double cfsr_bilinear(cfsr_dataset_t* dataset, struct tm date, double lat, double lon)
{
    double i = mod((lon-dataset->lon0)/dataset->dx, dataset->Ni);
    double j = mod((lat-dataset->lat0)/dataset->dy, dataset->Nj);
    double i0 = floor(i);
    double j0 = floor(j);
    double i1 = mod(i0+1, dataset->Ni);
    double j1 = mod(j0+1, dataset->Nj);
    int n00 = cfsr_ij2n(dataset, i0, j0);
    int n01 = cfsr_ij2n(dataset, i0, j1);
    int n10 = cfsr_ij2n(dataset, i1, j0);
    int n11 = cfsr_ij2n(dataset, i1, j1);
    double di = i - i0;
    double dj = j - j0;
    
    double v00, v01, v10, v11;
    codes_handle* h = cfsr_handle(dataset, date);
    codes_get_double_element(h, "values", n00, &v00);
    codes_get_double_element(h, "values", n01, &v01);
    codes_get_double_element(h, "values", n10, &v10);
    codes_get_double_element(h, "values", n11, &v11);
    // printf("lon0: %f lat0: %f lon1: %f lat1: %f\n", dataset->lon0, dataset->lat0, dataset->lon1, dataset->lat1);
    // printf("i: %f j: %f\n", i, j);
    // printf("i0: %f j0: %f i1: %f j1: %f\n", i0, j0, i1, j1);
    // printf("n00: %d n01: %d n10: %d n11: %d\n", n00, n01, n10, n11);
    // printf("v00: %f v01: %f v10: %f v11: %f\n", v00, v01, v10, v11);

    return v00*(1-di)*(1-dj) + v01*(1-di)*dj + v10*di*(1-dj) + v11*di*dj;
}