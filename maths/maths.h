#ifndef _MATHS_H
#define _MATHS_H

#include "../point/point.h"

point3d_t barycentric(point2d_t p, point2d_t v1, point2d_t v2, point2d_t v3);

float float_min(float a, float b);
float float_max(float a, float b);

#endif