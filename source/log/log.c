#include "log.h"

void printMatrix4x4(const matrix4x4_t* matrix) {
    printf("Matrix 4x4:\n");
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            printf("%f ", matrix->M[i][j]);
        }
        printf("\n");
    }
}

void printVector3d(const char* str, const vector3d_t* vec) {
    printf("%s: (%f, %f, %f)\n", str, vec->x, vec->y, vec->z);
}

void printVector2d(const char* str, const vector2d_t* vec) {
    printf("%s: (%f, %f)\n", str, vec->x, vec->y);
}

void printfloat(const char* str, const float* vec) {
    printf("%s: %f\n", str, *vec);
}