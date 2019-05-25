#include "cfsr/cfsr_nc.h"
#include "cfsr/cfsr.h"
#include "latlon.h"
#include <netcdf.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>

typedef struct cfsr_nc_dataset_t
{
    int ncid[CFSR_END_YEAR-CFSR_START_YEAR][12];
    char* str;
    long Ni;
    long Nj;
    double lon0;
    double lat0;
    double lon1;
    double lat1;
    double dx;
    double dy;
} cfsr_nc_dataset_t;

cfsr_nc_dataset_t cfsr_nc_ocnu5 = {.str = "ocnu5"},
                  cfsr_nc_ocnv5 = {.str = "ocnv5"};
cfsr_nc_dataset_t *CFSR_NC_OCNU5 = &cfsr_nc_ocnu5;
cfsr_nc_dataset_t *CFSR_NC_OCNV5 = &cfsr_nc_ocnv5;

int cfsr_nc_load(cfsr_nc_dataset_t* dataset, struct tm date)
{
    int err;
    char* filename = cfsr_nc_filename(dataset, date);
    if ((err = nc_open(filename, NC_NOWRITE, &dataset->ncid[1900+date.tm_year-CFSR_START_YEAR][date.tm_mon]))) {
        printf("cfsr_nc_load: failed to load file %s\n", filename);
        return err;
    }
    printf("cfsr_nc_load: loaded %s\n", filename);

    dataset->Ni = 720;
    dataset->Nj = 360;
    dataset->lat0 = 89.750000;
    dataset->lon0 = 0.250000;
    dataset->lat1 = -89.750000;
    dataset->lon1 = 359.750000;
    dataset->dy = -0.500000;
    dataset->dx = 0.500000;

    return 0;
}

char* cfsr_nc_filename(cfsr_nc_dataset_t* dataset, struct tm date)
{
    static char filename[128];
    snprintf(filename, sizeof(filename), "data/%s.gdas.%04u%02u.nc", dataset->str, 1900+date.tm_year, date.tm_mon+1);
    return filename;
}

int cfsr_nc_free(cfsr_nc_dataset_t* dataset)
{
    for (int i = 0; i < CFSR_END_YEAR-CFSR_START_YEAR; i++)
        for (int j = 0; j < 12; j++)
    nc_close(dataset->ncid[i][j]);
}

double cfsr_nc_bilinear(cfsr_nc_dataset_t* dataset, struct tm date, latlon_t loc)
{
    int ncid = dataset->ncid[1900+date.tm_year-CFSR_START_YEAR][date.tm_mon];
    if (!ncid) {
        cfsr_nc_load(dataset, date);
        ncid = dataset->ncid[1900+date.tm_year-CFSR_START_YEAR][date.tm_mon];
    }
    double i = mod((loc.lon-dataset->lon0)/dataset->dx, dataset->Ni);
    double j = mod((loc.lat-dataset->lat0)/dataset->dy, dataset->Nj);
    double i0 = floor(i);
    double j0 = floor(j);
    double i1 = mod(i0+1, dataset->Ni);
    double j1 = mod(j0+1, dataset->Nj);
    double di = i - i0;
    double dj = j - j0;

    size_t dim[4][5] = {
        { date.tm_mday-1, date.tm_hour/6, date.tm_hour%6, j0, i0 },
        { date.tm_mday-1, date.tm_hour/6, date.tm_hour%6, j1, i0 },
        { date.tm_mday-1, date.tm_hour/6, date.tm_hour%6, j0, i1 },
        { date.tm_mday-1, date.tm_hour/6, date.tm_hour%6, j1, i1 },
    };
    int16_t s[4];
    double v[4];
    for (int i = 0; i < 4; i++) {
        nc_get_var1_short(ncid, 5, dim[i], &s[i]);
        v[i] = 0.03947173179924627 + s[i] * 5.6536401507637375E-5;
    }

    return v[0]*(1-di)*(1-dj) + v[1]*(1-di)*dj + v[2]*di*(1-dj) + v[3]*di*dj;
}

