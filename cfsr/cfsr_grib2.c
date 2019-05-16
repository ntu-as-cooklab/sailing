#include "cfsr_grib2.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <eccodes.h>

int grib2_open (grib2_t* grib2, char* filename)
{
    FILE* in = fopen(filename, "r");
    if(!in) {
        printf("ERROR: unable to open file %s\n", filename);
        return -1;
    }
    printf("Opened file: %s\n", filename);

    int count;
    codes_count_in_file(0, in, &count);
    printf("count: %d\n", count);
    
    grib2->hh = malloc(count * sizeof(codes_handle*));
    for (int i = 0; i < count; i++) {
        int err;
        grib2->hh[i] = codes_grib_handle_new_from_file(0, in, &err);
        // printf("%d\n", i);
        // grib2_printkeys(grib2->hh[i], "ls");
    }
        
    fclose(in);
    return 0;
}

int grib2_printkeys(codes_handle* h, char* namespace)
{
    /* To skip read only and computed keys
     unsigned long key_iterator_filter_flags=CODES_KEYS_ITERATOR_SKIP_READ_ONLY |
                                             CODES_KEYS_ITERATOR_SKIP_COMPUTED;
     */
    unsigned long key_iterator_filter_flags = CODES_KEYS_ITERATOR_ALL_KEYS
                                              | CODES_KEYS_ITERATOR_SKIP_DUPLICATES;

    /* Choose a namespace. E.g. "ls", "time", "parameter", "geography", "statistics" */
    /* name_space=NULL to get all the keys */
    //const char* name_space = "time"; //"ls";

    codes_keys_iterator* kiter = codes_keys_iterator_new(h,key_iterator_filter_flags,namespace);
    if (!kiter) {
        printf("ERROR: Unable to create keys iterator\n");
        return -1;
    }

    #define MAX_VAL_LEN  65536
    char value[MAX_VAL_LEN];
    size_t vlen=MAX_VAL_LEN;

    while(codes_keys_iterator_next(kiter))
    {
        const char* name = codes_keys_iterator_get_name(kiter);
        vlen = MAX_VAL_LEN;
        bzero(value,vlen);
        CODES_CHECK(codes_get_string(h,name,value,&vlen),name);
        printf("%s = %s\n",name,value);

        /* Alternative way of getting the string value */
        //CODES_CHECK(codes_keys_iterator_get_string(kiter, value, &vlen),0);
    }

    codes_keys_iterator_delete(kiter);
    return 0;
}