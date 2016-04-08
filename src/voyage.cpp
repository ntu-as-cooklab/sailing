#include <iostream>
#include <iomanip> // for std::setprecisionma

#include "voyage.hpp"
#include "cfsr.hpp"
#include "interface.hpp"

extern CfsrReader* cfsrReader;
bool Voyage::debug = false;

UV Voyage::adj_direction(LatLon curr, LatLon dest)
{
	return UV(	lon2km((curr+dest).lat()/2) * (dest-curr).lon(),
				lat2km((curr+dest).lat()/2) * (dest-curr).lat()
			).normalize();
}

UV Voyage::calc_sail_gain(UV wind, UV dir)
{
	float wind_speed = norm(wind); //norm(wind)<8 ? norm(wind) : 0;
	float angle_diff = anglediff(wind, dir);
	float boat_speed = 0;

	if (angle_diff <= 30)
	    boat_speed = (-0.0008*pow(wind_speed,3) - 0.0037*pow(wind_speed,2) + 0.5548*wind_speed ) * ( -0.0333*angle_diff+1 ) +
	              ( 0.0023*pow(wind_speed,3) - 0.0699*pow(wind_speed,2) + 0.9546*wind_speed ) * ( 0.0333*angle_diff );
	else if (angle_diff <= 60 && angle_diff > 30)
	    boat_speed = ( 0.0023*pow(wind_speed,3) - 0.0699*pow(wind_speed,2) + 0.9546*wind_speed ) * ( -0.0333*angle_diff+2 ) +
	              ( 0.0065*pow(wind_speed,3) - 0.1715*pow(wind_speed,2) + 1.5687*wind_speed ) * ( 0.0333*angle_diff-1 ) ;
	else if (angle_diff <= 90 && angle_diff > 60)
	    boat_speed = ( 0.0065*pow(wind_speed,3) - 0.1715*pow(wind_speed,2) + 1.5687*wind_speed ) * ( -0.0333*angle_diff+3 ) +
	              ( 0.0057*pow(wind_speed,3) - 0.1596*pow(wind_speed,2) + 1.5091*wind_speed ) * ( 0.0333*angle_diff-2 );
	else if (angle_diff <= 120 && angle_diff > 90)
	    boat_speed = ( 0.0057*pow(wind_speed,3) - 0.1596*pow(wind_speed,2) + 1.5091*wind_speed  ) * ( -0.0333*angle_diff+4 ) +
	              ( 0.0062*pow(wind_speed,3) - 0.1684*pow(wind_speed,2) + 1.4957*wind_speed ) * ( 0.0333*angle_diff-3 );
	else if (angle_diff <= 150 && angle_diff > 120)
	    boat_speed = ( 0.0062*pow(wind_speed,3) - 0.1684*pow(wind_speed,2) + 1.4957*wind_speed ) * ( -0.0333*angle_diff+5 ) +
	              ( 0.0022*pow(wind_speed,3) - 0.0780*pow(wind_speed,2) + 0.9223*wind_speed ) * ( 0.0333*angle_diff-4 );
	else if (angle_diff <= 180 && angle_diff > 150)
	    boat_speed = ( 0.0022*pow(wind_speed,3) - 0.0780*pow(wind_speed,2) + 0.9223*wind_speed ) * ( -0.0333*angle_diff+6 ) +
	              ( 0.0030*pow(wind_speed,3) - 0.0871*pow(wind_speed,2) + 0.8549*wind_speed ) * ( 0.0333*angle_diff-5 );
	return boat_speed * dir;
}

// 給予初始經緯度,應該往南北與東西移動的速度(m/s),時間步距 --> 計算出下一點的位置
LatLon Voyage::calc_next_place(LatLon curr, UV speed)
{
	// speed: in (m/s)
	return curr + movement_factor * timestep/1000
		* LatLon(speed.y/lat2km(curr.lat()), speed.x/lon2km(curr.lat()));
}

void Voyage::sail_timestep()
{
	if (debug) std::cout
		<< "[Voyage] timestep: " << runhour << "\n"
		<< "[Voyage] time: " << date << "\n";

	// Ocean current:
	UV ocean = cfsrReader->OUV(date, curr);
	if (debug) std::cout << std::fixed
		<< "[Voyage] ocean: " << ocean << " (" << ocean.norm() << ")\n"
		<< std::setprecision(1) << "[Voyage] ocean heading: " << ocean.heading() << "\n";

	UV wind, sail_dir, sail_gain;
	switch (sailmode)
	{
		case WIND: case DIR: case DEST:
			// Wind speed:
			wind =	sail_open ? // if sail is open
					cfsrReader->AUV(date, curr) * pow(altitude/10.0,alpha) : // calculate wind speed at (2m) from wind speed at 10m using wind profile power law
					0;
			if (debug) std::cout << std::fixed
				<< "[Voyage] wind: " << wind << " (" << wind.norm() << ")\n"
				<< std::setprecision(1) << "[Voyage] wind heading: " << wind.heading() << "\n";

			// adjust boat direction and calculate boat speed gain due to wind
			// TODO: path finding
			sail_dir =  (
							sailmode == DRIFT 	? 	wind.normalize() :
							sailmode == DIR 	? 	dir :
							sailmode == DEST 	? 	adj_direction(curr, dest) :
							0
						).normalize();
			if (debug) std::cout << std::fixed << std::setprecision(1) << "[Voyage] sail heading: " << sail_dir.heading() << "\n";

			sail_gain = calc_sail_gain(wind, sail_dir); // boat speed gain due to wind
			if (debug) std::cout << "[Voyage] sail gain: " << sail_gain << " (" << sail_gain.norm() << ")\n";
	}

	// total speed
	UV gain = ocean + sail_gain;
	if (debug) std::cout << "[Voyage] total gain: " << gain << " (" << gain.norm() << ")\n";

	// calculate next place
	curr = calc_next_place(curr, gain);
	if (debug) std::cout << "[Voyage] position: " << curr << "\n";
	kml.writeLatLon(curr, altitude);

	// increment values for calculating averages
	Total_Ocean_Sp		+= ocean.norm();
	Total_Wind_Sp		+= wind.norm();
	Total_Ocean_Angle 	+= anglediff(ocean, sail_dir);
	Total_Wind_Angle  	+= anglediff(wind,  sail_dir);
	Total_Ocean_Gain	+= ocean * sail_dir; // dot product
	Total_Wind_Gain   	+= sail_gain.norm();

	// check if current position is within target range
	//if (norm(curr - dest) < range)
	//{ // TODO: use real distance
	//	printf("[Voyage] Reached destination\n");
	//	return true;
	//}

	// check if current position is out of bounds --> travel failed
	//if (curr.lon() > bounds.lon() || curr.lat() < bounds.lat() )
	//{ // TODO: generalize
	//	printf("\nSailed out of bounds\n");
	//	return false;
	//}

	if (debug) std::cout << "\n";
}


bool Voyage::sail() // result: whether we reached our destination
{
	std::cout << "\n[Voyage] Running simulation...\n";
	kml.open("voyage.kml");
	kml.writeHeader();
	curr = orig;
	date = {1979, 1,  1,  0};

	while (runhour++, date++ < enddate) // only open sail for half a day
	{
		if ((sail_open = date.hour<=12)) sailhour++;
		sail_timestep();

		// calculate averages from start of simulation up to now
		float 	Avg_Wind_Gain 	= Total_Wind_Gain 	/ sailhour,
				Avg_Ocean_Gain 	= Total_Ocean_Gain 	/ sailhour,
				Avg_Ocean_Sp 	= Total_Ocean_Sp 	/ runhour,
				Avg_Ocean_Angle = Total_Ocean_Angle / runhour,
				Avg_Wind_Sp 	= Total_Wind_Sp 	/ runhour,
				Avg_Wind_Angle	= Total_Wind_Angle 	/ runhour;
	}

	printf("[Voyage] Reached end of time range\n");
	kml.writeFooter();
	kml.close();
	execCmd("send voyage.kml");
	return false;
}
