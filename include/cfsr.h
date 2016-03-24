#ifndef CFSR_H
#define CFSR_H

#ifdef  __cplusplus
extern  "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <netcdf.h>
#include <curl/curl.h> // for downloading data

// NetCDF error handling
extern int err;
#define ERRCODE 2
#define ERR(e) { printf("NetCDF error: %s\n", nc_strerror(e)); exit(ERRCODE); }

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

// CFSR functions
int openCFSR(CFSR type, int year, int month);
void closeCFSR(int ncid);
float getOUV(int ncid, int day, float lat, float lon);
float getAUV(int ncid, int day, float lat, float lon);

#ifdef  __cplusplus
}
#endif

#endif
