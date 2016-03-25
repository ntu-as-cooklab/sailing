/*typedef struct FtpFile FtpFile;
struct FtpFile
{
	const char *filename;
	FILE *stream;
};

static size_t my_fwrite(void *buffer, size_t size, size_t nmemb, void *stream)
{
	struct FtpFile *out = (struct FtpFile *) stream;
	if(out && !out->stream)
	{
		out->stream = fopen(out->filename, "wb"); // open file for writing
		if(! out->stream)
			return -1; // failure, can't open file to write
	}
  	return fwrite(buffer, size, nmemb, out->stream);
}

int getCFSR(char *filename)
{
	CURL *curl;
	CURLcode result;
	FtpFile ftpfile = { filename, NULL };

  	curl_global_init(CURL_GLOBAL_DEFAULT);

  	if (curl = curl_easy_init())
  	{
		char url[80] = ;
		strcat(url, filename);
    	curl_easy_setopt(curl, CURLOPT_URL, url);
    	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, my_fwrite); // Define our callback to get called when there's data to be written
    	curl_easy_setopt(curl, CURLOPT_WRITEDATA, &ftpfile); // Set a pointer to our struct to pass to the callback
    	curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L); // Switch on full protocol/debug output

    	result = curl_easy_perform(curl);

    	curl_easy_cleanup(curl); // always cleanup

    	if(result != CURLE_OK)
      		fprintf("[Curl] %d\n", res); // we failed
  	}

  	if(ftpfile.stream)
    	fclose(ftpfile.stream); // close the local file

  	curl_global_cleanup();

  	return 0;
}*/
