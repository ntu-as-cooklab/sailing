#ifndef KML_HPP
#define KML_HPP

#include <fstream>
#include <latlon.hpp>

struct KML
{
	std::ofstream file;

	KML(std::string filename) 	{ file.open(filename); }
	~KML() 						{ file.close(); }
	void writeHeader();
	void writeFooter();
	void writeLatLon(LatLon latlon, float altitude);
};

#endif
