#ifndef CFSR_H
#define CFSR_H

#ifdef  __cplusplus
extern  "C" {
#endif

#include <stdint.h>
#include <time.h>

int cfsr_fetch(const char* dataset, uint16_t year, uint8_t month);
char* cfsr_filename(const char* dataset, struct tm date);
int cfsr_ou(struct tm date, double lat, double lon);

#define CFSR_START_YEAR 1979
#define CFSR_END_YEAR 2012

#ifdef  __cplusplus
}
#endif

#endif
