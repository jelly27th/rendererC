/* 
  DIBSECT.C -- Displays a DIB Section in the client area
  https://blog.csdn.net/lijian2017/article/details/139498000
*/
#include "win32.h"

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

TCHAR szAppName[] = TEXT("windows class");

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
  window->width = width;
  window->height = height;
  window->hwnd = NULL;
  window->memoryHdc = NULL;
  window->framebuffer = NULL;
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
    |-------------->x
    |
    |
    |
    v y
 */
void drawScreen(windows_t* window, framebuffer_t* framebuffer) {
  for (int i = 0; i < window->width * window->height; i++) {
    window->framebuffer[i].rgbBlue = framebuffer->color_buffer[i].b;
    window->framebuffer[i].rgbGreen = framebuffer->color_buffer[i].g;
    window->framebuffer[i].rgbRed = framebuffer->color_buffer[i].r;
    window->framebuffer[i].rgbReserved = framebuffer->color_buffer[i].a;
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

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {
  /* for get static variable see https://stackoverflow.com/questions/21369256/how-to-use-wndproc-as-a-class-function */
  windows_t* window = (windows_t*)GetProp(hwnd, szAppName);
  switch (message) {
    case WM_CLOSE:
        window->isClose = true;
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
  }
  return DefWindowProc(hwnd, message, wParam, lParam);
}