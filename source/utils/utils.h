#ifndef _UTILS_H
#define _UTILS_H

#include "../maths/maths.h"
#include "../common/type.h"
#include <stdio.h>

#define EPSILON 1e-5f

/* https://en.wikipedia.org/wiki/Radian */
#define PI 3.1415926f
#define radians_2_degrees(radians) ((radians) * (180.0 / PI))
#define degrees_2_radians(degrees) ((degrees) * (PI / 180.0))

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))

void swap_float(float *a, float *b);
void swap_point2d(point2d_t *a, point2d_t *b);

float float_clamp(float value, float min, float max);

void read_file_bytes(void* buffer, uint32_t size, FILE* fd);

#endif