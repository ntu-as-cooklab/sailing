#ifndef CFSR_GRIB2_H
#define CFSR_GRIB2_H

#include <eccodes.h>

codes_handle* grib2_open (char* filename);
int grib2_printkeys(codes_handle* h, char* namespace);

#endif