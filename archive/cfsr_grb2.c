#include "cfsr_grb2.h"
#include "cfsr.h"
#include <stdint.h>
#include <string.h>
#include "util_ftp.h"
#include <sys/stat.h>
#include <eccodes.h>
#include <math.h>

#include "cfsr_nc.h"
#include <netcdf.h>

typedef struct cfsr_grb2_dataset_t
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
} cfsr_grb2_dataset_t;

cfsr_grb2_dataset_t cfsr_grb2_ocnu5 = {.str = "ocnu5"},
                    cfsr_grb2_ocnv5 = {.str = "ocnv5"};
cfsr_grb2_dataset_t *CFSR_GRB2_OCNU5 = &cfsr_grb2_ocnu5;
cfsr_grb2_dataset_t *CFSR_GRB2_OCNV5 = &cfsr_grb2_ocnv5;

int cfsr_grb2_fetch(cfsr_grb2_dataset_t* dataset, struct tm date)
{
    char* filename = cfsr_grb2_filename(dataset, date);
    char url[512];
    snprintf(url, sizeof(url), "ftp://nomads.ncdc.noaa.gov/CFSR/HP_time_series/%04u%02u/%s",
            date.tm_year, date.tm_mon+1, filename);
    return ftp_getfile(url);
}

int cfsr_grb2_load(cfsr_grb2_dataset_t* dataset, struct tm date)
{
    char* filename = cfsr_grb2_filename(dataset, date);
    struct stat st;
    if (stat(filename, &st)) cfsr_grb2_fetch(dataset, date);
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

            printf("Ni: %d\n", dataset->Ni);
            printf("Nj: %d\n", dataset->Nj);
            printf("lat0: %f\n", dataset->lat0);
            printf("lon0: %f\n", dataset->lon0);
            printf("lat1: %f\n", dataset->lat1);
            printf("lon1: %f\n", dataset->lon1);
            printf("dy: %f\n", dataset->dy);
            printf("dx: %f\n", dataset->dx);
        }

        long dataDate, dataTime, startStep;
        codes_get_long(h, "dataDate", &dataDate);
        codes_get_long(h, "dataTime", &dataTime);
        codes_get_long(h, "startStep", &startStep);
        date.tm_mday = dataDate%100;
        date.tm_hour = dataTime/100 + startStep;
        date.tm_min = dataTime%100;
        dataset->handle[date.tm_year-CFSR_START_YEAR][date.tm_mday-1][date.tm_hour] = h;
         printf("date.tm_mday-1: %d date.tm_hour: %d\n", date.tm_mday-1, date.tm_hour);
    }

    fclose(in);
    return 0;
}

char* cfsr_grb2_filename(cfsr_grb2_dataset_t* dataset, struct tm date)
{
    static char filename[128];
    snprintf(filename, sizeof(filename), "%s.gdas.%04u%02u.grb2", dataset->str, date.tm_year, date.tm_mon+1);
    return filename;
}

static codes_handle* cfsr_grb2_handle(cfsr_grb2_dataset_t* dataset, struct tm date)
{
    return dataset->handle[date.tm_year-CFSR_START_YEAR][date.tm_mday-1][date.tm_hour];
}

double cfsr_grb2_value(cfsr_grb2_dataset_t* dataset, struct tm date, double lat, double lon)
{
    codes_handle* h = cfsr_grb2_handle(dataset, date);
    if (!h) {
        cfsr_grb2_load(dataset, date);
        h = cfsr_grb2_handle(dataset, date);
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
    return cfsr_grb2_idw(values, distances, size);
}

double cfsr_grb2_idw(double* values, double* distances, size_t size)
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

void cfsr_grb2_free(cfsr_grb2_dataset_t* dataset)
{
    codes_grib_nearest_delete(dataset->nearest);

    for (int j = 0; j < CFSR_END_YEAR-CFSR_START_YEAR; j++)
        for (int k = 0; k < 31; k++)
            for (int l = 0; l < 24; l++)
                codes_handle_delete(dataset->handle[j][k][l]);
}

int cfsr_grb2_ij2n(cfsr_grb2_dataset_t* dataset, int i, int j)
{
    return j * dataset->Ni + i;
}

double cfsr_grb2_bilinear(cfsr_grb2_dataset_t* dataset, struct tm date, double lat, double lon)
{
    double i = mod((lon-dataset->lon0)/dataset->dx, dataset->Ni);
    double j = mod((lat-dataset->lat0)/dataset->dy, dataset->Nj);
    double i0 = floor(i);
    double j0 = floor(j);
    double i1 = mod(i0+1, dataset->Ni);
    double j1 = mod(j0+1, dataset->Nj);
    double di = i - i0;
    double dj = j - j0;
    int n[4] = {
        cfsr_grb2_ij2n(dataset, i0, j0),
        cfsr_grb2_ij2n(dataset, i0, j1),
        cfsr_grb2_ij2n(dataset, i1, j0),
        cfsr_grb2_ij2n(dataset, i1, j1),
    };
    
    double v[4];
    codes_get_double_elements(cfsr_grb2_handle(dataset, date), "values", n, 4, v);
    return v[0]*(1-di)*(1-dj) + v[1]*(1-di)*dj + v[2]*di*(1-dj) + v[3]*di*dj;
}

int cfsr_convert(cfsr_grb2_dataset_t* dataset, struct tm date)
{
    char filename[128];
    snprintf(filename, sizeof(filename), "%s.gdas.%04u%02u.nc", dataset->str, date.tm_year, date.tm_mon+1);

    int ncid;
    nc_create(filename, NC_NETCDF4, &ncid);

    int latid, lonid, dateid, timeid;
    nc_def_dim(ncid, "latitude",  dataset->Nj, &latid);
    nc_def_dim(ncid, "longitude", dataset->Ni, &lonid);
    nc_def_dim(ncid, "date", 31, &dateid);
    nc_def_dim(ncid, "time", 24, &timeid);

    int varid;
    int dims[] = {dateid, timeid, lonid, latid};
    nc_def_var(ncid, dataset->str, NC_DOUBLE, 4, dims, &varid);

    for (int d = 0; d < 28; d++)
        for (int h = 0; h < 24; h++)
        {
            size_t start[4] = {d, h, 0, 0};
            size_t count[4] = {1, 1, dataset->Ni, dataset->Nj};
            double data[dataset->Ni*dataset->Nj];
            size_t data_size = dataset->Ni*dataset->Nj;
            codes_handle* handle = dataset->handle[date.tm_year-CFSR_START_YEAR][d][h];
            if(!handle) { printf("err\n"); return -1; }
            codes_get_double_array(
                dataset->handle[date.tm_year-CFSR_START_YEAR][d][h],
                "values",
                data,
                &data_size
            );
            nc_put_vara_double(ncid, varid, start, count, data);
            printf("d: %d h: %d\n", d, h);
        }
    
    nc_enddef(ncid);

    nc_close(ncid);
}