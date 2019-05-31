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
    int varid;
    char* str;
    long Ni;
    long Nj;
    float lon0;
    float lat0;
    double dx;
    double dy;
    double scale_factor;
    double add_offset;
    uint16_t missing_value;
} cfsr_nc_dataset_t;

cfsr_nc_dataset_t cfsr_nc_ocnu5 = {.str = "ocnu5"},
                  cfsr_nc_ocnv5 = {.str = "ocnv5"},
                  cfsr_nc_wndu10 = {.str = "wnd10m.l"},
                  cfsr_nc_wndv10 = {.str = "wnd10m.l"};
cfsr_nc_dataset_t *CFSR_NC_OCNU5 = &cfsr_nc_ocnu5;
cfsr_nc_dataset_t *CFSR_NC_OCNV5 = &cfsr_nc_ocnv5;
cfsr_nc_dataset_t *CFSR_NC_WNDU10 = &cfsr_nc_wndu10;
cfsr_nc_dataset_t *CFSR_NC_WNDV10 = &cfsr_nc_wndv10;

int* cfsr_ncid(cfsr_nc_dataset_t* dataset, struct tm date)
{
    return &dataset->ncid[1900+date.tm_year-CFSR_START_YEAR][date.tm_mon];
}

int cfsr_nc_open(cfsr_nc_dataset_t* dataset, struct tm date)
{
    const char* dirs[] = {"data", "/data"};
    char* filename;
    for (int i = 0; i < sizeof(dirs)/sizeof(char*); i++) {
        filename = cfsr_nc_filename(dirs[i], dataset, date);
        if (nc_open(filename, NC_NOWRITE, cfsr_ncid(dataset, date)) == 0)
            return 0;
    }
    printf("cfsr_nc_load: failed to load file %s\n", filename);
    return -1;
}

int cfsr_nc_load(cfsr_nc_dataset_t* dataset, struct tm date)
{
    if (cfsr_nc_open(dataset, date) < 0) return -1;
    int ncid = *cfsr_ncid(dataset, date);

    // int ndims, nvars, natts, unlimdimid;
    // nc_inq(ncid, &ndims, &nvars, &natts, &unlimdimid);
    // for (int i = 0; i < nvars; i++) {
    //     char name[NC_MAX_NAME+1];
    //     nc_inq_varname(ncid, i, name);
    //     printf("name: %s\n", name);
    // }

    if (dataset == CFSR_NC_OCNU5 || dataset == CFSR_NC_OCNV5 || dataset == CFSR_NC_WNDU10)
        dataset->varid = 5;
    else if (dataset == CFSR_NC_WNDV10)
        dataset->varid = 6;

    nc_get_att_double(ncid, dataset->varid, "scale_factor", &dataset->scale_factor);
    nc_get_att_double(ncid, dataset->varid, "add_offset", &dataset->add_offset);
    nc_get_att_short(ncid, dataset->varid, "missing_value", &dataset->missing_value);
    nc_inq_dimlen(ncid, CFSR_LON_ID, &dataset->Ni); // longitude
    nc_inq_dimlen(ncid, CFSR_LAT_ID, &dataset->Nj); // latitude

    size_t dim = 0;
    nc_get_var1_float(ncid, CFSR_LON_ID, &dim, &dataset->lon0);
    nc_get_var1_float(ncid, CFSR_LAT_ID, &dim, &dataset->lat0);

    dim = 1;
    float lat1, lon1;
    nc_get_var1_float(ncid, CFSR_LON_ID, &dim, &lon1);
    nc_get_var1_float(ncid, CFSR_LAT_ID, &dim, &lat1);
    dataset->dx = lon1 - dataset->lon0;
    dataset->dy = lat1 - dataset->lat0;

    return 0;
}

char* cfsr_nc_filename(const char* root, cfsr_nc_dataset_t* dataset, struct tm date)
{
    static char filename[128];
    snprintf(filename, sizeof(filename), "%s/%s.gdas.%04u%02u.nc", root, dataset->str, 1900+date.tm_year, date.tm_mon+1);
    return filename;
}

int cfsr_nc_free(cfsr_nc_dataset_t* dataset)
{
    for (int i = 0; i < CFSR_END_YEAR-CFSR_START_YEAR; i++)
        for (int j = 0; j < 12; j++)
    nc_close(dataset->ncid[i][j]);
}

static double bilinear(double* v, double di, double dj)
{
    int nan_count = 0;
    for (int i = 0; i < 4; i++)
        if (v[i] == NAN)
            nan_count++;

    switch (nan_count)
    {
        case 0: return v[0]*(1-di)*(1-dj) + v[1]*(1-di)*dj + v[2]*di*(1-dj) + v[3]*di*dj;
        case 1:
            if (v[0] == NAN) return v[1]*(1-di) + v[2]*(1-dj) + v[3]*di*dj;
            if (v[1] == NAN) return v[0]*(1-di) + v[2]*di*(1-dj) + v[3]*dj;
            if (v[2] == NAN) return v[0]*(1-dj) + v[1]*(1-di)*dj + v[3]*di;
            if (v[3] == NAN) return v[0]*(1-di-dj) + v[1]*dj + v[2]*di;
        default: return NAN;
    }
}

double cfsr_nc_bilinear(cfsr_nc_dataset_t* dataset, struct tm date, latlon_t loc)
{
    int ncid = *cfsr_ncid(dataset, date);
    if (!ncid) {
        if (cfsr_nc_load(dataset, date) < 0) return NAN;
        ncid = *cfsr_ncid(dataset, date);
    }
    double i = mod((loc.lon-dataset->lon0)/dataset->dx, dataset->Ni);
    double j = mod((loc.lat-dataset->lat0)/dataset->dy, dataset->Nj);
    double i0 = floor(i);
    double j0 = floor(j);
    double i1 = mod(i0+1, dataset->Ni);
    double j1 = mod(j0+1, dataset->Nj);

    size_t dim[4][5] = {
        { date.tm_mday-1, date.tm_hour/6, date.tm_hour%6, j0, i0 },
        { date.tm_mday-1, date.tm_hour/6, date.tm_hour%6, j1, i0 },
        { date.tm_mday-1, date.tm_hour/6, date.tm_hour%6, j0, i1 },
        { date.tm_mday-1, date.tm_hour/6, date.tm_hour%6, j1, i1 },
    };
    double v[4];
    for (int p = 0; p < 4; p++) {
        int16_t s;
        nc_get_var1_short(ncid, dataset->varid, dim[p], &s);
        v[p] =  (s == dataset->missing_value) ? NAN : 
                (dataset->add_offset + s * dataset->scale_factor);
    }

    return bilinear(v, i - i0, j - j0);
}

