#ifndef _POINT_H
#define _POINT_H

typedef struct {
    union {
        struct {
            float x, y;
        };    
        float p[2];
    };
} point2d_t;

typedef struct {
    union {
        struct {
            float x, y, z;
        };
        float p[3];
    };
} point3d_t;

typedef struct {
    union {
        struct {
            float x, y, z, w;
        };
        float p[4];
    };
} point4d_t;

point2d_t point2d_min(point2d_t p1, point2d_t p2);
point2d_t point2d_max(point2d_t p1, point2d_t p2);

#endif