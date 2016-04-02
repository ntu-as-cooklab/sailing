#include <iostream>
#include <iomanip> // for std::setprecisionma

#include "voyage.hpp"
#include "cfsr.hpp"
#include "interface.hpp"

UV Voyage::adj_direction(LatLon curr, LatLon dest)
{
	return UV(	lon2km((curr+dest).lat()/2) * (dest-curr).lon(),
				lat2km((curr+dest).lat()/2) * (dest-curr).lat()
			).normalize();
}

float Voyage::boat_speed(UV wind, UV dir)
{
	float wind_speed = norm(wind); //norm(wind)<8 ? norm(wind) : 0;
	float angle_diff = anglediff(wind, dir);

	if (angle_diff <= 30)
	    return (-0.0008*pow(wind_speed,3) - 0.0037*pow(wind_speed,2) + 0.5548*wind_speed ) * ( -0.0333*angle_diff+1 ) +
	              ( 0.0023*pow(wind_speed,3) - 0.0699*pow(wind_speed,2) + 0.9546*wind_speed ) * ( 0.0333*angle_diff );
	else if (angle_diff <= 60 && angle_diff > 30)
	    return ( 0.0023*pow(wind_speed,3) - 0.0699*pow(wind_speed,2) + 0.9546*wind_speed ) * ( -0.0333*angle_diff+2 ) +
	              ( 0.0065*pow(wind_speed,3) - 0.1715*pow(wind_speed,2) + 1.5687*wind_speed ) * ( 0.0333*angle_diff-1 ) ;
	else if (angle_diff <= 90 && angle_diff > 60)
	    return ( 0.0065*pow(wind_speed,3) - 0.1715*pow(wind_speed,2) + 1.5687*wind_speed ) * ( -0.0333*angle_diff+3 ) +
	              ( 0.0057*pow(wind_speed,3) - 0.1596*pow(wind_speed,2) + 1.5091*wind_speed ) * ( 0.0333*angle_diff-2 );
	else if (angle_diff <= 120 && angle_diff > 90)
	    return ( 0.0057*pow(wind_speed,3) - 0.1596*pow(wind_speed,2) + 1.5091*wind_speed  ) * ( -0.0333*angle_diff+4 ) +
	              ( 0.0062*pow(wind_speed,3) - 0.1684*pow(wind_speed,2) + 1.4957*wind_speed ) * ( 0.0333*angle_diff-3 );
	else if (angle_diff <= 150 && angle_diff > 120)
	    return ( 0.0062*pow(wind_speed,3) - 0.1684*pow(wind_speed,2) + 1.4957*wind_speed ) * ( -0.0333*angle_diff+5 ) +
	              ( 0.0022*pow(wind_speed,3) - 0.0780*pow(wind_speed,2) + 0.9223*wind_speed ) * ( 0.0333*angle_diff-4 );
	else if (angle_diff <= 180 && angle_diff > 150)
	    return ( 0.0022*pow(wind_speed,3) - 0.0780*pow(wind_speed,2) + 0.9223*wind_speed ) * ( -0.0333*angle_diff+6 ) +
	              ( 0.0030*pow(wind_speed,3) - 0.0871*pow(wind_speed,2) + 0.8549*wind_speed ) * ( 0.0333*angle_diff-5 );
	return 0;
}

// 給予初始經緯度,應該往南北與東西移動的速度(m/s),時間步距 --> 計算出下一點的位置
LatLon Voyage::calcu_next_place(LatLon curr, UV speed)
{
	// speed: in (m/s)
	return curr + movement_factor * timestep/1000
		* LatLon(speed.y/lat2km(curr.lat()), speed.x/lon2km(curr.lat()));
}

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// 給予經緯度、順推逆推、推算時間、推算方式(瓶中信、動力與否)
// 讀取資料
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool Voyage::sail() // result: whether we reached our destination
{
	std::cout << "\n[Voyage] Running simulation...\n";
	kml.open("voyage.kml");
	kml.writeHeader();
	int	runday = 0, runhour = 0; // count simulation time
	LatLon curr = orig; // current position

	// Sums --> used for averages
	float 	Total_Ocean_Sp         = 0,
			Total_Ocean_Angle_Diff = 0,
			Total_Wind_Sp          = 0,
			Total_Wind_Angle_Diff  = 0,
			Total_Wind_Gain        = 0, // speed gain due to wind
			Total_Ocean_Gain       = 0; // speed gain due to ocean

	for (int year=1979; year<=1979; year++)
		for (int month=1; month<=1; month++)
		{
			int ouid = openCFSR(CFSR_OU, year, month),
				ovid = openCFSR(CFSR_OV, year, month),
				auid = openCFSR(CFSR_U, year, month),
				avid = openCFSR(CFSR_V, year, month);

			int daymax = 26; // TODO
			for (int day=1; day<=daymax; day++, runday++)
				for (int hour=1; hour<=1; hour++, runhour++, sail_open = hour<=12) // only open sail for half a day
				{
					//std::cout << "Time: " << year << " " << month << " " << day << " " << hour << "\n";
					//std::cout << "Current position: " << std::fixed << std::setprecision(6) << curr << "\n";
					// TODO: interpolate ocean & wind in space and time using data from two days
					// (hour-1)/24

					// Ocean current:
					UV ocean = getOUV(ouid, ovid, day, curr);
					//std::cout << "ouid: " << ouid << "\n";
					//std::cout << "ovid: " << ovid << "\n";
					//std::cout << "Ocean: " << ocean << "\n";

					// Wind speed:
					UV wind =
						sail_open ? // if sail is open
						getAUV(auid, avid, day, curr) * pow(altitude/10.0,alpha) :
						0;
					// calculate wind speed at (2m) from wind speed at 10m using wind profile power law
					/*wind = OUV(curr, (hour-1)/24);
					WindSP_10m = norm(wind);
					WindSP_2m  = WindSP_10m / pow(5,alpha); // wind profile power law
					wind *= (WindSP_2m / WindSP_10m);*/
					//std::cout << "Wind: " << wind << "\n";

					// adjust boat direction and calculate boat speed gain due to wind
					// TODO: path finding
					//dir = new UV(1,-1);
					UV 		sail_dir 	= wind.normalize(); //(dir ? *dir : adj_direction(curr, dest)).normalize();
					//std::cout << "Sail dir: " << sail_dir << "\n";
					float 	sail_sp 	= boat_speed(wind, sail_dir); // boat speed gain due to wind
					//std::cout << "Sail sp: " << sail_sp << "\n";

					// boat movement vector due to wind from direction and speed
					UV wind_gain = sail_sp * sail_dir;
					//std::cout << "Wind gain: " << wind_gain << "\n";

					// total speed
					UV gain = ocean + wind_gain;
					//std::cout << "Total gain: " << gain << "\n";

					// calculate next place
					curr = calcu_next_place(curr, gain);
					/*printf("\n[Timestep %03d]  %d/%02d/%02d %02d:00\n", runhour, year, month, day, hour);
					printf("Position:    (%6f, %6f)\n", curr.lat(), curr.lon());
					printf("Heading:     %5f\n", sail_dir.heading());
					printf("Ocean gain:  %5f (%5f, %5f)\n", ocean.norm(), ocean.x, ocean.y);
					printf("Ocean angle: %5f\n", anglediff(ocean, sail_dir));
					printf("Wind:        %5f, %5f\n", wind.x, wind.y);
					printf("Wind angle:  %5f\n", anglediff(wind, sail_dir));
					printf("Wind gain:   %5f (%5f, %5f)\n", sail_sp, wind_gain.x, wind_gain.y);
					printf("Total gain:  %5f, %5f\n", gain.x, gain.y);*/
					kml.writeLatLon(curr, altitude);

					// increment total speed
					Total_Ocean_Sp         += norm(ocean);
					Total_Wind_Sp          += norm(wind);

					// increment total angle with boat (in degrees)
					Total_Ocean_Angle_Diff += anglediff(ocean, sail_dir);
					Total_Wind_Angle_Diff  += anglediff(wind,  sail_dir);

					// increment total speed gain
					Total_Ocean_Gain       += dot(sail_dir, ocean);
					Total_Wind_Gain        += sail_sp;

					// calculate averages from start of simulation up to now
					float 	Avg_Wind_Gain 			= Total_Wind_Gain*2 		/ runhour,
							Avg_Ocean_Gain 			= Total_Ocean_Gain 			/ runhour,
							Avg_Ocean_Sp 			= Total_Ocean_Sp 			/ runhour,
							Avg_Ocean_Angle_Diff 	= Total_Ocean_Angle_Diff 	/ runhour,
							Avg_Wind_Sp 			= Total_Wind_Sp*2 			/ runhour,
							Avg_Wind_Angle_Diff 	= Total_Wind_Angle_Diff*2 	/ runhour;

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

					//std::cout << "\n";
				}
			closeCFSR(ouid);
			closeCFSR(ovid);
			closeCFSR(auid);
			closeCFSR(avid);
		}

	printf("[Voyage] Reached end of time range\n");
	kml.writeFooter();
	kml.close();
	std::cout << parseCmd("send voyage.kml") << "\n";
	return false;
}
