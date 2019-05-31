#include "cfsr/cfsr_data.h"
#include "cfsr/cfsr_nc.h"
#include "vec2.h"
#include "latlon.h"
#include <time.h>

vec2 cfsr_ocn(struct tm date, latlon_t loc)
{
    double u = cfsr_nc_bilinear(CFSR_NC_OCNU5, date, loc);
    double v = cfsr_nc_bilinear(CFSR_NC_OCNV5, date, loc);
    //printf("ocn: %f %f\n", u, v);
    return (vec2){u, v};
}

vec2 cfsr_wnd(struct tm date, latlon_t loc)
{
    double u = cfsr_nc_bilinear(CFSR_NC_WNDU10, date, loc);
    double v = cfsr_nc_bilinear(CFSR_NC_WNDV10, date, loc);
    return (vec2){u, v};
}