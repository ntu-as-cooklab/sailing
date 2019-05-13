LDLIBS += -lcurl -lm -lnetcdf

bin/cfsr_test: test.c cfsr_ftp.c cfsr_interp.c cfsr.c
	mkdir -p $(@D)
	$(CC) $^ $(LDLIBS) -o $@