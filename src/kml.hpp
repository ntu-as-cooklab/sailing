#ifndef KML_HPP
#define KML_HPP

#include <sstream>
#include "latlon.h"
#include "path.hpp"

void kml_writeHeader(std::stringstream &s);
void kml_writeFooter(std::stringstream &s);
void kml_writeLatLon(std::stringstream &s, latlon_t latlon, double altitude);
std::stringstream kml_fromPath(path_t* path);

#endif