#ifndef _WIN32_H
#define _WIN32_H

#include <stdio.h>
#include <stdbool.h>
#include <windows.h>

#include "../renderer/framebuffer/framebuffer.h"

typedef enum {
  KEY_A,
  KEY_D,
  KEY_S,
  KEY_W,
  KEY_SPACE,
  KEY_NUM
} keycode_t;

typedef enum {
  BUTTON_L,
  BUTTON_R,
  BUTTON_NUM
} button_t;

typedef struct windows windows_t;

typedef struct {
  void (*key_callback)(windows_t *window, keycode_t key, int press);
  void (*button_callback)(windows_t *window, button_t button, int press);
  void (*scroll_callback)(windows_t *window, float offset);
} callbacks_t;

struct windows{
  HWND hwnd;
  HDC memoryHdc;
  int width;
  int height;
  RGBQUAD *framebuffer;

  int frameRate;
  DWORD lastTick;

  int isClose;

  char keys[KEY_NUM];
  char buttons[BUTTON_NUM];
  callbacks_t callbacks;

  void *user_data;
};

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

int getKeyState(windows_t* window, keycode_t keycode);
int getButtonState(windows_t* window, button_t buttoncode);
POINT getQueryCursorPos(windows_t* window);
void setUserData(windows_t* window, void* user_data);
void* getUserData(windows_t* window);

#endif