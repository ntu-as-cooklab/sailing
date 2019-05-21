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
    int err;
    char* filename = cfsr_nc_filename(dataset, date);
    if ((err = nc_open(filename, NC_NOWRITE, &dataset->ncid))) return err;

    int ndims, nvars, ngatts, unlimdimid;
    nc_inq(dataset->ncid, &ndims, &nvars, &ngatts, &unlimdimid);
    printf("ndims: %d nvars: %d ngatts: %d unlimdimid: %d\n", ndims, nvars, ngatts, unlimdimid);

    for (int i = 0; i < ndims; i++)
    {
        char name[NC_MAX_NAME];
        size_t length;
        nc_inq_dim(dataset->ncid, i, name, &length);
        printf("dim name: %s len: %d\n", name, length);
    }

    for (int i = 0; i < nvars; i++)
    {
        char name[NC_MAX_NAME];
        size_t length;
        nc_type xtype;
        int ndims;
        int dimids[NC_MAX_VAR_DIMS];
        int natts;
        nc_inq_var(dataset->ncid, i, name, &xtype, &ndims, dimids, &natts);
        printf("var name: %s ndims: %d\n", name, ndims);
    }

    for (int i = 0; i < ngatts; i++)
    {
        char name[NC_MAX_NAME];
        size_t len;
        nc_type xtype;
        nc_inq_att(dataset->ncid, NC_GLOBAL, name, &xtype, &len);
        printf("gatt name: %s len: %d\n", name, len);
    }

    return 0;
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

