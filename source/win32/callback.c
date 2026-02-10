#include "callback.h"
#include "../log/log.h"

vector2d_t get_posDelta(windows_t* window, vector2d_t start, vector2d_t end) {
    vector2d_t delta;
    float k = 1.0f / (float)window->height;
    delta.x = end.x - start.x;
    delta.y = end.y - start.y;
    return vector2d_Mult(k, delta);
}

vector2d_t get_cursorPos(windows_t* window) {
    POINT pos = getQueryCursorPos(window);
    vector2d_t cursor_pos = { (float)pos.x, (float)pos.y };
    return cursor_pos;
}

void button_callback(windows_t* window, button_t button, int press) {

    record_t* record = (record_t*)getUserData(window);
    vector2d_t cursor_pos = get_cursorPos(window);
    if (button == BUTTON_L) {
        if (press) {
            record->is_orbiting = 1;
            record->orbit_pos = cursor_pos;
        } else {
            vector2d_t posDelta = get_posDelta(window, record->orbit_pos, cursor_pos);
            record->orbit_delta = vector2d_Add(record->orbit_delta, posDelta);
            record->is_orbiting = 0;
        }
    } else if (button == BUTTON_R) {
        if (press) {
            record->is_panning = 1;
            record->pan_pos = cursor_pos;
        } else {
            vector2d_t posDelta = get_posDelta(window, record->pan_pos, cursor_pos);
            record->pan_delta = vector2d_Add(record->pan_delta, posDelta);
            record->is_panning = 0;
        }
    }
}

void scroll_callback(windows_t* window, float offset) {
    record_t* record = (record_t*)getUserData(window);
    record->dolly_delta += offset;
}

void orbitCamera_update(windows_t* window, orbit_camera_t* camera, record_t* record) {

    vector2d_t cursor_pos = get_cursorPos(window);
    if (record->is_orbiting) {
        vector2d_t posDelta = get_posDelta(window, record->orbit_pos, cursor_pos);
        record->orbit_delta = vector2d_Add(record->orbit_delta, posDelta);
        record->orbit_pos = cursor_pos;
    }

    if (record->is_panning) {
        vector2d_t posDelta = get_posDelta(window, record->pan_pos, cursor_pos);
        record->pan_delta = vector2d_Add(record->pan_delta, posDelta);
        record->pan_pos = cursor_pos;
    }

    if (getKeyState(window, KEY_SPACE)) {
        set_orbitCamera(camera, (vector3d_t){0, 12, 5}, (vector3d_t){0, 0, 0});
    } else {
        motion_t motion = {0};
        motion.orbit = record->orbit_delta;
        motion.pan = record->pan_delta;
        motion.dolly = record->dolly_delta;
        update_orbitCamera_status(camera, &motion);
    }
}