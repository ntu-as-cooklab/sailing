#include <voyage.hpp>

UV Voyage::adj_direction(LatLon curr, LatLon dest)
{
	return UV(	lon2km((curr+dest).lat()/2) * (dest-curr).lon(),
				lat2km((curr+dest).lat()/2) * (dest-curr).lat()
			).normalize();
}

float Voyage::boat_speed(UV wind, UV dir)
{
	float wind_speed = norm(wind)<8 ? norm(wind) : 0;
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
