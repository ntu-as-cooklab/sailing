#ifndef CFSR_NC_H
#define CFSR_NC_H

#include <time.h>
#include "latlon.h"

#ifdef  __cplusplus
extern  "C" {
#endif

#define CFSR_LON_ID 0
#define CFSR_LAT_ID 1

typedef struct cfsr_nc_dataset_t cfsr_nc_dataset_t;

int* cfsr_ncid(cfsr_nc_dataset_t* dataset, struct tm date);
int cfsr_nc_open(cfsr_nc_dataset_t* dataset, struct tm date);
int cfsr_nc_load(cfsr_nc_dataset_t* dataset, struct tm date);
char* cfsr_nc_filename(const char* root, cfsr_nc_dataset_t* dataset, struct tm date);
int cfsr_nc_free(cfsr_nc_dataset_t* dataset);
double cfsr_nc_bilinear(cfsr_nc_dataset_t* dataset, struct tm date, latlon_t loc);

extern cfsr_nc_dataset_t *CFSR_NC_OCNU5, *CFSR_NC_OCNV5, *CFSR_NC_WNDU10, *CFSR_NC_WNDV10;

#ifdef  __cplusplus
}
#endif

#endif