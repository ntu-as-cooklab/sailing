LDLIBS += -lcurl -lm -leccodes
CPPFLAGS += -Ofast

.PHONY: all clean
all: $(patsubst %.c,bin/%,$(wildcard test_*.c))

bin/test_%: test_%.c cfsr*.c util*.c
	mkdir -p $(@D)
	$(CC) $(CPPFLAGS) $^ $(LDLIBS) -o $@

%.c: %.h

clean:
	rm -rf bin
	rm -rf ocnu5.gdas.*/