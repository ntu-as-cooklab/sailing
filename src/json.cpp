#include "json.hpp"
#include "voyage.hpp"

void JSON::writeHeader(Voyage* voyage)
{
	json
	<< "{"
	<< "\"name\": \"" 	<< voyage->name << "\", "
	<< "\"startdate\": {"
		<< "\"year\": " << voyage->startdate.year << ", "
		<< "\"month\": " << voyage->startdate.month << ", "
		<< "\"day\": " << voyage->startdate.day << ", "
		<< "\"hour\": " << voyage->startdate.hour
	<< "}, "
	<< "\"enddate\": {"
		<< "\"year\": " << voyage->enddate.year << ", "
		<< "\"month\": " << voyage->enddate.month << ", "
		<< "\"day\": " << voyage->enddate.day << ", "
		<< "\"hour\": " << voyage->enddate.hour
	<< "}, "
	<< "\"days\": " 	<< voyage->days << ", "
	<< "\"mode\": " 	<< voyage->mode << ", "
	<< "\"orig\": {\"lat\": " << voyage->orig.lat() << ", \"lng\": " << voyage->orig.lon() << "}, "
	<< "\"dest\": {\"lat\": " << voyage->dest.lat() << ", \"lng\": " << voyage->dest.lon() << "}, "
	<< "\"altitude\": " 	<< voyage->altitude << ", "
	<< "\"windlimit\": " 	<< voyage->windlimit << ", "
	<< "\"sailopenhours\": " 	<< voyage->sailopenhours << ", "
	<< "\"alpha\": " 	<< voyage->alpha << ", "
	<< "\"timestep\": " 	<< voyage->timestep << ", "
	<< "\"movement_factor\": " 	<< voyage->movement_factor << ", "
	<< "\"range\": " 	<< voyage->range << ", "
	<< "\"bounds\": {\"lat\": " << voyage->bounds.lat() << ", \"lng\": " << voyage->bounds.lon() << "}, "
	<< "\"path\": [";
}

void JSON::writeLine(Voyage* voyage, bool first)
{
	json
	<< (first ? "{" : ", {")
	<< "\"runstep\": " << voyage->runstep << ", "
	<< "\"date\": {"
		<< "\"year\": " << voyage->date.year << ", "
		<< "\"month\": " << voyage->date.month << ", "
		<< "\"day\": " << voyage->date.day << ", "
		<< "\"hour\": " << voyage->date.hour
	<< "}, "
	<< "\"curr\": {\"lat\": " << voyage->curr.lat() << ", \"lng\": " << voyage->curr.lon() << "}, "
	<< "\"ocean\": [" << voyage->ocean << "], "
	<< "\"ocean_norm\": " << voyage->ocean.norm() << ", "
	<< "\"ocean_heading\": " << voyage->ocean.heading() << ", "
	<< "\"wind\": [" << voyage->wind << "], "
	<< "\"wind_norm\": " << voyage->wind.norm() << ", "
	<< "\"wind_heading\": " << voyage->wind.heading() << ", "
	<< "\"sailopen\": " << voyage->sailopen << ", "
	<< "\"sailstep\": " << voyage->sailstep << ", "
	<< "\"sail_dir\": [" << voyage->sail_dir << "], "
	<< "\"sail_dir_heading\": " << voyage->sail_dir.heading() << ", "
	<< "\"sail_gain\": [" << voyage->sail_gain << "], "
	<< "\"sail_gain_norm\": " << voyage->sail_gain.norm() << ", "
	<< "\"sail_gain_heading\": " << voyage->sail_gain.heading() << ", "
	<< "\"gain\": [" << voyage->gain << "], "
	<< "\"gain_norm\": " << voyage->gain.norm() << ", "
	<< "\"gain_heading\": " << voyage->gain.heading()
	<< "}";
}

void JSON::writeFooter()
{
	json
	<< "]}";
}
