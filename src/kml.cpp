#include <kml.hpp>

void KML::writeHeader()
{
	kmlfile <<
   "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n\
	<kml xmlns=\"http://www.opengis.net/kml/2.2\">\n\
		<Document>\n\
			<Placemark>\n\
			    <name>Voyage path</name>\n\
				<Style>\n\
					<LineStyle>\n\
						<color>ff0000ff</color>\n\
						<width>5</width>\n\
					</LineStyle>\n\
				</Style>\n\
			    <LineString>\n\
			        <altitudeMode>absolute</altitudeMode>\n\
			        <tessellate>1</tessellate>\n\
			        <coordinates>\n";
}

void KML::writeFooter()
{
	kmlfile <<
	"				</coordinates>\n\
		        </LineString>\n\
		    </Placemark>\n\
		</Document>\n\
	</kml>";
}

void KML::writeLatLon(LatLon latlon, float altitude)
{
	kmlfile << latlon.lon() << "," << latlon.lat() << "," << altitude << "\n";
}
