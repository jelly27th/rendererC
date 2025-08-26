#ifndef _CALLBACK_H
#define _CALLBACK_H

#include "../vector/vector.h"
#include "../camera/orbitCamera.h"
#include "win32.h"

typedef struct {
    /* orbit */
    int is_orbiting;
    vector2d_t orbit_pos;
    vector2d_t orbit_delta;
    /* pan */
    int is_panning;
    vector2d_t pan_pos;
    vector2d_t pan_delta;
    /* zoom */
    float dolly_delta;
} record_t;

void button_callback(windows_t* window, button_t button, int press);
void scroll_callback(windows_t* window, float offset);
void orbitCamera_update(windows_t* window, orbit_camera_t* camera, record_t* record);

#endif