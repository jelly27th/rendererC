#ifndef LOG_H
#define LOG_H

#include "../vector/vector.h"
#include <stdio.h>
#include "../point/point.h"
#include "../matrix/matrix.h"

void printMatrix4x4(const matrix4x4_t* matrix);
void printVector2d(const char* str, const vector2d_t* vec);
void printVector3d(const char* str, const vector3d_t* vec);
void printfloat(const char* str, const float* vec);

#endif