#include <iostream>

#include <cfsr.hpp>
#include <sailing.hpp>
#include <kml.hpp>

int main()
{
	UV v(-1,1);
	std::cout << v.heading();

	Voyage voyage(LatLon(30.791412, 122.269901), LatLon(33.823205, 134.887155)); // origin, destination
	voyage.sail();

   	return 0;
}
