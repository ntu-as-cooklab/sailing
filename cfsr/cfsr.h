#ifndef CFSR_H
#define CFSR_H

#ifdef  __cplusplus
extern  "C" {
#endif

#include <netcdf.h>
#include <stdio.h>

// NetCDF error handling
extern int err;
#define ERR(e) { printf("NetCDF error: %s\n", nc_strerror(e)); return(1e4); }

// CFSR data types
typedef enum
{
	CFSR_OU, // Ocean U
	CFSR_OV, // Ocean V
	CFSR_U,  // Wind U
	CFSR_V   // Wind V
} CFSR;

extern const char* CFSR_dir;
extern const char* CFSR_filename[4];
extern const char* AVG_filename[4];

int openCFSR(CFSR type, int year, int month);
int closeCFSR(int ncid);

#ifdef  __cplusplus
}
#endif

#endif
