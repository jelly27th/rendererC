#ifndef _GRAPHICS_H
#define _GRAPHICS_H

#include "../point/point.h"
#include "../line/line.h"
#include "../color/color.h"
#include "../framebuffer/framebuffer.h"

void draw2d_Point(point2d_t p0, color_t* color, framebuffer_t* framebuffer);
void draw2d_Line(line2d_t line, color_t* color, framebuffer_t* framebuffer);
void graphics_draw_Triangle(point2d_t p0, point2d_t p1, point2d_t p2, color_t* color, framebuffer_t* framebuffer);

#endif