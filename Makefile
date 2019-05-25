SHELL:=/bin/bash

LDLIBS += -lcurl -lm -leccodes -lnetcdf -lwebsockets
CPPFLAGS += -O2 -Isrc
CFLAGS 	+= -std=gnu11
CXXFLAGS += -std=c++14

SRC = $(wildcard src/*.c* src/*/*.c*)
OBJ = $(patsubst %,build/%.o,$(SRC))
BIN = $(patsubst apps/%,bin/%,$(basename $(wildcard apps/*.c*)))

all: $(BIN)

bin/%: apps/%.* $(OBJ)
	mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ $(LDLIBS) -o $@

build/%.cpp.o: %.cpp
	mkdir -p $(@D)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $^ -o $@

build/%.c.o: %.c
	mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $^ -o $@

.PHONY: clean
clean:
	rm -rf build/ bin/

MONTHS := $(shell d="1979-01-02"; until [[ $$d > 2011-12-31 ]]; do echo "$$(date +%Y%m -d $$d)"; d=$$(date -I -d "$$d + 1 month"); done)
FIELDS := ocnu5 ocnv5

.PHONY: grb2 nc
grb2 nc: %: $(foreach month,$(MONTHS),$(foreach field,$(FIELDS),data/$(field).gdas.$(month).%))

%.grb2:
	-mkdir -p $(@D); \
	YYYYMM=$$(echo "$@"|cut -d. -f3); \
	cd $(@D) && curl -O ftp://nomads.ncdc.noaa.gov/CFSR/HP_time_series/$$YYYYMM/$(notdir $@)

%.nc: %.grb2
	grib_to_netcdf -T -I type -k3 -o $@ $<

.PHONY: install
install:
	cd www; npm i