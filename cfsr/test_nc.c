#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "cfsr_grb2.h"

int main (void)
{
    struct tm date = {.tm_year=1979,.tm_mon=0,.tm_mday=1};
    cfsr_grb2_load(CFSR_GRB2_OCNU5, date);
    cfsr_convert(CFSR_GRB2_OCNU5, date);

    return 0;
}