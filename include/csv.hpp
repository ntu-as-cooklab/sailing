#ifndef CSV_HPP
#define CSV_HPP

#include <fstream>

struct CSV
{
	std::ofstream file;

	CSV(std::string filename) 	{ file.open(filename); }
	~CSV() 						{ file.close(); }
	void writeHeader();
	void writeLine();
};

#endif
