#include "vector.h"

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