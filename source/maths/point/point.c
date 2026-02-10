#include "point.h"
#include "../maths.h"

point2d_t point2d_min(point2d_t p1, point2d_t p2) {
    point2d_t result;
    result.x = float_min(p1.x, p2.x);
    result.y = float_min(p1.y, p2.y);
    return result;
}

point2d_t point2d_max(point2d_t p1, point2d_t p2) {
    point2d_t result;
    result.x = float_max(p1.x, p2.x);
    result.y = float_max(p1.y, p2.y);
    return result;
}