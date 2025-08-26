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
