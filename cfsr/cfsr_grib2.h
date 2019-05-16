#ifndef CFSR_GRIB2_H
#define CFSR_GRIB2_H

#include <eccodes.h>

typedef struct grib2_t
{
    int count;
    codes_handle** hh;
} grib2_t;

int grib2_open (grib2_t* grib2, char* filename);
int grib2_printkeys(codes_handle* h, char* namespace);
void grib2_free(grib2_t* grib2);

#endif