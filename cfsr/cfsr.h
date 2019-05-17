#ifndef CFSR_H
#define CFSR_H

#ifdef  __cplusplus
extern  "C" {
#endif

#include <stdint.h>

int cfsr_fetch(char* dataset, uint16_t year, uint8_t month);

#ifdef  __cplusplus
}
#endif

#endif
