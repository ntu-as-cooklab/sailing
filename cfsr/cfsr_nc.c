#include "cfsr_nc.h"
#include "cfsr.h"
#include <netcdf.h>
#include <string.h>
#include <stdio.h>

typedef struct cfsr_nc_dataset_t
{
    int ncid;
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
    char* filename = cfsr_nc_filename(dataset, date);
    return nc_open(filename, NC_NOWRITE, &dataset->ncid);
}

char* cfsr_nc_filename(cfsr_nc_dataset_t* dataset, struct tm date)
{
    static char filename[128];
    snprintf(filename, sizeof(filename), "%s.gdas.%04u%02u.nc", dataset->str, date.tm_year, date.tm_mon+1);
    return filename;
}

int cfsr_nc_free(cfsr_nc_dataset_t* dataset)
{
    nc_close(dataset->ncid);
}

double cfsr_nc_bilinear(cfsr_nc_dataset_t* dataset, struct tm date, double lat, double lon)
{

}

