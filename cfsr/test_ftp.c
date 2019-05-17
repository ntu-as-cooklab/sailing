#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include "util_ftp.h"
#include "cfsr.h"

int main (void)
{
    curl_global_init(CURL_GLOBAL_DEFAULT);
    cfsr_fetch("ocnu5", 1979, 01);
    cfsr_fetch("ocnu5", 1979, 02);
    cfsr_fetch("ocnu5", 1979, 03);
    curl_global_cleanup();
    return 0;
}