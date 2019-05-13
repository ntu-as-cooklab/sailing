#include "cfsr.h"
#include <string.h>

int err;
const char* CFSR_dir = "data/"; // predefined CFSR directory
const char* CFSR_filename[] = // predefined CFSR filenames
{
	"ocea/OU/CFSR_ocnh06.gdas.OU_YYYYMM_5.nc", 
	"ocea/OV/CFSR_ocnh06.gdas.OV_YYYYMM_5.nc",
	"atmo/U10/CFSR_flxf06.gdas.U_10m_YYYYMM.nc",
	"atmo/V10/CFSR_flxf06.gdas.V_10m_YYYYMM.nc"
};
const char* AVG_filename[] = // predefined CFSR filenames
{
	"OU_DailyM/9999oc.csv",
	"OV_DailyM/9999oc.csv",
	"U10_DailyM/9999.csv",
	"V10_DailyM/9999.csv"
};

int openCFSR(CFSR type, int year, int month)
{
	int ncid; // This will be the netCDF ID for the file and data variable.
	char filepath[80];
        char yyyymm[10];
	strcpy(filepath, CFSR_dir);
	strcat(filepath, CFSR_filename[type]); // append CFSR filename to filepath
	snprintf(yyyymm, 7, "%04d%02d", year, month);
	strncpy(strchr(filepath,'Y'), yyyymm, 6);
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
