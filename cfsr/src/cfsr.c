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

inline float rawOUV(int ncid, int day, int i, int j)
{
	size_t dim[4] = { day-1, 0, i, j };
	float v;
	if ((nc_get_var1_float(ncid, 4, dim, &v))) ERR(err);
	return v;
}

inline float rawAUV(int ncid, int day, int i, int j)
{
	size_t dim[3] = { day-1, i, j };
	float v;
	if ((nc_get_var1_float(ncid, 0, dim, &v))) ERR(err);
	return v;
}

inline float bilinearDayOUV(int ncid, int day, float i, float j)
{
	int i0 = floorf(i), i1 = ceilf(i),
		j0 = floorf(j), j1 = ceilf(j);

	return 	(i-i0) * (j-j0) * rawOUV(ncid, day, i0, j0) +
			(i-i0) * (j1-j) * rawOUV(ncid, day, i0, j1) +
			(i1-i) * (j-j0) * rawOUV(ncid, day, i1, j0) +
			(i1-i) * (j1-j) * rawOUV(ncid, day, i1, j1);
}

inline float bilinearDayAUV(int ncid, int day, float i, float j)
{
	int i0 = floorf(i), i1 = ceilf(i),
		j0 = floorf(j), j1 = ceilf(j);

		printf("%d\n", i1-i0);
	return 	(i-i0) * (j-j0) * rawAUV(ncid, day, i0, j0) +
			(i-i0) * (j1-j) * rawAUV(ncid, day, i0, j1) +
			(i1-i) * (j-j0) * rawAUV(ncid, day, i1, j0) +
			(i1-i) * (j1-j) * rawAUV(ncid, day, i1, j1);
}

float bilinearOUV(int ncid, float day, float lat, float lon)
{
	float i = (89.75-lat)/0.5, j = ((lon<0.25?lon+360:lon)-0.25)/0.5; // TODO: fix range hack
	int d0 = floorf(day), d1 = ceilf(day);
	return bilinearDayOUV(ncid, d0, i, j) * d0 + bilinearDayOUV(ncid, d1, i, j) * d1;
}

float bilinearAUV(int ncid, float day, float lat, float lon)
{
	float i = (89.761-lat)/0.31249958, j = (lon<0?lon+360:lon)/0.31249958; // TODO: fix range hack
	int d0 = floorf(day), d1 = ceilf(day);
	return bilinearDayAUV(ncid, d0, i, j) * d0 + bilinearDayAUV(ncid, d1, i, j) * d1;
}
