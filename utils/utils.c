#include "utils.h"

void swap_float(float* a, float* b) {
  float tmp = *a;
  *a = *b;
  *b = tmp;
}

void swap_point2d(point2d_t* a, point2d_t* b) {
  point2d_t tmp = *a;
  *a = *b;
  *b = tmp;
}

float float_clamp(float value, float min, float max) {
  return value < min ? min : (value > max ? max : value);
}