#ifndef KML_HPP
#define KML_HPP

#include <sstream>
#include "latlon.h"

void kml_writeHeader(std::stringstream &s);
void kml_writeFooter(std::stringstream &s);
void kml_writeLatLon(std::stringstream &s, latlon_t latlon, double altitude);

#endif