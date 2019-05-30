#include "kml.hpp"
#include <iomanip> // for std::setprecision
#include <sstream>

void kml_writeHeader(std::stringstream &s)
{
	s <<
   	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
	"<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n"
	"	<Document>\n"
	"		<Placemark>\n"
	"		    <name>Voyage path</name>\n"
	"			<Style>\n"
	"				<LineStyle>\n"
	"					<color>ff0000ff</color>\n"
	"					<width>5</width>\n"
	"				</LineStyle>\n"
	"			</Style>\n"
	"		    <LineString>\n"
	"		        <altitudeMode>absolute</altitudeMode>\n"
	"		        <tessellate>1</tessellate>\n"
	"		        <coordinates>\n";
}

void kml_writeFooter(std::stringstream &s)
{
	s <<
	"				</coordinates>\n"
	"	        </LineString>\n"
	"	    </Placemark>\n"
	"	</Document>\n"
	"</kml>";
}

void kml_writeLatLon(std::stringstream &s, latlon_t latlon, double altitude)
{
	s << std::fixed << std::setprecision(6) << latlon.lon << "," << latlon.lat << "," << altitude << "\n";
}
