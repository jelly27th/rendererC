#include "graphics.h"

void drawPoint(point2d_t p0, color_t *color, framebuffer_t* framebuffer) {
    if (p0.x >= 0 && p0.x < framebuffer->width && p0.y >= 0 && p0.y < framebuffer->height) {
        int index = (int)(p0.y * framebuffer->width + p0.x);
        framebuffer->color_buffer[index] = *color;
    }
}