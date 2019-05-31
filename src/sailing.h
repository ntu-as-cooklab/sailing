#ifndef SAILING_H
#define SAILING_H

#include "vec2.h"
#include "latlon.h"

#ifdef  __cplusplus
extern  "C" {
#endif

vec2 heading2dir(double heading);
double heading(vec2 a);
double anglediff(vec2 dir1, vec2 dir2); // 計算兩角度間的夾角

vec2 adj_direction(latlon_t curr, latlon_t dest);
vec2 calc_sail_gain(vec2 wind, vec2 dir);

#ifdef  __cplusplus
}
#endif

#endif
