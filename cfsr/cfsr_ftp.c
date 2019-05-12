#include "cfsr_ftp.h"
#include <string.h>

typedef struct FtpXfer
{
	const char *filename;
	FILE *stream;
} FtpXfer;

static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *xfer)
{
	struct FtpXfer *out = (struct FtpXfer *) xfer;
	if(out && !out->stream)
	{
		out->stream = fopen(out->filename, "wb"); // open file for writing
		if(!out->stream)
			return -1; // failure, can't open file to write
	}
  	return fwrite(buffer, size, nmemb, out->stream);
}   

struct myprogress {
  curl_off_t lastruntime; /* type depends on version, see above */ 
  CURL *curl;
};

/* this is how the CURLOPT_XFERINFOFUNCTION callback works */ 
static int xferinfo(void *p,
                    curl_off_t dltotal, curl_off_t dlnow,
                    curl_off_t ultotal, curl_off_t ulnow)
{
  	struct myprogress *myp = (struct myprogress *)p;
	CURL *curl = myp->curl;
	curl_off_t curtime = 0;
	
	curl_easy_getinfo(curl, CURLINFO_TOTAL_TIME_T, &curtime);
	
	/* under certain circumstances it may be desirable for certain functionality
		to only run every N seconds, in order to do this the transaction time can
		be used */ 
	if(dlnow > 0 && curtime - myp->lastruntime >= 500000)
	{
		myp->lastruntime = curtime;
		fprintf(stderr, "DOWN: %" CURL_FORMAT_CURL_OFF_T " of %" CURL_FORMAT_CURL_OFF_T "\n",
        dlnow, dltotal);
	}
	
  	return 0;
}

struct myprogress prog;

int getCFSR(char* url)
{
	CURL *curl;
	CURLcode result;
	char* filename = strrchr(url, '/') + 1;
	FtpXfer ftpfile = { filename, NULL };

  	curl_global_init(CURL_GLOBAL_DEFAULT);

  	if ((curl = curl_easy_init()))
  	{
		prog.lastruntime = 0;
    	prog.curl = curl;

    	curl_easy_setopt(curl, CURLOPT_URL, url);
    	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite); // Define our callback to get called when there's data to be written
    	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftpfile); // Set a pointer to our struct to pass to the callback
    	curl_easy_setopt(curl, CURLOPT_VERBOSE, 0L); // Switch on full protocol/debug output

		curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, xferinfo);
		/* pass the struct pointer into the xferinfo function, note that this is an alias to CURLOPT_PROGRESSDATA */ 
		curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &prog);
    	curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);

    	result = curl_easy_perform(curl);

    	curl_easy_cleanup(curl); // always cleanup

    	if(result != CURLE_OK)
			return -1;
  	}

  	if(ftpfile.stream)
    	fclose(ftpfile.stream); // close the local file

  	curl_global_cleanup();

  	return 0;
}
