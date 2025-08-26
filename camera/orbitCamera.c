#include "orbitCamera.h"
#include "../utils/utils.h"
#include <stdlib.h>

static vector3d_t default_up = {0.0f, 1.0f, 0.0f};
static float default_fovY = 60.0f;
static float default_near = 0.1f;
static float default_far = 1000.0f;

orbit_camera_t* create_orbit_camera(vector3d_t eye, vector3d_t target, float aspect) {
    orbit_camera_t* camera = (orbit_camera_t*)malloc(sizeof(orbit_camera_t));
    if (!camera) return NULL;

    // Initialize camera parameters
    camera->eye = eye;
    camera->target = target;
    camera->up = default_up;

    camera->aspect = aspect;
    camera->fovY = default_fovY;
    camera->Near = default_near;
    camera->Far = default_far;

    return camera;
}

void destroy_orbit_camera(orbit_camera_t* camera) {
    if (camera) {
        free(camera);
    }
}

/* set camera status */
void set_orbitCamera(orbit_camera_t* camera, vector3d_t eye, vector3d_t target) {
    if (camera) {
        camera->eye = eye;
        camera->target = target;
        camera->up = default_up;

        camera->fovY = default_fovY;
        camera->Near = default_near;
        camera->Far = default_far;
    }
}

static vector3d_t calculatePanOffset(orbit_camera_t* camera, motion_t* motion) {

    vector3d_t lookAt = vector3d_Sub(camera->target, camera->eye);
    vector3d_t Negative_ZAxis = vector3d_Normalize(lookAt);
    vector3d_t newZAxis = { -Negative_ZAxis.x, -Negative_ZAxis.y, -Negative_ZAxis.z };
    vector3d_t newXAxis = vector3d_Cross(Negative_ZAxis, default_up);
    vector3d_t newYAxis = vector3d_Cross(newZAxis, newXAxis);

    float distance = vector3d_Length(lookAt);
    float factor = distance * (float)tan(degrees_2_radians(camera->fovY) / 2) * 2;
    vector3d_t panOffsetX = vector3d_Mult(newXAxis, factor * motion->pan.x);
    vector3d_t panOffsetY = vector3d_Mult(newYAxis, factor * motion->pan.y);
    return vector3d_Add(panOffsetX, panOffsetY);
}

/* 
    Conversion between spherical coordinate system and Cartesian coordinate system
    see https://zh.wikipedia.org/wiki/%E7%90%83%E5%BA%A7%E6%A8%99%E7%B3%BB 
    Note: Pay attention to the XYZ correspondence between the camera coordinate system 
          and the standard coordinate system
*/
static vector3d_t calculate_OrbitAndDollyOffset(orbit_camera_t* camera, motion_t* motion) {

    vector3d_t lookAt = vector3d_Sub(camera->target, camera->eye);
    
    float radius = vector3d_Length(lookAt);
    float polar = acosf(lookAt.y / radius);
    float azimuth = atan2f(lookAt.x, lookAt.z);

    radius *= powf(0.95, motion->dolly);
    azimuth -= motion->orbit.x * 2 * PI;
    polar -= motion->orbit.y * 2 * PI;
    polar = float_clamp(polar, EPSILON, PI - EPSILON);

    vector3d_t orbitDollyOffset;
    orbitDollyOffset.z = radius * sinf(polar) * cosf(azimuth);
    orbitDollyOffset.y = radius * cosf(polar);
    orbitDollyOffset.x = radius * sinf(polar) * sinf(azimuth);

    return orbitDollyOffset;
}

void update_orbitCamera_status(orbit_camera_t* camera, motion_t* motion) {
    vector3d_t panOffset = calculatePanOffset(camera, motion);
    vector3d_t orbitDollyOffset = calculate_OrbitAndDollyOffset(camera, motion);

    camera->target = vector3d_Add(camera->eye, panOffset);
    camera->eye = vector3d_Add(camera->eye, orbitDollyOffset);
}

/* Get the view matrix and projection matrix */ 
matrix4x4_t get_view_matrix(orbit_camera_t* camera) {
    return matrix4x4_lookAt(camera->eye, camera->target, camera->up);
}

matrix4x4_t get_projection_matrix(orbit_camera_t* camera) {
    return matrix4x4_perspective(camera->fovY, camera->aspect, camera->Near, camera->Far);
}

matrix4x4_t get_view_projection_matrix(orbit_camera_t* camera) {
    matrix4x4_t view = get_view_matrix(camera);
    matrix4x4_t projection = get_projection_matrix(camera);
    return matrix4x4_mult_4x4(projection, view);
}