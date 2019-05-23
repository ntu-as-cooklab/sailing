#include "cfsr_data.h"
#include "cfsr_nc.h"
#include "vec2.h"
#include <time.h>

vec2 cfsr_ocn(struct tm date, double lat, double lon)
{
    double ou = cfsr_nc_bilinear(CFSR_NC_OCNU5, date, lat, lon);
    double ov = cfsr_nc_bilinear(CFSR_NC_OCNV5, date, lat, lon);
    return (vec2){ou, ov};
}