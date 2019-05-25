#ifndef VEC2_H
#define VEC2_H

typedef struct vec2
{
	double x;
    double y;
} vec2;

#ifdef  __cplusplus
extern  "C" {
#endif

double dot(vec2 a, vec2 b);
double norm(vec2 a);
vec2 normalize(vec2 a);

#ifdef  __cplusplus
}
#endif

#endif
