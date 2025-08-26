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

vector3d_t vector3d_Sub(vector3d_t va, vector3d_t vb);
float vector3d_Length(vector3d_t va);
vector3d_t vector3d_Mult(vector3d_t va, float k);
vector3d_t vector3d_Normalize(vector3d_t va);
vector3d_t vector3d_Cross(vector3d_t a, vector3d_t b);
vector3d_t vector3d_Add(vector3d_t va, vector3d_t vb);

vector2d_t vector2d_Add(vector2d_t va, vector2d_t vb);
vector2d_t vector2d_Sub(vector2d_t va, vector2d_t vb);
vector2d_t vector2d_Mult(float k, vector2d_t va);

#endif