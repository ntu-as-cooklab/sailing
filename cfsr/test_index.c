#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <eccodes.h>
#include "cfsr_grb2.h"

int main (void)
{
    cfsr_grb2_dataset_t* dataset = CFSR_OCNU5;
    struct tm date = {.tm_year=1979,.tm_mon=0,.tm_mday=1};

    char* filename = cfsr_grb2_filename(dataset, date);
    int err;
    codes_index* index = codes_index_new_from_file(NULL, filename, "testing2", &err);

    size_t valuesSize;
    codes_index_get_size(index,"testing2",&valuesSize);
    printf("valuesSize: %u\n", valuesSize);

    return 0;
}

