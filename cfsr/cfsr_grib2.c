#include "cfsr_grib2.h"
#include <stdio.h>
#include <stdlib.h>
#include <eccodes.h>

codes_handle* grib2_open (char* filename)
{
    FILE* in = fopen(filename, "r");
    if(!in) {
        printf("ERROR: unable to open file %s\n", filename);
        return NULL;
    }

    int err;
    codes_handle* h = codes_handle_new_from_file(0, in, PRODUCT_GRIB, &err);
    if (h == NULL) {
        printf("Error: unable to create handle from file %s\n", filename);
        return NULL;
    }

    printf("Opened file: %s\n", filename);
    fclose(in);
    return h;
}

int grib2_getkeys(codes_handle* h)
{
    /* To skip read only and computed keys
     unsigned long key_iterator_filter_flags=CODES_KEYS_ITERATOR_SKIP_READ_ONLY |
                                             CODES_KEYS_ITERATOR_SKIP_COMPUTED;
     */
    unsigned long key_iterator_filter_flags = CODES_KEYS_ITERATOR_ALL_KEYS |
                                              CODES_KEYS_ITERATOR_SKIP_DUPLICATES;
 
    /* Choose a namespace. E.g. "ls", "time", "parameter", "geography", "statistics" */
    const char* name_space = NULL; //"ls";
 
    /* name_space=NULL to get all the keys */
    /* char* name_space=0; */

    codes_keys_iterator* kiter = codes_keys_iterator_new(h,key_iterator_filter_flags,name_space);
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