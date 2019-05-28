#include "vec2.h"
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdio.h>

double dot(vec2 a, vec2 b)
{
	return a.x*b.x + a.y*b.y;
}

double norm(vec2 a)
{
	return sqrt(norm2(a));
}

double norm2(vec2 a)
{
    return dot(a, a);
}

vec2 normalize(vec2 a)
{
    double r = norm(a);
    if (r > 0) {a.x /= r; a.y /= r;}
    return a;
}