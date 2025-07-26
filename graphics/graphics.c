#include "graphics.h"
#include "../utils/utils.h"
#include <stdbool.h>

void drawPoint(point2d_t start, color_t *color, framebuffer_t* framebuffer) {
    if (start.x >= 0 && start.x < framebuffer->width && start.y >= 0 && start.y < framebuffer->height) {
        int index = (int)(start.y * framebuffer->width + start.x);
        framebuffer->color_buffer[index] = *color;
    }
}

/**
 * see Bresenham's line algorithm from https://www.cs.montana.edu/courses/spring2009/425/dslectures/Bresenham.pdf
 */
void draw2d_line(point2d_t start, point2d_t end, color_t* color, framebuffer_t* framebuffer) {
  float x, y;

  /* vertical line */
  if (0 == (end.x - start.x)) {
    for (x = start.x, y = start.y; y < end.y; y++) {
      point2d_t p = {x, y};
      drawPoint(p, color, framebuffer);
    }
    /* horizontal line */
  } else if (0 == (end.y - start.y)) {
    for (x = start.x, y = start.y; x < end.x; x++) {
      point2d_t p = {x, y};
      drawPoint(p, color, framebuffer);
    }
  } else {
    /* x0>x1 */
    if (start.x > end.x) {
      swap_point2d(&start, &end);
    }
    /* k<0 */
    bool mirror = false;
    if ((end.y - start.y) / (end.x - start.x) < 0) {
      start.y = -start.y;
      end.y = -end.y;
      mirror = true;
    }

    /* k>1 */
    bool steep = false;
    if ((end.y - start.y) / (end.x - start.x) > 1) {
      swap_float(&start.x, &start.y);
      swap_float(&end.x, &end.y);
      steep = true;
    }

    /* x0<x1 0<k<1 */
    int dx = end.x - start.x;
    int dy = end.y - start.y;
    int incrE = 2 * dy;
    int incrNE = 2 * (dy - dx);
    int d = 2 * dy - dx;

    for (x = start.x, y = start.y; x < end.x; x++) {
      if ((true == steep) && (false == mirror)) {
        point2d_t p = {y, x};
        drawPoint(p, color, framebuffer);
      } else if ((false == steep) && (false == mirror)) {
        point2d_t p = {x, y};
        drawPoint(p, color, framebuffer);
      } else if ((false == steep) && (true == mirror)) {
        point2d_t p = {x, -y};
        drawPoint(p, color, framebuffer);
      } else if ((true == steep) && (true == mirror)) {
        point2d_t p = {y, -x};
        drawPoint(p, color, framebuffer);
      }
      if (d < 0) {
        d += incrE;
      } else {
        y++;
        d += incrNE;
      }
    }
  }
}