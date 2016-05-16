#include <cfsr.h>
#include <string.h>
#include <math.h>
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>

int copy_file(char *new_filename, char *old_filename)
{
	FILE *ptr_old, *ptr_new;
	errno_t err = 0, err1 = 0;
	int  a;

	err = fopen_s(&ptr_old, old_filename, "rb");
	err1 = fopen_s(&ptr_new, new_filename, "wb");

	if(err) return -1;

	if(err1)
	{
		fclose(ptr_old);
		return -1;
	}

	while(1)
	{
		a = fgetc(ptr_old);
		if(!feof(ptr_old)) fputc(a, ptr_new);
		else break;
	}

	fclose(ptr_new);
	fclose(ptr_old);
	return  0;
}

int convertCFSR(CFSR type, int month)
{
	int sourceid, destid; // This will be the netCDF ID for the file and data variable.
	char sourcepath[50], destpath[50];
	strcpy(sourcepath, CFSR_dir);
	strcat(sourcepath, CFSR_filename[type]); // append CFSR filename to filepath
	snprintf(strchr(sourcepath,'Y'), 6, "%04d%02d", 1979, month);

	printf("[CFSR] Opening file: %s", sourcepath);
	if ((err = nc_open(sourcepath, NC_NOWRITE, &sourceid))) ERR(err); // Open the file. NC_NOWRITE tells netCDF we want read-only access to the file.
	printf("\t...ok\n");

	strcpy(destpath, CFSR_dir);
	strcat(destpath, CFSR_filename[type]); // append CFSR filename to filepath
	snprintf(strchr(destpath,'Y'), 6, "%04d%02d", 9999, month);

	copy_file(destpath, sourcepath);

	printf("[CFSR] Writing file: %s", destpath);
	if ((err = nc_open(destpath, NC_WRITE, &destid))) ERR(err); // Open the file. NC_NOWRITE tells netCDF we want read-only access to the file.
	printf("\t...ok\n");

	char avgpath0[50], avgpath[50];
	strcpy(avgpath0, CFSR_dir);
	strcat(avgpath0, AVG_filename[type]); // append CFSR filename to filepath

	FILE* fp;
    char line[8192];
	int day = 0;

    while (	strcpy(avgpath, avgpath0),
			snprintf(strchr(avgpath,'9'), 4, "%02d%02d", month, day++),
			fp = fopen(avgpath, "r")
	)
	{
		int row = 0;
		while ((fgets(line, 8192, fp)))
		{
			int col = 0;
			char *pch;
			pch = strtok(line, ",");
			do
			{
				//printf ("%s, ", pch);
				float var = atof(pch);
				int retval;
				if (type == CFSR_OU || type == CFSR_OV)
				{
					size_t dim[4] = { day-1, 0, col, row };
					//printf ("%d, %d\n", col, row);
					if ((retval = nc_put_var1_float(destid, 4, dim, &var))) ERR(retval);
				}
				if (type == CFSR_U || type == CFSR_V)
				{
					size_t dim[3] = { day-1, col, row };
					//printf ("%d, %d\n", col, row);
					if ((retval = nc_put_var1_float(destid, 0, dim, &var))) ERR(retval);
				}
				col++;
			}
			while ((pch = strtok(NULL, ",")));
			//printf ("%d\n", col);
			row++;
		}
		fclose(fp);
		//printf ("%d\n", row);
	}
	closeCFSR(sourceid);
	closeCFSR(destid);

	return 0;
}
