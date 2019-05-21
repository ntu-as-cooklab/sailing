#include "csv.hpp"
#include "voyage.hpp"

void CSV::writeHeader()
{
	file
	<< "Steps#,"
	<< "Year,"
	<< "Month,"
	<< "Day,"
	<< "Hour,"
	<< "Lat,Lon,"
	<< "Ocean speed U (m/s),Ocean speed V (m/s),Ocean speed magnitude (m/s),Ocean heading (deg),"
	<< "Wind speed U (m/s),Wind speed V (m/s),Wind speed magnitude (m/s),Wind heading (deg),"
	<< "Sail open,"
	<< "Sail open steps#,"
	<< "Sail direction U,Sail direction V,Sail direction heading (deg),"
	<< "Sail speed U (m/s),Sail speed V (m/s),Sail speed magnitude (m/s),Sail speed heading (deg),"
	<< "Total speed U (m/s),Total speed V (m/s),Total speed magnitude (m/s),Total speed heading (deg),"
	<< "\n";
}

void CSV::writeLine(Voyage* voyage)
{
	file
	<< voyage->runstep << ","
	<< voyage->date.year << ","
	<< voyage->date.month << ","
	<< voyage->date.day << ","
	<< voyage->date.hour << ","
	<< voyage->curr << ","
	<< voyage->ocean << ","
	<< voyage->ocean.norm() << ","
	<< voyage->ocean.heading() << ","
	<< voyage->wind << ","
	<< voyage->wind.norm() << ","
	<< voyage->wind.heading() << ","
	<< voyage->sailopen << ","
	<< voyage->sailstep << ","
	<< voyage->sail_dir << ","
	<< voyage->sail_dir.heading() << ","
	<< voyage->sail_gain << ","
	<< voyage->sail_gain.norm() << ","
	<< voyage->sail_gain.heading() << ","
	<< voyage->gain << ","
	<< voyage->gain.norm() << ","
	<< voyage->gain.heading() << ","
	<< "\n";
}
