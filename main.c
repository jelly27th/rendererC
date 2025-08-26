#include "win32/win32.h"
#include "win32/callback.h"
#include "vector/vector.h"
#include "matrix/matrix.h"
#include "framebuffer/framebuffer.h"
#include "graphics/graphics.h"
#include "pmx/pmxFile.h"
#include "camera/orbitCamera.h"

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

    /* Render your scene here start */
    orbitCamera_update(window, camera, &record);
    // matrix4x4_t camera = matrix4x4_lookAt((vector3d_t){0, 25, 25}, (vector3d_t){0, 5, -10}, (vector3d_t){0, 1, 0});
    // matrix4x4_t projection = matrix4x4_perspective(60.0f, (float)window->width / (float)window->height, 0.1f, 1000.0f);
    matrix4x4_t viewport = matrix4x4_viewport(window->width, window->height);
    matrix4x4_t vp = get_view_projection_matrix(camera);

    for (int i = 0; i < pmx->face.count; i++) {
        for (int j=0; j<3; j++) { 
          int index1 = pmx->face.data[i].indices[j];
          int index2 = pmx->face.data[i].indices[(j+1)%3];
          pmx_vertex_data_t* vertex1 = &pmx->vertex.data[index1];
          pmx_vertex_data_t* vertex2 = &pmx->vertex.data[index2];
          vector3d_t v0 = vertex1->position; 
          vector3d_t v1 = vertex2->position;
          color_t color = {255, 0, 0, 255}; // Red color

          vector4d_t v0_clip = {v0.x, v0.y, v0.z, 1.0f};
          vector4d_t v1_clip = {v1.x, v1.y, v1.z, 1.0f};
          v0_clip = matrix4x4_mult_4x1(vp, v0_clip);
          v1_clip = matrix4x4_mult_4x1(vp, v1_clip);

          vector4d_t v0_ndc = (vector4d_t){v0_clip.x / v0_clip.w, v0_clip.y / v0_clip.w, v0_clip.z / v0_clip.w, 1.0f};
          vector4d_t v1_ndc = (vector4d_t){v1_clip.x / v1_clip.w, v1_clip.y / v1_clip.w, v1_clip.z / v1_clip.w, 1.0f};

          if (v0_ndc.x < -1.0f || v0_ndc.x > 1.0f || v0_ndc.y < -1.0f || v0_ndc.y > 1.0f ||
              v1_ndc.x < -1.0f || v1_ndc.x > 1.0f || v1_ndc.y < -1.0f || v1_ndc.y > 1.0f) {
              continue;
          }
          vector4d_t window_v0 = matrix4x4_mult_4x1(viewport, v0_ndc);
          vector4d_t window_v1 = matrix4x4_mult_4x1(viewport, v1_ndc);

          point2d_t p0 = {ceil(window_v0.x), ceil(window_v0.y)};
          point2d_t p1 = {ceil(window_v1.x), ceil(window_v1.y)};
          line2d_t line = {p0, p1};
          draw2d_Line(line, &color, framebuffer);
      }
    }

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