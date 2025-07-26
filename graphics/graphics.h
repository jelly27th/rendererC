#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include "../point/point.h"
#include "../color/color.h"
#include "../framebuffer/framebuffer.h"

void drawPoint(point2d_t p0, color_t* color, framebuffer_t* framebuffer);
void draw2d_line(point2d_t start, point2d_t end, color_t* color, framebuffer_t* framebuffer);

#endif