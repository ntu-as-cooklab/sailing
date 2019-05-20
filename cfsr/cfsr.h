#ifndef CFSR_H
#define CFSR_H

#ifdef  __cplusplus
extern  "C" {
#endif

#include <time.h>

typedef enum cfsr_dataset_t
{
    CFSR_OCNU5,
    CFSR_OCNV5,
    CFSR_DATASET_MAX,
} cfsr_dataset_t;

int cfsr_fetch(cfsr_dataset_t dataset, struct tm date);
char* cfsr_filename(cfsr_dataset_t dataset, struct tm date);
int cfsr_value(cfsr_dataset_t dataset, struct tm date, double lat, double lon);

#define CFSR_START_YEAR 1979
#define CFSR_END_YEAR 2012

#ifdef  __cplusplus
}
#endif

#endif
