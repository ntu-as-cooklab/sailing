#ifndef CFSR_GRB2_H
#define CFSR_GRB2_H

#ifdef  __cplusplus
extern  "C" {
#endif

#include <time.h>

typedef struct cfsr_grb2_dataset_t cfsr_grb2_dataset_t;

int cfsr_grb2_fetch(cfsr_grb2_dataset_t* dataset, struct tm date);
int cfsr_grb2_load(cfsr_grb2_dataset_t* dataset, struct tm date);
char* cfsr_grb2_filename(cfsr_grb2_dataset_t* dataset, struct tm date);
double cfsr_grb2_value(cfsr_grb2_dataset_t* dataset, struct tm date, double lat, double lon);
double cfsr_grb2_idw(double* values, double* distances, size_t size);
void cfsr_grb2_free(cfsr_grb2_dataset_t* dataset);
double cfsr_grb2_bilinear(cfsr_grb2_dataset_t* dataset, struct tm date, double lat, double lon);

#define CFSR_START_YEAR 1979
#define CFSR_END_YEAR 2012

extern cfsr_grb2_dataset_t *CFSR_OCNU5, *CFSR_OCNV5;

#ifdef  __cplusplus
}
#endif

#endif
