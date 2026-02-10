#include "vector.h"
#include <math.h>

vector3d_t vector4d_2_vector3d(vector4d_t v) {
    vector3d_t v3d;
    v3d.x = v.x;
    v3d.y = v.y;
    v3d.z = v.z;
    return v3d;
}

/*                  ----> */
/* create  a vector vbva  */
vector3d_t vector3d_Sub(vector3d_t va, vector3d_t vb) {
    vector3d_t vdiff;

    vdiff.x = va.x - vb.x;
    vdiff.y = va.y - vb.y;
    vdiff.z = va.z - vb.z;

    return(vdiff);
} 

float vector3d_Length(vector3d_t va) {
    return( (float)sqrtf(va.x*va.x + va.y*va.y + va.z*va.z) );
} 

vector3d_t vector3d_Mult(vector3d_t va, float k) {
    vector3d_t vn;
    vn.x = k*va.x;
    vn.y = k*va.y;
    vn.z = k*va.z;
    return  vn;
} 

vector3d_t vector3d_Normalize(vector3d_t va) {

    vector3d_t vn = {0};
    float length = vector3d_Length(va);

    float length_inv = 1.0/length;

    vn = vector3d_Mult(va, length_inv);

    return vn;
}

/*                               
                 /|\ ->  
                  |  C
                  |                   ->              
                  |________________\  b
                 /  ->             /
                /   a
              \/_
*/
vector3d_t vector3d_Cross(vector3d_t a, vector3d_t b) {
    vector3d_t c;

    c.x =  ( (a.y * b.z) - (a.z * b.y) );
    c.y = -( (a.x * b.z) - (a.z * b.x) );
    c.z =  ( (a.x * b.y) - (a.y * b.x) );

    return c;
} 

vector3d_t vector3d_Add(vector3d_t va, vector3d_t vb) {
    vector3d_t vsum;

    vsum.x = va.x + vb.x;
    vsum.y = va.y + vb.y;
    vsum.z = va.z + vb.z;
    return(vsum);
} 

float vector3d_Dot(vector3d_t va, vector3d_t vb) {
    return( (va.x * vb.x) + (va.y * vb.y) + (va.z * vb.z) );
}

vector2d_t vector2d_Add(vector2d_t va, vector2d_t vb) {
    vector2d_t vsum;
    vsum.x = va.x + vb.x;
    vsum.y = va.y + vb.y;
    return(vsum);
} 

vector2d_t vector2d_Sub(vector2d_t va, vector2d_t vb) {
    vector2d_t vdiff;
    vdiff.x = va.x - vb.x;
    vdiff.y = va.y - vb.y;
    return(vdiff);  
} 

vector2d_t vector2d_Mult(float k, vector2d_t va) {
    vector2d_t vscaled;
    vscaled.x = k*va.x;
    vscaled.y = k*va.y;
    return(vscaled);
} 
