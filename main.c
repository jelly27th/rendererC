#include <stdio.h>

#include "source/api.h"

#define GRAPHICS_IMPLEMENTATION 1
#define GRAPHICS_LINE_MODE 0
#define GRAPHICS_LIGHTING_MODE 1

int main() {

#if (1 == GRAPHICS_IMPLEMENTATION)
  windows_t* window = initWindow(800, 800);
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

  orbit_camera_t* camera = create_orbit_camera((vector3d_t){0, 12, 5}, (vector3d_t){0, 0, 0}, (float)window->width / (float)window->height);
  record_t record = {0};
  setUserData(window, &record);

  /* load your model */
  mesh_t* mesh = mesh_load("assets\\common\\sphere.obj");

  while (!window->isClose) {

    startFrameRateTickMS(window);

    /* Render your scene here start */
    orbitCamera_update(window, camera, &record);

    matrix4x4_t viewport = matrix4x4_viewport(window->width, window->height);
    matrix4x4_t vp = get_view_projection_matrix(camera);
    matrix4x4_t model = matrix4x4_scale(2.0f, 2.0f, 2.0f);

    memset(framebuffer->color_buffer, 0, sizeof(color_t) * framebuffer->width * framebuffer->height);
    
    int i;
    for ( i= 0; i < mesh->indexCount; i++) {
          //printf("index_count %d: (%d, %d, %d)\n", i, mesh->indices[i*3].vertexIdx, mesh->indices[i*3+1].vertexIdx, mesh->indices[i*3+2].vertexIdx);
          int index1 = mesh->indices[i*3].vertexIdx;
          int index2 = mesh->indices[i*3+1].vertexIdx;
          int index3 = mesh->indices[i*3+2].vertexIdx;
          vector3d_t v0 = mesh->vertices[index1]; 
          vector3d_t v1 = mesh->vertices[index2];
          vector3d_t v2 = mesh->vertices[index3];
          
          color_t color = {255, 255, 255, 255}; // white color

          vector4d_t v0_clip = {v0.x, v0.y, v0.z, 1.0f};
          vector4d_t v1_clip = {v1.x, v1.y, v1.z, 1.0f};
          vector4d_t v2_clip = {v2.x, v2.y, v2.z, 1.0f};
          v0_clip = matrix4x4_mult_4x1(model, v0_clip);
          v1_clip = matrix4x4_mult_4x1(model, v1_clip);
          v2_clip = matrix4x4_mult_4x1(model, v2_clip);

          #if (1 == GRAPHICS_LIGHTING_MODE)
            /* half Lambert model */
            float kd = 1.0f; // Diffuse reflection coefficient
            float ambient = 0.5f; // Ambient light intensity
            vector3d_t light_dir = {0, 10, 10};
            vector3d_t edge1 = vector3d_Sub(vector4d_2_vector3d(v1_clip), vector4d_2_vector3d(v0_clip));
            vector3d_t edge2 = vector3d_Sub(vector4d_2_vector3d(v2_clip), vector4d_2_vector3d(v0_clip));
            light_dir = vector3d_Normalize(light_dir);
            
            vector3d_t normal = vector3d_Normalize(vector3d_Cross(edge2, edge1));
            
            float diff = (vector3d_Dot(light_dir, normal)*0.5) + 0.5;
            diff = ambient + (1.0f - ambient) * diff;

            color.r = (uint8_t)((float)color.r * kd * diff);
            color.g = (uint8_t)((float)color.g * kd * diff);
            color.b = (uint8_t)((float)color.b * kd * diff);

          #endif

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
          #if (1 == GRAPHICS_LINE_MODE)
            line2d_t line1 = {p0, p1};
            line2d_t line2 = {p1, p2};
            line2d_t line3 = {p2, p0};
            draw2d_Line(line1, &color, framebuffer);
            draw2d_Line(line2, &color, framebuffer);
            draw2d_Line(line3, &color, framebuffer);
          #else
            graphics_draw_Triangle(p0, p1, p2, &color, framebuffer);
          #endif
    }

    record.orbit_delta = (vector2d_t){0, 0};
    record.pan_delta = (vector2d_t){0, 0};
    record.dolly_delta = 0;

    /* Render your scene here end */
    drawScreen(window, framebuffer);
    fillScreen(window);

    stopFrameRateTickMS(window);
    
    messageLoop();
  }

  destroyFramebuffer(framebuffer);
  destroy_orbit_camera(camera);
#else
  fastObjMesh* mesh = fast_obj_read("D:\\UID03408\\Desktop\\code\\rendererC\\assets\\Jenny\\Jenny.obj");
  
  printf("index_count: %u\n", mesh->index_count);
  // for (unsigned int i = 0; i < mesh->index_count; i++) {
      unsigned int i = mesh->index_count-3;
      printf("index_count %u: (%d, %d, %d)\n", i, mesh->indices[i].p, mesh->indices[i].t, mesh->indices[i].n);
      // printf("index_count %u: (%d, %d, %d)\n", i+1, mesh->indices[i+1].p, mesh->indices[i+1].t, mesh->indices[i+1].n);
      // printf("index_count %u: (%d, %d, %d)\n", i+2, mesh->indices[i+2].p, mesh->indices[i+2].t, mesh->indices[i+2].n);
  // }

#endif
  return 0;
}