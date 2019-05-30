#ifndef CSV_HPP
#define CSV_HPP

#include <sstream>
#include "latlon.h"
#include "path.hpp"

void csv_writeHeader(std::stringstream &s);
void csv_writePt(std::stringstream &s, int step, pathpt_t pt);
std::stringstream csv_fromPath(path_t* path);

#endif