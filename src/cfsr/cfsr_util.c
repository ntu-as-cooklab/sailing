#include <math.h>
#include "cfsr_util.h"

double mod(double x, double n)
{
    return fmod(fmod(x,n)+n, n);
}

double bilinear(double* v, double di, double dj)
{
    int nan_count = 0;
    for (int i = 0; i < 4; i++)
        if (v[i] == NAN)
            nan_count++;

    switch (nan_count)
    {
        case 0: return v[0]*(1-di)*(1-dj) + v[1]*(1-di)*dj + v[2]*di*(1-dj) + v[3]*di*dj;
        case 1:
            if (v[0] == NAN) return v[1]*(1-di) + v[2]*(1-dj) + v[3]*di*dj;
            if (v[1] == NAN) return v[0]*(1-di) + v[2]*di*(1-dj) + v[3]*dj;
            if (v[2] == NAN) return v[0]*(1-dj) + v[1]*(1-di)*dj + v[3]*di;
            if (v[3] == NAN) return v[0]*(1-di-dj) + v[1]*dj + v[2]*di;
        default: return NAN;
    }
}