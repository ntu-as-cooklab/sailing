#include "util_ftp.h"
#include <stdint.h>
#include <string.h>
#include <curl/curl.h>

typedef struct FtpXfer
{
	curl_off_t lastruntime;
	CURL *curl;
	const char *filename;
	FILE *stream;
} FtpXfer;

static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *p)
{
	FtpXfer* xfer = (FtpXfer*)p;
	if(xfer && !xfer->stream)
	{
		xfer->stream = fopen(xfer->filename, "wb"); // open file for writing
		if(!xfer->stream)
			return -1; // failure, can't open file to write
	}
  	return fwrite(buffer, size, nmemb, xfer->stream);
}   

static int xferinfo(void *p, curl_off_t dltotal, curl_off_t dlnow, curl_off_t ultotal, curl_off_t ulnow)
{
  	FtpXfer* xfer = (FtpXfer*)p;
	curl_off_t curtime;
	curl_easy_getinfo(xfer->curl, CURLINFO_TOTAL_TIME_T, &curtime);
	
	if(dlnow > 0 && curtime - xfer->lastruntime >= 500000)
	{
		xfer->lastruntime = curtime;
		fprintf(stderr, "%s: %" CURL_FORMAT_CURL_OFF_T " of %" CURL_FORMAT_CURL_OFF_T "\n", 
			xfer->filename, dlnow, dltotal);
	}
	
  	return 0;
}

int ftp_getfile(char* url)
{
	CURLcode result;
	FtpXfer xfer = {.filename = strrchr(url, '/') + 1};

  	if ((xfer.curl = curl_easy_init()))
  	{
    	curl_easy_setopt(xfer.curl, CURLOPT_URL, url);
    	curl_easy_setopt(xfer.curl, CURLOPT_WRITEFUNCTION, my_fwrite); // Define our callback to get called when there's data to be written
    	curl_easy_setopt(xfer.curl, CURLOPT_WRITEDATA, &xfer); // Set a pointer to our struct to pass to the callback
    	curl_easy_setopt(xfer.curl, CURLOPT_VERBOSE, 0L); // Switch on full protocol/debug output

		curl_easy_setopt(xfer.curl, CURLOPT_XFERINFOFUNCTION, xferinfo);
		/* pass the struct pointer into the xferinfo function, note that this is an alias to CURLOPT_PROGRESSDATA */ 
		curl_easy_setopt(xfer.curl, CURLOPT_XFERINFODATA, &xfer);
    	curl_easy_setopt(xfer.curl, CURLOPT_NOPROGRESS, 0L);

    	result = curl_easy_perform(xfer.curl);

    	curl_easy_cleanup(xfer.curl); // always cleanup

    	if(result != CURLE_OK)
			return -1;
  	}

  	if(xfer.stream)
    	fclose(xfer.stream); // close the local file

  	return 0;
}

int __attribute__((constructor)) ftp_init(void)
{
	printf("Init CURL resources\n");
	curl_global_init(CURL_GLOBAL_ALL);	
}

void __attribute__((destructor)) ftp_free(void)
{
	printf("Freeing CURL resources\n");
	curl_global_cleanup();
}