#include <sailing.hpp>
#include <cfsr.hpp>

// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
// 給予經緯度、順推逆推、推算時間、推算方式(瓶中信、動力與否)
// 讀取資料
// %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

bool Voyage::sail() // result: whether we reached our destination
{
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
				for (int hour=1; hour<=24; hour++, runhour++) // hour
				{
					// TODO: interpolate ocean & wind in space and time using data from two days
					// (hour-1)/24

					// Ocean current:
					UV ocean = getOUV(ouid, ovid, day, curr);

					// Wind speed:
					UV wind =
						hour <=12 ? // hours when sail is open, TODO: check model
						getAUV(auid, avid, day, curr) / pow(5,alpha) :	// open sail for hours <= 12
						0; 												// no sail for hours > 12
					/*// calculate wind speed at 2m from wind speed at 10m
					wind = OUV(curr, (hour-1)/24);
					WindSP_10m = norm(wind);
					WindSP_2m  = WindSP_10m / pow(5,alpha); // wind profile power law
					wind *= (WindSP_2m / WindSP_10m);*/

					// adjust boat direction and calculate boat speed gain due to wind
					UV 		boat_dir 	= adj_direction(curr, dest);
					float 	boat_sp 	= boat_speed(wind, boat_dir); // boat speed gain due to wind

					// boat movement vector due to wind from direction and speed
					UV wind_gain = boat_sp * boat_dir;

					// total speed
					UV gain = ocean + wind; //wind_gain;

					// calculate next place
					curr = calcu_next_place(curr, gain);
					printf("\n[Timestep %03d]  %d/%02d/%02d %02d:00\n", runhour, year, month, day, hour);
					printf("(%6f, %6f)\n", curr.lat(), curr.lon());
					printf("Ocean gain:  %5f, %5f\n", ocean.x, ocean.y);
					printf("Wind:        %5f, %5f\n", wind.x, wind.y);
					printf("Wind gain:   %5f, %5f\n", wind_gain.x, wind_gain.y);
					printf("Total gain:   %5f, %5f\n", gain.x, gain.y);

					// increment total speed
					Total_Ocean_Sp         += norm(ocean);
					Total_Wind_Sp          += norm(wind);

					// increment total angle with boat (in degrees)
					Total_Ocean_Angle_Diff += anglediff(ocean, boat_dir);
					Total_Wind_Angle_Diff  += anglediff(wind,  boat_dir);

					// increment total speed gain
					Total_Ocean_Gain       += dot(boat_dir, ocean);
					Total_Wind_Gain        += boat_sp;

					// calculate averages from start of simulation up to now
					float 	Avg_Wind_Gain 			= Total_Wind_Gain*2 		/ runhour,
							Avg_Ocean_Gain 			= Total_Ocean_Gain 			/ runhour,
							Avg_Ocean_Sp 			= Total_Ocean_Sp 			/ runhour,
							Avg_Ocean_Angle_Diff 	= Total_Ocean_Angle_Diff 	/ runhour,
							Avg_Wind_Sp 			= Total_Wind_Sp*2 			/ runhour,
							Avg_Wind_Angle_Diff 	= Total_Wind_Angle_Diff*2 	/ runhour;

					// check if current position is within target range
					if (norm(curr - dest) < range)
					{ // TODO: use real distance
						printf("\nReached destination\n");
						return true;
					}

					// check if current position is out of bounds --> travel failed
					if (curr.lon() > bounds.lon() || bounds.lat() > curr.lat())
					{ // TODO: generalize
						printf("\nSailed out of bounds\n");
						return false;
					}
				}
			closeCFSR(ouid);
			closeCFSR(ovid);
			closeCFSR(auid);
			closeCFSR(avid);
		}

	return false;
}

// xlswrite("所羅門至斐濟", final);
