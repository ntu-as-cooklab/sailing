# Sailing
This is a sailing simulation based on [CFSR wind and ocean current data](https://nomads.ncdc.noaa.gov/data/). The goal is to simulate the sailing route of a sailing ship during different seasons based on a simple model, which may be helpful for studies on human migration across the ocean.

* [NOAA CFS](https://www.ncdc.noaa.gov/data-access/model-data/model-datasets/climate-forecast-system-version2-cfsv2)

## Prerequisites

* [libcurl](https://curl.haxx.se/libcurl/)
* [ecCodes](https://confluence.ecmwf.int//display/ECC/ecCodes+Home)

## Instructions

To build,
```console
$ git submodule init
$ git submodule update
$ mkdir build
$ cd build
$ cmake ..
$ make
```

The program expects the CFSR data to be in the `data` folder, arranged in a directory structure like:
```
ocea/OU/CFSR_ocnh06.gdas.OU_YYYYMM_5.nc
ocea/OV/CFSR_ocnh06.gdas.OV_YYYYMM_5.nc
atmo/U10/CFSR_flxf06.gdas.U_10m_YYYYMM.nc
atmo/V10/CFSR_flxf06.gdas.V_10m_YYYYMM.nc
```

The application follows a server-client pattern, with a C++ based backend and a Javascript frontend run in the browser. Calculated sailing routes are saved into the `www/output` folder.

## Project directory structure
* cfsr: C code for parsing CFSR data.
* float2: C++ code for handling 2-dimensional float vectors
* include: Headers for main C++ application
* res: Application icons
* src: Source files for main C++ application
* systemd: systemd unit files for auto startup on Linux
* websocketpp: websocketpp submodule
* www: Source files for browser frontend
* barnes.cpp.for: Barnes interpolation code not yet merged into main application
* build.bat: build script for Windows
