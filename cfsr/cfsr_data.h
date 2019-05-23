#ifndef CFSR_DATA_H
#define CFSR_DATA_H

#include <time.h>
#include "vec2.h"

#ifdef  __cplusplus
extern  "C" {
#endif

vec2 cfsr_ocn(struct tm date, double lat, double lon);

#ifdef  __cplusplus
}
#endif

#endif