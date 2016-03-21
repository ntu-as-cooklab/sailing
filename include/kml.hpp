#ifndef KML_HPP
#define KML_HPP

#include <fstream>
#include <latlon.hpp>

struct KML
{
	std::ofstream kmlfile;

	KML(std::string filename) { kmlfile.open(filename); writeHeader(); }
	~KML() { writeFooter(); kmlfile.close(); }
	void writeHeader();
	void writeFooter();
	void writeLatLon(LatLon latlon);
};

#endif
