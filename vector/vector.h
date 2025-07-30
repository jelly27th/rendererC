#ifndef _VECTOR_H
#define _VECTOR_H

typedef struct {
    union {
        struct {
            float x, y;
        };    
        float M[2];
    };
} vector2d_t;

typedef struct {
    union {
        struct {
            float x, y, z;
        };
        float M[3];
    };
} vector3d_t;

typedef struct {
    union {
        struct {
            float x, y, z, w;
        };
        float M[4];
    };
} vector4d_t;

#endif