#ifndef CFSR_INTERP_H
#define CFSR_INTERP_H

#ifdef  __cplusplus
extern  "C" {
#endif

float bilinearOUV(int ncid, float day, float lat, float lon);
float bilinearAUV(int ncid, float day, float lat, float lon);

// TODO: Barnes interpolation

#ifdef  __cplusplus
}
#endif

#endif
