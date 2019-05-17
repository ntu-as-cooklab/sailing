LDLIBS += -lcurl -lm -lnetcdf -leccodes

.PHONY: all clean
all: $(patsubst %.c,bin/%,$(wildcard test_*.c))

bin/test_%: test_%.c cfsr*.c util*.c
	mkdir -p $(@D)
	$(CC) $^ $(LDLIBS) -o $@

%.c: %.h

clean:
	rm -rf bin