#ifndef _MATRIX_H
#define _MATRIX_H

#include "../vector/vector.h"
#include <math.h>

typedef struct {
    union {
        struct {
            float M00, M01, M02, M03;
            float M10, M11, M12, M13;
            float M20, M21, M22, M23;
            float M30, M31, M32, M33;
        };
        float M[4][4];
    };

} matrix4x4_t;

matrix4x4_t matrix4x4_identity();
matrix4x4_t matrix4x4_zero();
matrix4x4_t matrix4x4_mult_4x4(matrix4x4_t ma, matrix4x4_t mb);
vector4d_t matrix4x4_mult_4x1(matrix4x4_t ma, vector4d_t mb);
matrix4x4_t matrix4x4_lookAt(vector3d_t eye, vector3d_t target, vector3d_t up);
matrix4x4_t matrix4x4_translate(float x, float y, float z);
matrix4x4_t matrix4x4_scale(float x, float y, float z);
matrix4x4_t matrix4x4_ortho(float left, float right, float bottom, float top, float near, float far);
matrix4x4_t matrix4x4_perspective(float fovY, float aspect, float near, float far);
matrix4x4_t matrix4x4_viewport(int width, int height);

#endif