# Sailing
This is a sailing simulation based on [CFSR wind and ocean current data](https://nomads.ncdc.noaa.gov/data/). The goal is to simulate the sailing route of a sailing ship during different seasons based on a simple model, which may be helpful for studies on human migration across the ocean.

* [NOAA CFS](https://www.ncdc.noaa.gov/data-access/model-data/model-datasets/climate-forecast-system-version2-cfsv2)
* [GRIB Parameter Database](https://apps.ecmwf.int/codes/grib/param-db/)

## Prerequisites

* [libcurl](https://curl.haxx.se/libcurl/)
* [ecCodes](https://confluence.ecmwf.int//display/ECC/ecCodes+Home)
* [libwebsockets](https://libwebsockets.org/) (v3.1.0)
* [netcdf](https://www.unidata.ucar.edu/software/netcdf/)
* [libcbor](https://github.com/PJK/libcbor)
* [nlohmann-json](https://github.com/nlohmann/json) (v3.6.1)
* GCC 8 (gcc/g++)
* CMake
* nodejs
* npm

For Ubuntu 18.04,
```console
# apt-get install gcc-8 g++-8 libcurl-dev libeccodes-dev libeccodes-tools libnetcdf-dev libcbor-dev cmake libssl1.0-dev nodejs-dev npm
```

### libwebsockets:
```console
$ git clone https://github.com/warmcat/libwebsockets
$ cd libwebsockets
$ git checkout v3.1.0
$ mkdir build; cd build
$ cmake ..
$ make -j8
$ sudo make install
$ sudo ldconfig
```
### nlohmann-json:
```console
$ git clone https://github.com/nlohmann/json
$ cd json
$ git checkout v3.6.1
$ mkdir build; cd build
$ cmake ..
$ make -j8
$ sudo make install
$ sudo ldconfig
```

## Instructions

To build,
```console
$ git submodule init
$ git submodule update
$ make npm
$ make
```

To run,
```console
$ sudo bin/sailing
```

## Project directory structure
* [src](src): Source files for main application
* [www](www): Source files for browser frontend
