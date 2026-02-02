#include "maths.h"

/*  for more to see https://en.wikipedia.org/wiki/Barycentric_coordinate_system Edge approach 
from chapter Conversion between barycentric and Cartesian coordinates. */
point3d_t barycentric(point2d_t p, point2d_t v1, point2d_t v2, point2d_t v3) {
    
    float lambda1 = (float)((v2.y - v3.y) * (p.x - v3.x) + (v3.x - v2.x) * (p.y - v3.y)) \
                    / ((v2.y - v3.y) * (v1.x - v3.x) + (v3.x - v2.x) * (v1.y - v3.y));
    
    float lambda2 = (float)((v3.y - v1.y) * (p.x - v3.x) + (v1.x - v3.x) * (p.y - v3.y)) \
                    / ((v2.y - v3.y) * (v1.x - v3.x) + (v3.x - v2.x) * (v1.y - v3.y));
    
    float lambda3 = 1 - lambda1 - lambda2;
    
    point3d_t f = {lambda1, lambda2, lambda3};
    
    return f;
}

float float_min(float a, float b) {
    return (a < b) ? a : b;
}

float float_max(float a, float b) {
    return (a > b) ? a : b;
}