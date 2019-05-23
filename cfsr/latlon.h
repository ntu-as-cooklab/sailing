#ifndef LATLON_H
#define LATLON_H

typedef struct latlon_t
{
    double lat;
    double lon;
} latlon_t;

#ifdef  __cplusplus
extern  "C" {
#endif

double lat2km(double lon); // 計算該緯度的每一緯度應該是多少距離(km)
double lon2km(double lat); // 計算該緯度的每一經度應該是多少距離(km)

#ifdef  __cplusplus
}
#endif

#endif