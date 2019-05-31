#ifndef CFSR_DATA_H
#define CFSR_DATA_H

#include <time.h>
#include "vec2.h"
#include "latlon.h"

#ifdef  __cplusplus
extern  "C" {
#endif

vec2 cfsr_ocn(struct tm date, latlon_t loc);
vec2 cfsr_wnd(struct tm date, latlon_t loc);

#ifdef  __cplusplus
}
#endif

#endif