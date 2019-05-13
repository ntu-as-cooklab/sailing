#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "cfsr_ftp.h"

int main (void)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    cfsr_getfile("ocnu5", 1979, 01);
    curl_global_cleanup();
    return 0;
}