#include "cfsr_data.h"
#include "cfsr_nc.h"
#include "vec2.h"
#include "latlon.h"
#include <time.h>

vec2 cfsr_ocn(struct tm date, latlon_t loc)
{
    double ou = cfsr_nc_bilinear(CFSR_NC_OCNU5, date, loc);
    double ov = cfsr_nc_bilinear(CFSR_NC_OCNV5, date, loc);
    return (vec2){ou, ov};
}