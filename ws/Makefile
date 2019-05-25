SHELL:=/bin/bash

LDLIBS += -lwebsockets
CPPFLAGS += -O2
CFLAGS 	+= -std=gnu11

bin/test_ws: *.c
	mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $(CFLAGS) $^ $(LDLIBS) -o $@

.PHONY: clean
clean:
	rm -rf bin/