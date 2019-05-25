#ifndef CFSR_H
#define CFSR_H

#define CFSR_START_YEAR 1979
#define CFSR_END_YEAR 2012

#include <math.h>

static inline double mod(double x, double n)
{
    return fmod(fmod(x,n)+n, n);
}

#endif