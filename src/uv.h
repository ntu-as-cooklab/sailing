#ifndef UV_H
#define UV_H

#include "vec2.h"

#ifdef  __cplusplus
extern  "C" {
#endif

double heading(vec2 a);
double anglediff(vec2 dir1, vec2 dir2); // 計算兩角度間的夾角
double lat2km(double lon); // 計算該緯度的每一緯度應該是多少距離(km)
double lon2km(double lat); // 計算該緯度的每一經度應該是多少距離(km)

#ifdef  __cplusplus
}
#endif

#endif
