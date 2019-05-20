#include "cfsr.h"
#include <stdint.h>
#include <string.h>
#include "util_ftp.h"
#include <sys/stat.h>
#include <eccodes.h>
#include <math.h>

static const char* dataset_str[] = {
    "ocnu5",
    "ocnv5",
};

int cfsr_fetch(cfsr_dataset_t dataset, struct tm date)
{
	char url[512];
    char filename[128];
    snprintf(filename, sizeof(filename), "%s.gdas.%04u%02u.grb2", dataset_str[dataset], date.tm_year, date.tm_mon+1);

    struct stat st;
    if (stat(filename, &st)) {
        snprintf(url, sizeof(url), "ftp://nomads.ncdc.noaa.gov/CFSR/HP_time_series/%04u%02u/%s",
                date.tm_year, date.tm_mon+1, filename);
	    if (ftp_getfile(url) != 0) return -1;
    }

    char basename[128];
    snprintf(basename, sizeof(basename), "%s.gdas.%04u%02u", dataset_str[dataset], date.tm_year, date.tm_mon+1);
    mkdir(basename, 0755);

    FILE* in = fopen(filename, "r");
    if (!in) return -1;
    printf("Extracting file: %s\n", filename);

    codes_handle* h;
    int err;
    while (h = codes_grib_handle_new_from_file(0, in, &err)) {
        char dataDate_str[16];
        char dataTime_str[16];
        size_t dataDate_len = sizeof(dataDate_str);
        size_t dataTime_len = sizeof(dataTime_str);
        codes_get_string(h, "dataDate", dataDate_str, &dataDate_len);
        codes_get_string(h, "dataTime", dataTime_str, &dataTime_len);
        int dataDate = atoi(dataDate_str);
        int dataTime = atoi(dataTime_str);
        long startStep;
        codes_get_long(h, "startStep", &startStep);
        dataTime += startStep * 100;
        //printf("dataDate: %08d\n", dataDate);
        //printf("dataTime: %04d\n", dataTime);

        char out_filename[128];
        snprintf(out_filename, sizeof(out_filename), "%s.gdas.%04u%02u/%08d%04d.grb2", 
                    dataset_str[dataset], date.tm_year, date.tm_mon+1, dataDate, dataTime);
        codes_write_message(h, out_filename, "wb");
    }

    codes_handle_delete(h);   
    fclose(in);
    return 0;
}

char* cfsr_filename(cfsr_dataset_t dataset, struct tm date)
{
    static char filename[128];
    snprintf(filename, sizeof(filename), "%s.gdas.%04u%02u/%04u%02u%02u%02u%02u.grb2", 
                dataset_str[dataset], date.tm_year, date.tm_mon+1, 
                date.tm_year, date.tm_mon+1, date.tm_mday, date.tm_hour, date.tm_min);
    return filename;
}

static codes_nearest* nearest[CFSR_DATASET_MAX];

double cfsr_value(cfsr_dataset_t dataset, struct tm date, double lat, double lon)
{
    char* filename = cfsr_filename(dataset, date);
    struct stat st;
    if (stat(filename, &st)) {
        printf("cfsr_fetch %s\n", filename);
        cfsr_fetch(dataset, date);
    }

    int err;
    FILE* in = fopen(filename, "r");
    if (!in) return NAN;
    printf("Opened file: %s\n", filename);
    codes_handle* h = codes_grib_handle_new_from_file(0, in, &err);
    fclose(in);

    if(!nearest[dataset]) nearest[dataset] = codes_grib_nearest_new(h, &err);
    static double lats[4]={0,};
    static double lons[4]={0,};
    static double values[4]={0,};
    static double distances[4]={0,};
    static int indexes[4]={0,};
    static size_t size=4;
    codes_grib_nearest_find(nearest[dataset], h, lat, lon, CODES_NEAREST_SAME_GRID, 
                            lats, lons, values, distances, indexes, &size);
    // printf("\nIdx\tlat\tlon\tdist\tval\n");
    // for (int i=0;i<4;i++) printf("%d\t%.2f\t%.2f\t%g\t%g\n",
    //         (int)indexes[i],lats[i],lons[i],distances[i],values[i]);
    // printf("\n");

    codes_handle_delete(h);
    //codes_grib_nearest_delete(nearest);

    return cfsr_idw(values, distances, size);
}

double cfsr_idw(double* values, double* distances, size_t size)
{
    double weight[size]; 
    for (int i = 0; i < size; i++) weight[i] = 1./distances[i];
    double result = 0;
    for (int i = 0; i < size; i++) {
        result += values[i] * weight[i];
        //printf("%f %f %f\n", values[i], weight[i], result);
    }
    return result;
}