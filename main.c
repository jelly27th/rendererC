#include "win32/win32.h"
#include "framebuffer/framebuffer.h"
#include "graphics/graphics.h"


int main() {
  
  windows_t* window = initWindow(800, 600);
  if (window == NULL) {
    return 0;
  }
  framebuffer_t* framebuffer = createFramebuffer(window->width, window->height);

  registerClass();
  createWindow(window);
  createScreen(window);
  setFrameRate(window, 60);
  while (!window->isClose) {

    startFrameRateTickMS(window);

    /* Render your scene here start */
    color_t color = {255, 0, 0, 255}; // Red color
    for (int i = 0; i < framebuffer->width; i++) {
      for (int j = 0; j < framebuffer->height; j++) {
        point2d_t p = {i, 1};
        drawPoint(p, &color, framebuffer);
      }
    }
    /* Render your scene here end */
    drawScreen(window, framebuffer);
    fillScreen(window);

    stopFrameRateTickMS(window);
    
    messageLoop();
  }

  destroyFramebuffer(framebuffer);

  return 0;
}