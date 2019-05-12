LDLIBS += -lcurl

bin/cfsr_test: test.c cfsr_ftp.c
	mkdir -p $(@D)
	$(CC) $^ $(LDLIBS) -o $@