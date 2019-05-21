SHELL:=/bin/bash

LDLIBS += -lm
CPPFLAGS += -O2
CFLAGS 	+= -std=gnu11
CXXFLAGS += -std=c++14

SRC = $(wildcard src/*.cpp src/*.c)
OBJ = $(patsubst src/%,build/%.o,$(SRC))

bin/sailing: $(OBJ)
	mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ $(LDLIBS) -o $@

build/%.cpp.o: src/%.cpp
	mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $^ -o $@

build/%.c.o: src/%.c
	mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $^ -o $@

.PHONY: clean
clean:
	rm -rf build/ bin/

define data_src
$(shell d="1979-01-02"; until [[ $$d > 2011-12-31 ]]; do echo "$1.gdas.$$(date +%Y%m -d $$d).$2"; d=$$(date -I -d "$$d + 1 month"); done)
endef

.PHONY: ocnu5 ocnv5
ocnu5 ocnv5: %: $(call data_src,%,nc)

%.grb2:
	MONTH=$$(echo "$@"|cut -d. -f3); \
	curl -O ftp://nomads.ncdc.noaa.gov/CFSR/HP_time_series/$$MONTH/$@

%.nc: %.grb2
	grib_to_netcdf -I type,step -k3 -o $@ $<