/* 
  DIBSECT.C -- Displays a DIB Section in the client area
  https://blog.csdn.net/lijian2017/article/details/139498000
*/
#include "win32.h"
#include <direct.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

TCHAR szAppName[] = TEXT("windows class");

void initialize_path(void) {
  #ifdef UNICODE
      wchar_t path[MAX_PATH];
      GetModuleFileName(NULL, path, MAX_PATH);
      *wcsrchr(path, L'\\') = L'\0';
      _wchdir(path);
      _wchdir(L"assets");
  #else
      char path[MAX_PATH];
      GetModuleFileName(NULL, path, MAX_PATH);
      *strrchr(path, '\\') = '\0';
      _chdir(path);
      _chdir("assets");
  #endif
}

void registerClass() {
  WNDCLASS wndclass;
  wndclass.style = CS_HREDRAW | CS_VREDRAW;
  wndclass.lpfnWndProc = WndProc;
  wndclass.cbClsExtra = 0;
  wndclass.cbWndExtra = 0;
  wndclass.hInstance = GetModuleHandle(NULL);
  wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
  wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  wndclass.lpszMenuName = szAppName;
  wndclass.lpszClassName = szAppName;
  if (!RegisterClass(&wndclass)) {
    MessageBox(NULL, TEXT("This program requires Windows NT!"), szAppName, MB_ICONERROR);
  }
  initialize_path();
}

void createWindow(windows_t* window) {
  window->hwnd = CreateWindow(szAppName, TEXT("Renderer"), WS_OVERLAPPEDWINDOW,
                               CW_USEDEFAULT, CW_USEDEFAULT, window->width, window->height, NULL, NULL,
                               GetModuleHandle(NULL), NULL);

  /* for get static variable see https://stackoverflow.com/questions/21369256/how-to-use-wndproc-as-a-class-function */
  SetProp(window->hwnd, szAppName, window);

  ShowWindow(window->hwnd, SW_SHOW);
  UpdateWindow(window->hwnd);
}

windows_t* initWindow(int width, int height) {
  windows_t* window = (windows_t*)malloc(sizeof(windows_t));

  memset(window, 0, sizeof(windows_t)); // Initialize all members to zero
  window->width = width;
  window->height = height;

  window->frameRate = 60; // default frame rate
  window->lastTick = 0;
  
  window->isClose = false;

  return window;
}

void messageLoop() {
  MSG msg;
  // not use GetMessage
  while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
  }
}

void createScreen(windows_t* window) {
  // Get the screen device context
  HDC hdcScreen = GetDC(window->hwnd);

  // Create a memory device context
  window->memoryHdc = CreateCompatibleDC(hdcScreen);

  // Define bitmap information
  BITMAPINFO bmi = {0};
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biWidth = window->width;   // Bitmap width
  bmi.bmiHeader.biHeight = -window->height;  // Negative value indicates a top-down bitmap
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;         // 32-bit color depth (ARGB)
  bmi.bmiHeader.biCompression = BI_RGB;  // Uncompressed format

  // Create bitmap
  HBITMAP hBitmap = CreateDIBSection(window->memoryHdc, &bmi, DIB_RGB_COLORS,(void**)(&window->framebuffer), NULL, 0);

  // Select bitmap into memory device context
  SelectObject(window->memoryHdc, hBitmap);

  // Clean up resources
  DeleteObject(hBitmap);
  ReleaseDC(window->hwnd, hdcScreen);
}

/*
     y ^                       |------------->x   
       |                       |   
       |                -->    |
       |                       |
       |------------>x         v y   
*/
void drawScreen(windows_t* window, framebuffer_t* framebuffer) {
  for (int x = 0; x < window->width; x++) {
    for (int y = 0; y < window->height; y++) {
      int worldPos = window->width * y + x;
      int devicePos = window->width * (window->height - y - 1) + x;
      window->framebuffer[devicePos].rgbBlue = framebuffer->color_buffer[worldPos].b;
      window->framebuffer[devicePos].rgbGreen = framebuffer->color_buffer[worldPos].g;
      window->framebuffer[devicePos].rgbRed = framebuffer->color_buffer[worldPos].r;
      window->framebuffer[devicePos].rgbReserved = framebuffer->color_buffer[worldPos].a;
    }
  }
}

void fillScreen(windows_t* window) {
  // Get the screen device context
  HDC hdcScreen = GetDC(window->hwnd);

  // Create a memory device context
  BitBlt(hdcScreen, 0, 0, window->width, window->height, window->memoryHdc, 0, 0, SRCCOPY);

  ReleaseDC(window->hwnd, hdcScreen);
}

DWORD getTimeMS() {
  return GetTickCount();
}

void setFrameRate(windows_t* window, int fps) {

  window->frameRate = (1.0 / fps) * 1000; // convert to milliseconds
}

void startFrameRateTickMS(windows_t* window) {
  window->lastTick = getTimeMS();
}

void stopFrameRateTickMS(windows_t* window) {
  DWORD currentTick;
  DWORD elapsed;
  do {
    currentTick = getTimeMS();
    elapsed = currentTick - window->lastTick;
  } while (elapsed < window->frameRate);
}

static void handleKeyEvent(windows_t* window, WPARAM key, int action) {
  keycode_t keycode;
  switch (key) {
    case 'A': keycode = KEY_A; break;
    case 'D': keycode = KEY_D; break;
    case 'S': keycode = KEY_S; break;
    case 'W': keycode = KEY_W; break;
    case VK_SPACE: keycode = KEY_SPACE; break;
    default: return; // Unknown key
  }

  window->keys[keycode] = action;
  if (window->callbacks.key_callback) {
    window->callbacks.key_callback(window, keycode, action);
  }
}

static void handleButtonEvent(windows_t* window, button_t button, int action) {

  window->buttons[button] = action;
  if (window->callbacks.button_callback) {
    window->callbacks.button_callback(window, button, action);
  }
}

static void handleScrollEvent(windows_t* window, float offset) {
  if (window->callbacks.scroll_callback) {
    window->callbacks.scroll_callback(window, offset);
  }
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {
  /* for get static variable see https://stackoverflow.com/questions/21369256/how-to-use-wndproc-as-a-class-function */
  windows_t* window = (windows_t*)GetProp(hwnd, szAppName);
  switch (message) {
    case WM_CLOSE:
        window->isClose = true;
        break;
    case WM_LBUTTONDOWN:
      handleButtonEvent(window, BUTTON_L, 1);
      break;
    case WM_LBUTTONUP:
      handleButtonEvent(window, BUTTON_L, 0);
      break;
    case WM_RBUTTONDOWN:
      handleButtonEvent(window, BUTTON_R, 1);
      break;
    case WM_RBUTTONUP:
      handleButtonEvent(window, BUTTON_R, 0);
      break;
    case WM_KEYUP:
      handleKeyEvent(window, wParam, 0);
      break;
    case WM_KEYDOWN:
      handleKeyEvent(window, wParam, 1);
      break;
    case WM_MOUSEWHEEL:
      handleScrollEvent(window, (float)GET_WHEEL_DELTA_WPARAM(wParam) / WHEEL_DELTA);
      break;
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
  }
  return DefWindowProc(hwnd, message, wParam, lParam);
}

int getKeyState(windows_t* window, keycode_t keycode) {
  return window->keys[keycode];
}

int getButtonState(windows_t* window, button_t buttoncode) {
  return window->buttons[buttoncode];
}

POINT getQueryCursorPos(windows_t* window) {
    POINT pos;
    GetCursorPos(&pos);
    ScreenToClient(window->hwnd, &pos);
    return pos;
}

void setUserData(windows_t* window, void* user_data) {
    window->user_data = user_data;
}

void* getUserData(windows_t* window) {
    return window->user_data;
}