#ifndef CFSR_H
#define CFSR_H

#ifdef  __cplusplus
extern  "C" {
#endif

#include <time.h>

typedef struct cfsr_dataset_t cfsr_dataset_t;

int cfsr_fetch(cfsr_dataset_t* dataset, struct tm date);
int cfsr_load(cfsr_dataset_t* dataset, struct tm date);
char* cfsr_filename(cfsr_dataset_t* dataset, struct tm date);
double cfsr_value(cfsr_dataset_t* dataset, struct tm date, double lat, double lon);
double cfsr_idw(double* values, double* distances, size_t size);
void cfsr_free(cfsr_dataset_t* dataset);
double cfsr_bilinear(cfsr_dataset_t* dataset, struct tm date, double lat, double lon);

#define CFSR_START_YEAR 1979
#define CFSR_END_YEAR 2012

extern cfsr_dataset_t *CFSR_OCNU5, *CFSR_OCNV5;

#ifdef  __cplusplus
}
#endif

#endif
