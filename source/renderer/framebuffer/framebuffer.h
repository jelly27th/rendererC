#ifndef _FRAMEBUFFER_H
#define _FRAMEBUFFER_H

#include "../../color/color.h"

typedef struct {
    int width, height;
    color_t *color_buffer;
} framebuffer_t;

framebuffer_t* createFramebuffer(int width, int height);
void destroyFramebuffer(framebuffer_t *fb);

#endif