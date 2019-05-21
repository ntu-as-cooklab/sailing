#ifndef CSV_HPP
#define CSV_HPP

#include <fstream>
class Voyage;

struct CSV
{
	std::ofstream file;

	void open(std::string filename) { file.open(filename); }
	void close() 					{ file.close(); }
	void writeHeader();
	void writeLine(Voyage* voyage);
};

#endif
