#ifndef _WIN32_H
#define _WIN32_H

#include <stdio.h>
#include <stdbool.h>
#include <windows.h>

#include "../framebuffer/framebuffer.h"

typedef struct {
  HWND hwnd;
  HDC memoryHdc;
  int width;
  int height;
  RGBQUAD *framebuffer;

  int frameRate;
  DWORD lastTick;

  int isClose;
} windows_t;

windows_t *initWindow(int width, int height);
void registerClass();
void createWindow(windows_t *window);
void createScreen(windows_t *window);
void drawScreen(windows_t *window, framebuffer_t* framebuffer);
void fillScreen(windows_t *window);
DWORD getTimeMS();
void setFrameRate(windows_t *window, int fps);
void startFrameRateTickMS(windows_t *window);
void stopFrameRateTickMS(windows_t *window);
void messageLoop();
LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);


#endif