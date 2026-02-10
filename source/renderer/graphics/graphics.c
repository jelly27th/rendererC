#include "graphics.h"
#include "../../utils/utils.h"
#include <stdbool.h>

void draw2d_Point(point2d_t start, color_t *color, framebuffer_t* framebuffer) {
    if (start.x >= 0 && start.x < framebuffer->width && start.y >= 0 && start.y < framebuffer->height) {
        int index = (int)(start.y * framebuffer->width + start.x);
        framebuffer->color_buffer[index] = *color;
    }
}

/**
 * see Bresenham's line algorithm from https://www.cs.montana.edu/courses/spring2009/425/dslectures/Bresenham.pdf
 */
void draw2d_Line(line2d_t line, color_t* color, framebuffer_t* framebuffer) {
  float x, y;

  point2d_t start = line.start;
  point2d_t end = line.end;
  /* vertical line */
  if (0 == (end.x - start.x)) {
    for (x = start.x, y = start.y; y < end.y; y++) {
      point2d_t p = {x, y};
      draw2d_Point(p, color, framebuffer);
    }
    /* horizontal line */
  } else if (0 == (end.y - start.y)) {
    for (x = start.x, y = start.y; x < end.x; x++) {
      point2d_t p = {x, y};
      draw2d_Point(p, color, framebuffer);
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
        draw2d_Point(p, color, framebuffer);
      } else if ((false == steep) && (false == mirror)) {
        point2d_t p = {x, y};
        draw2d_Point(p, color, framebuffer);
      } else if ((false == steep) && (true == mirror)) {
        point2d_t p = {x, -y};
        draw2d_Point(p, color, framebuffer);
      } else if ((true == steep) && (true == mirror)) {
        point2d_t p = {y, -x};
        draw2d_Point(p, color, framebuffer);
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

typedef struct {point2d_t min, max;} Bbox_t;
Bbox_t find_bounding_box(point2d_t v0, point2d_t v1, point2d_t v2) {
  Bbox_t box;
  box.min = point2d_min(point2d_min(v0, v1), v2);
  box.max = point2d_max(point2d_max(v0, v1), v2);
  return box;
}

void graphics_draw_Triangle(point2d_t p0, point2d_t p1, point2d_t p2, color_t* color, framebuffer_t* framebuffer) {

  // find bounding box AABB（Axis-Aligned Bounding Boxes)
  Bbox_t AABB_box = find_bounding_box(p0, p1, p2);

  // raster triangles
  for (float x = AABB_box.min.x; x < AABB_box.max.x; x++) {
    for (float y = AABB_box.min.y; y < AABB_box.max.y; y++) {
      point2d_t p = {x, y};

      point3d_t barycoord = barycentric(p, p0, p1, p2);
      // handling accuracy issues. 
      // if `-0.01` is `0`, maybe some point from model will discard in rendering.
      if (barycoord.x < -0.01|| barycoord.y < -0.01 || barycoord.z < -0.01) {
        continue; 
      }
      draw2d_Point(p, color, framebuffer);
    }
  }
}