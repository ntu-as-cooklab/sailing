#ifndef CFSR_FTP_H
#define CFSR_FTP_H

#ifdef  __cplusplus
extern  "C" {
#endif

#include <stdint.h>

int ftp_getfile(char *filename);
int cfsr_getfile(char* dataset, uint16_t year, uint8_t month);

#ifdef  __cplusplus
}
#endif

#endif
