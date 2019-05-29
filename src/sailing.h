#ifndef SAILING_H
#define SAILING_H

#include "vec2.h"

#ifdef  __cplusplus
extern  "C" {
#endif

double heading(vec2 a);
double anglediff(vec2 dir1, vec2 dir2); // 計算兩角度間的夾角

#ifdef  __cplusplus
}
#endif

#endif
