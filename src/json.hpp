#ifndef JSON_HPP
#define JSON_HPP

#include <sstream>

class Voyage;

struct JSON
{
	std::stringstream json;

	void clear() { json = std::stringstream(); }
	void writeLine(Voyage* voyage, bool first = false);
	void writeHeader(Voyage* voyage);
	void writeFooter();
};

#endif
