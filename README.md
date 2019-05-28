# Sailing
This is a sailing simulation based on [CFSR wind and ocean current data](https://nomads.ncdc.noaa.gov/data/). The goal is to simulate the sailing route of a sailing ship during different seasons based on a simple model, which may be helpful for studies on human migration across the ocean.

* [NOAA CFS](https://www.ncdc.noaa.gov/data-access/model-data/model-datasets/climate-forecast-system-version2-cfsv2)
* [GRIB Parameter Database](https://apps.ecmwf.int/codes/grib/param-db/)

## Prerequisites

* [libcurl](https://curl.haxx.se/libcurl/)
* [ecCodes](https://confluence.ecmwf.int//display/ECC/ecCodes+Home)
* [libwebsockets](https://libwebsockets.org/)
* [netcdf](https://www.unidata.ucar.edu/software/netcdf/)
* [libcbor](https://github.com/PJK/libcbor)
* GCC 8 (gcc/g++)
* nodejs, npm

## Instructions

To build,
```console
$ git submodule init
$ git submodule update
$ make
```

## Project directory structure
* src: Source files for main application
* www: Source files for browser frontend
