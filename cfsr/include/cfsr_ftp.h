#ifndef CFSR_FTP_H
#define CFSR_FTP_H

#ifdef  __cplusplus
extern  "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <curl/curl.h> // for downloading data

typedef struct FtpFile FtpFile;
struct FtpFile
{
	const char *filename;
	FILE *stream;
};

int getCFSR(char *filename);
static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream);

#ifdef  __cplusplus
}
#endif

#endif
