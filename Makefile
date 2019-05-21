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