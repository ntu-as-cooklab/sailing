#include <iostream>

#include <cfsr.hpp>
#include <sailing.hpp>

int main()
{
	Voyage voyage(LatLon(-17.5, 176.5), LatLon(-6.5, 155.5)); // origin, destination
	voyage.sail();

   	return 0;
}
