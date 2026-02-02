/*--------------------------------------------------------------------------
--
DIBSECT.C --
Displays a DIB Section in the client area
(c) Charles Petzold, 1998
----------------------------------------------------------------------------
-*/
#include <stdio.h>
#include <windows.h>

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
VOID *pBits = NULL;
RGBQUAD *rgbData = NULL;

void DIB_TEST(HWND hwnd);
void DIB_TEST2(HWND hwnd);
void DIB_TEST3();
HWND hwnd;
HDC hdcMem;
TCHAR szAppName[] = TEXT("DIBsect");
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine,
                   int iCmdShow) {
  MSG msg;
  WNDCLASS wndclass;
  wndclass.style = CS_HREDRAW | CS_VREDRAW;
  wndclass.lpfnWndProc = WndProc;
  wndclass.cbClsExtra = 0;
  wndclass.cbWndExtra = 0;
  wndclass.hInstance = hInstance;
  wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
  wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
  wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
  wndclass.lpszMenuName = szAppName;
  wndclass.lpszClassName = szAppName;
  if (!RegisterClass(&wndclass)) {
    MessageBox(NULL, TEXT("This program requires Windows NT!"), szAppName,
               MB_ICONERROR);
    return 0;
  }
  hwnd = CreateWindow(szAppName, TEXT("DIB Section Display"),
                      WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 800,
                      600, NULL, NULL, hInstance, NULL);
  ShowWindow(hwnd, iCmdShow);
  UpdateWindow(hwnd);

  DIB_TEST(hwnd);
  while (GetMessage(&msg, NULL, 0, 0)) {
    TranslateMessage(&msg);
    DispatchMessage(&msg);
    DIB_TEST3();
    DIB_TEST2(hwnd);
  }
  return msg.wParam;
}

void DIB_TEST(HWND hwnd) {
  // 获取屏幕设备上下文
  HDC hdcScreen = GetDC(hwnd);

  // 创建内存设备上下文
  hdcMem = CreateCompatibleDC(hdcScreen);

  // 定义位图信息
  BITMAPINFO bmi = {0};
  bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
  bmi.bmiHeader.biWidth = 800;   // 位图宽度
  bmi.bmiHeader.biHeight = 600;  // 负值表示从上到下的位图
  bmi.bmiHeader.biPlanes = 1;
  bmi.bmiHeader.biBitCount = 32;         // 32位色深（ARGB）
  bmi.bmiHeader.biCompression = BI_RGB;  // 未压缩格式

  // 创建位图
  // VOID *pBits = NULL;
  HBITMAP hBitmap =
      CreateDIBSection(hdcMem, &bmi, DIB_RGB_COLORS, &pBits, NULL, 0);
  if (hBitmap == NULL) {
    MessageBox(NULL, L"Failed to create DIB section!", L"Error", MB_OK);
    ReleaseDC(NULL, hdcScreen);
    //     DeleteDC(hdcMem);
    return;
  }

  // 将位图内存地址转换为RGBQUAD指针
  // RGBQUAD *rgbData = (RGBQUAD*)pBits;
  rgbData = (RGBQUAD *)pBits;

  // 加载RGB数据到rgbData指向的内存中
  // ... 填充RGB数据 ...
  //  static BYTE red = 0;
  // for (int i = 0; i < 800*600; i++)
  // {
  //     rgbData[i].rgbBlue = 222;
  //     rgbData[i].rgbGreen = 22;
  //     rgbData[i].rgbRed = red;
  //     red++;
  // }

  // 将位图选入内存设备上下文
  SelectObject(hdcMem, hBitmap);

  // 使用BitBlt或StretchBlt将位图绘制到屏幕上
  // BitBlt(hdcScreen, 0, 0, 800, 600, hdcMem, 0, 0, SRCCOPY);

  // 清理资源
  DeleteObject(hBitmap);
  // DeleteDC(hdcMem);
  ReleaseDC(hwnd, hdcScreen);
}

void DIB_TEST2(HWND hwnd) {
  // 获取屏幕设备上下文
  HDC hdcScreen = GetDC(hwnd);

  // 创建内存设备上下文
  //   HDC hdcMem = CreateCompatibleDC(hdcScreen);
  // 使用BitBlt或StretchBlt将位图绘制到屏幕上
  BitBlt(hdcScreen, 0, 0, 800, 600, hdcMem, 0, 0, SRCCOPY);

  //   DeleteDC(hdcMem);
  ReleaseDC(hwnd, hdcScreen);
}

void DIB_TEST3() {
  static BYTE red = 0;
  // rgbData = (RGBQUAD*)pBits;
  for (int i = 0; i < 800 * 600; i++) {
    rgbData[i].rgbBlue = 22;
    rgbData[i].rgbGreen = 22;
    rgbData[i].rgbRed = red;
  }
  printf("red = %d\n", red);
  red++;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam,
                         LPARAM lParam) {
  switch (message) {
    case WM_DESTROY:
      PostQuitMessage(0);
      return 0;
  }
  return DefWindowProc(hwnd, message, wParam, lParam);
}