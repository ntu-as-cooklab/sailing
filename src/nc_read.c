#include <stdlib.h>
#include <stdio.h>
#include <netcdf.h>

/* Handle errors by printing an error message and exiting with a
 * non-zero status. */
#define ERRCODE 2
#define ERR(e) { printf("Error: %s\n", nc_strerror(e)); exit(ERRCODE); }

// Error handling.
int err;

void printNC()
{
	// This will be the netCDF ID for the file and data variable.
	int ncid;
	int ndims, nvars, natts, unlimdimid;
	int format;
	// Inquire the file
	if (err = nc_inq(ncid, &ndims, &nvars, &natts, &unlimdimid)) ERR(err);
	printf("ncid: %d\n\n", ncid);

	// Inquire the binary format of the netCDF file
	if (err = nc_inq_format(ncid, &format)) ERR(err);
	printf("format: %d\n\n", format);

	// Inquire the unlimited dimension
	printf("unlimdimid: %d\n\n", unlimdimid);
}

void printDims(int ncid, int ndims)
{
	// Dimensions
	printf("Number of dimensions: %d\n", ndims);
	char* dimname = malloc((NC_MAX_NAME+1)*sizeof(char));
	int dimid;
	for(dimid=0; dimid<ndims; dimid++)
	{
		if (err = nc_inq_dimname(ncid, dimid, dimname)) ERR(err);
		printf("[Dimension]\t%s\n", dimname);
	}
	printf("\n");
}

void printVars(int ncid, int nvars)
{
	// Variables
	printf("Number of variables: %d\n", nvars);
	char* varname = malloc((NC_MAX_NAME+1)*sizeof(char));
	int varid;
	for(varid=0; varid<nvars; varid++)
	{
		if (err = nc_inq_varname(ncid, varid, varname)) ERR(err);
		printf("[Variable]\t%s\n", varname);
	}
	printf("\n");
}

void printGlobalAtts(int ncid, int natts)
{
	// Attributes
	printf("Number of attributes: %d\n", natts);
	char* attname = malloc((NC_MAX_NAME+1)*sizeof(char));
	int attid;
	for(attid=0; attid<natts; attid++)
	{
		if (err = nc_inq_attname(ncid, NC_GLOBAL, attid, attname)) ERR(err);
		printf("[Attribute]\t%s\n", attname);
	}
	printf("\n");
}

float getU(int ncid, int day, float lat, float lon)
{
	float y = (89.75-lat)*2, x = (lon-0.25)*2;
	size_t dim[4] = { day-1, 0, y, x };
	float u;
	nc_get_var1_float(ncid, 4, dim, &u);
	return u;
	// TODO: 2d interpolation, e.g. Barnes
}

int main()
{
	/* This is the name of the data file we will read. */
	char* FILE_NAME = "cfsr/CFSR_ocnh06.gdas.OU_197901_5.nc";

    // This will be the netCDF ID for the file and data variable.
    int ou_ncid, ov_ncid;

    // Open the file. NC_NOWRITE tells netCDF we want read-only access to the file.
    if (err = nc_open(FILE_NAME, NC_NOWRITE, &ou_ncid)) ERR(err);
    //printf("\n*** Reading file %s\n\n", FILE_NAME);

	printf("%f\n", getU(ou_ncid, 29, 7.75, 104.249));


	char input;
	scanf ("%c", &input);

    // Close the file, freeing all resources.
    if (err = nc_close(ou_ncid)) ERR(err);
    //printf("File closed.\n\n");

   return 0;
}
