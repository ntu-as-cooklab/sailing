windres res/sailing.rc -O coff -o res/sailing.res
cd build && make -j4 && cd ..
