#ifndef _ORBITCAMERA_H
#define _ORBITCAMERA_H

#include "../maths/maths.h"

typedef struct {
    vector2d_t orbit;
    vector2d_t pan;
    float dolly;
} motion_t;

typedef struct {
    vector3d_t eye;
    vector3d_t target;
    vector3d_t up;

    float aspect;
    float fovY;
    float Near;
    float Far;
} orbit_camera_t;

orbit_camera_t* create_orbit_camera(vector3d_t eye, vector3d_t target, float aspect);
void destroy_orbit_camera(orbit_camera_t* camera);

void set_orbitCamera(orbit_camera_t* camera, vector3d_t eye, vector3d_t target);
void update_orbitCamera_status(orbit_camera_t* camera, motion_t* motion);

matrix4x4_t get_view_matrix(orbit_camera_t* camera);
matrix4x4_t get_projection_matrix(orbit_camera_t* camera);
matrix4x4_t get_view_projection_matrix(orbit_camera_t* camera);

#endif