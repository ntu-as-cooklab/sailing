#include <cfsr.h>
#include <string.h>
#include <math.h>

int err;
const char* CFSR_dir = "data/"; // predefined CFSR directory
const char* CFSR_filename[] = // predefined CFSR filenames
{
	"CFSR_ocnh06.gdas.OU_YYYYMM_5.nc",
	"CFSR_ocnh06.gdas.OV_YYYYMM_5.nc",
	"CFSR_flxf06.gdas.U_10m_YYYYMM.nc",
	"CFSR_flxf06.gdas.V_10m_YYYYMM.nc"
};

int openCFSR(CFSR type, int year, int month)
{
	int ncid; // This will be the netCDF ID for the file and data variable.
	char filepath[50];
	strcpy(filepath, CFSR_dir);
	strcat(filepath, CFSR_filename[type]); // append CFSR filename to filepath
	snprintf(strchr(filepath,'Y'), 6, "%04d%02d", year, month);
	// TODO: fetchCFSR(filename);
	printf("[CFSR] Opening file: %s", filepath);
	if ((err = nc_open(filepath, NC_NOWRITE, &ncid))) ERR(err); // Open the file. NC_NOWRITE tells netCDF we want read-only access to the file.
	printf("\t...ok\n");
	return ncid;
}

int closeCFSR(int ncid)
{
	if ((err = nc_close(ncid))) ERR(err); // Close the file, freeing all resources.
	//printf("\n[CFSR] File closed.\n");
	return 0;
}

float getOUV(int ncid, int day, float lat, float lon)
{
	printf("%f %f\n", lat, lon);
	float y = (89.75-lat)/0.5, x = ((lon<0.25?lon+360:lon) -0.25)/0.5; // TODO: fix range hack
	printf("%f %f\n", y, x);
	int j = (int) y, i = (int) x;
	//printf("%d %d\n", j, i);
	// TODO: 2d interpolation, e.g. Barnes
	size_t dim[4] = { day-1, 0, j, i };
	float v;
	if ((nc_get_var1_float(ncid, 4, dim, &v))) ERR(err);;
	return v;
}

float getAUV(int ncid, int day, float lat, float lon)
{
	float y = (89.761-lat)/0.31249958, x = (lon<0?lon+360:lon)/0.31249958; // TODO: fix range hack
	int j = (int) y, i = (int) x;
	//printf("%d %d\n", j, i);
	// TODO: 2d interpolation, e.g. Barnes
	size_t dim[3] = { day-1, j, i };
	float v;
	if ((err = nc_get_var1_float(ncid, 0, dim, &v))) ERR(err);
	return v;
}
