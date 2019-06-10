#include "csv.hpp"
#include <iomanip> // for std::setprecision
#include <sstream>
#include "path.hpp"

void csv_writeHeader(std::stringstream &s)
{
	s
	<< "Step#,"
	<< "Year,"
	<< "Month,"
	<< "Day,"
	<< "Hour,"
	<< "Lat,"
	<< "Lon,"
	<< "Ocean current U (m/s),"
	<< "Ocean current V (m/s),"
	<< "Sail open"
	<< "Wind U (m/s),"
	<< "Wind V (m/s),"
	<< "Wind gain U (m/s),"
	<< "Wind gain V (m/s),"
	<< "Total gain U (m/s),"
	<< "Total gain V (m/s),"
	// << "Ocean speed U (m/s),Ocean speed V (m/s),Ocean speed magnitude (m/s),Ocean heading (deg),"
	// << "Wind speed U (m/s),Wind speed V (m/s),Wind speed magnitude (m/s),Wind heading (deg),"
	// << "Sail open,"
	// << "Sail open steps#,"
	// << "Sail direction U,Sail direction V,Sail direction heading (deg),"
	// << "Sail speed U (m/s),Sail speed V (m/s),Sail speed magnitude (m/s),Sail speed heading (deg),"
	// << "Total speed U (m/s),Total speed V (m/s),Total speed magnitude (m/s),Total speed heading (deg),"
	<< "\n";
}

void csv_writePt(std::stringstream &s, int step, pathpt_t pt)
{
	s
	<< step << ","
	<< 1900 + pt.date.tm_year << ","
	<< pt.date.tm_mon+1 << ","
	<< pt.date.tm_mday << ","
	<< pt.date.tm_hour << ","
	<< pt.loc.lat << ","
	<< pt.loc.lon << ","
	<< pt.ocean.x << ","
	<< pt.ocean.y << ","
	<< pt.sailopen << ","
	<< pt.wind.x << ","
	<< pt.wind.y << ","
	<< pt.wind_gain.x << ","
	<< pt.wind_gain.y << ","
	<< pt.gain.x << ","
	<< pt.gain.y << ","
	// << pt.ocean << ","
	// << pt.ocean.norm() << ","
	// << pt.ocean.heading() << ","
	// << pt.wind << ","
	// << pt.wind.norm() << ","
	// << pt.wind.heading() << ","
	// << pt.sailopen << ","
	// << pt.sailstep << ","
	// << pt.sail_dir << ","
	// << pt.sail_dir.heading() << ","
	// << pt.sail_gain << ","
	// << pt.sail_gain.norm() << ","
	// << pt.sail_gain.heading() << ","
	// << pt.gain << ","
	// << pt.gain.norm() << ","
	// << pt.gain.heading() << ","
	<< "\n";
}

std::stringstream csv_fromPath(path_t* path)
{
	std::stringstream s;
	csv_writeHeader(s);
	for (int i = 0; i < path->pts.size(); i++)
		csv_writePt(s, i, path->pts[i]);
	return s;
}