#include "framebuffer.h"

#include <stdlib.h>

framebuffer_t* createFramebuffer(int width, int height) {
    framebuffer_t *fb = (framebuffer_t *)malloc(sizeof(framebuffer_t));
    if (fb == NULL) {
        return NULL; // Memory allocation failed
    }
    
    fb->width = width;
    fb->height = height;
    fb->color_buffer = (color_t *)malloc(width * height * sizeof(color_t));
    
    if (fb->color_buffer == NULL) {
        free(fb); // Clean up allocated framebuffer
        return NULL; // Memory allocation failed
    }
    
    return fb;
}

void destroyFramebuffer(framebuffer_t *fb) {
    if (fb != NULL) {
        free(fb->color_buffer); // Free the color buffer
        free(fb); // Free the framebuffer structure
    }
}