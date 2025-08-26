#ifndef _UTILS_H
#define _UTILS_H

#include "../point/point.h"

#define EPSILON 1e-5f

/* https://en.wikipedia.org/wiki/Radian */
#define PI 3.1415926f
#define radians_2_degrees(radians) ((radians) * (180.0 / PI))
#define degrees_2_radians(degrees) ((degrees) * (PI / 180.0))

void swap_float(float *a, float *b);
void swap_point2d(point2d_t *a, point2d_t *b);

float float_clamp(float value, float min, float max);

#endif