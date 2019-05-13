LDLIBS += -lcurl -lm -lnetcdf

bin/cfsr_test: test.c cfsr_*.c
	mkdir -p $(@D)
	$(CC) $^ $(LDLIBS) -o $@