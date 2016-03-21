#include <kml.hpp>

void KML::writeHeader()
{
	kmlfile <<
	"<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
	<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n\
		<Document>\n\
		   <Style id=\"yellowPoly\">\n\
		       <LineStyle>\n\
		       <color>7f00ffff</color>\n\
		       <width>4</width>\n\
		   </LineStyle>\n\
		   <PolyStyle>\n\
		       <color>7f00ff00</color>\n\
		   </PolyStyle>\n\
		   </Style>\n\
		    <Placemark>\n\
		        <name>iMet path</name>\n\
		        <styleUrl>#yellowPoly</styleUrl>\n\
		        <LineString id=\"sailing\">\n\
		            <altitudeMode>absolute</altitudeMode>\n\
		            <extrude>1</extrude>\n\
		            <tessellate>1</tessellate>\n\
		            <coordinates>\n";
}

void KML::writeFooter()
{
	kmlfile <<
	"				</coordinates>\n\
		        </LineString>\n\
		    </Placemark>\n\
		    <Placemark>\n\
		        <name>Boat</name>\n\
		        <Point id=\"Boat\">\n\
		            <coordinates>\n\
					</coordinates>\n\
		        </Point>\n\
		    </Placemark>\n\
		</Document>\n\
	</kml>";
}

void KML::writeLatLon(LatLon latlon)
{
	kmlfile << latlon.lon() << ", " << latlon.lat() << "\n";
}
