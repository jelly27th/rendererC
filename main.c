#include "win32/win32.h"
#include "win32/callback.h"
#include "vector/vector.h"
#include "matrix/matrix.h"
#include "framebuffer/framebuffer.h"
#include "graphics/graphics.h"
#include "pmx/pmxFile.h"
#include "camera/orbitCamera.h"
#include "log/log.h"

int main() {
  
  windows_t* window = initWindow(700, 700);
  if (window == NULL) {
    return 0;
  }
  window->callbacks.button_callback = button_callback;
  window->callbacks.scroll_callback = scroll_callback;
  window->callbacks.key_callback = NULL; // No key callback for now
  
  framebuffer_t* framebuffer = createFramebuffer(window->width, window->height);

  registerClass();
  createWindow(window);
  createScreen(window);
  setFrameRate(window, 60);

  orbit_camera_t* camera = create_orbit_camera((vector3d_t){0, 25, 25}, (vector3d_t){0, 5, -10}, (float)window->width / (float)window->height);
  record_t record = {0};
  setUserData(window, &record);

  pmx_t* pmx = pmxReadFile("xiao\\xiao.pmx");
  while (!window->isClose) {

    startFrameRateTickMS(window);

    #if 1
    /* Render your scene here start */
    orbitCamera_update(window, camera, &record);

    matrix4x4_t viewport = matrix4x4_viewport(window->width, window->height);
    matrix4x4_t vp = get_view_projection_matrix(camera);

    memset(framebuffer->color_buffer, 0, sizeof(color_t) * framebuffer->width * framebuffer->height);
    
    for (int i = 0; i < pmx->face.count; i++) {
          int index1 = pmx->face.data[i].indices[0];
          int index2 = pmx->face.data[i].indices[1];
          int index3 = pmx->face.data[i].indices[2];
          pmx_vertex_data_t* vertex1 = &pmx->vertex.data[index1];
          pmx_vertex_data_t* vertex2 = &pmx->vertex.data[index2];
          pmx_vertex_data_t* vertex3 = &pmx->vertex.data[index3];
          vector3d_t v0 = vertex1->position; 
          vector3d_t v1 = vertex2->position;
          vector3d_t v2 = vertex3->position;
          color_t color = {255, 0, 0, 255}; // Red color

          vector4d_t v0_clip = {v0.x, v0.y, v0.z, 1.0f};
          vector4d_t v1_clip = {v1.x, v1.y, v1.z, 1.0f};
          vector4d_t v2_clip = {v2.x, v2.y, v2.z, 1.0f};
          v0_clip = matrix4x4_mult_4x1(vp, v0_clip);
          v1_clip = matrix4x4_mult_4x1(vp, v1_clip);
          v2_clip = matrix4x4_mult_4x1(vp, v2_clip);

          vector4d_t v0_ndc = (vector4d_t){v0_clip.x / v0_clip.w, v0_clip.y / v0_clip.w, v0_clip.z / v0_clip.w, 1.0f};
          vector4d_t v1_ndc = (vector4d_t){v1_clip.x / v1_clip.w, v1_clip.y / v1_clip.w, v1_clip.z / v1_clip.w, 1.0f};
          vector4d_t v2_ndc = (vector4d_t){v2_clip.x / v2_clip.w, v2_clip.y / v2_clip.w, v2_clip.z / v2_clip.w, 1.0f};

          if (v0_ndc.x < -1.0f || v0_ndc.x > 1.0f || v0_ndc.y < -1.0f || v0_ndc.y > 1.0f ||
              v1_ndc.x < -1.0f || v1_ndc.x > 1.0f || v1_ndc.y < -1.0f || v1_ndc.y > 1.0f ||
              v2_ndc.x < -1.0f || v2_ndc.x > 1.0f || v2_ndc.y < -1.0f || v2_ndc.y > 1.0f) {
              continue;
          }
          vector4d_t window_v0 = matrix4x4_mult_4x1(viewport, v0_ndc);
          vector4d_t window_v1 = matrix4x4_mult_4x1(viewport, v1_ndc);
          vector4d_t window_v2 = matrix4x4_mult_4x1(viewport, v2_ndc);

          point2d_t p0 = {ceil(window_v0.x), ceil(window_v0.y)};
          point2d_t p1 = {ceil(window_v1.x), ceil(window_v1.y)};
          point2d_t p2 = {ceil(window_v2.x), ceil(window_v2.y)};
          graphics_draw_Triangle(p0, p1, p2, &color, framebuffer);
    }

    record.orbit_delta = (vector2d_t){0, 0};
    record.pan_delta = (vector2d_t){0, 0};
    record.dolly_delta = 0;
    #else

    point2d_t p0 = {100, 100};
    point2d_t p1 = {500, 300};
    point2d_t p2 = {300, 500};
    color_t color = {0, 255, 0, 255}; // Green color
    graphics_draw_Triangle(p0, p1, p2, &color, framebuffer);
    #endif
    /* Render your scene here end */
    drawScreen(window, framebuffer);
    fillScreen(window);

    stopFrameRateTickMS(window);
    
    messageLoop();
  }

  destroyFramebuffer(framebuffer);
  destroy_orbit_camera(camera);

  return 0;
}