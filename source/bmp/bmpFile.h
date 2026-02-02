#ifndef _BMPFILE_H
#define _BMPFILE_H

#include "../type/type.h"
#include <stdio.h>

typedef struct {
    uint8_t  bfType[2];       // 2 bytes: "BM"
    uint32_t bfSize;          // 4 bytes: size of the file in bytes
    uint16_t bfReserved1;    // 4 bytes: reserved; must be 0
    uint16_t bfReserved2;    // 4 bytes: reserved; must be 0
    uint32_t bfOffBits;      // 4 bytes: offset to start of pixel data
} BITMAPFILEHEADER;

typedef struct {
    uint32_t biSize;          // 4 bytes: size of this header (40 bytes)
    int32_t  biWidth;        // 4 bytes: width of the bitmap in pixels
    int32_t  biHeight;       // 4 bytes: height of the bitmap in pixels
    uint16_t biPlanes;       // 2 bytes: number of color planes (must be 1)
    uint16_t biBitCount;    // 2 bytes: bits per pixel (1, 4, 8, 24, or 32)
    uint32_t biCompression; // 4 bytes: compression method (0 = none)
    uint32_t biSizeImage;   // 4 bytes: size of the image data (can be 0 for uncompressed)
    int32_t  biXPelsPerMeter; // 4 bytes: horizontal resolution (pixels per meter)
    int32_t  biYPelsPerMeter; // 4 bytes: vertical resolution (pixels per meter)
    uint32_t biClrUsed;     // 4 bytes: number of colors in the palette (0 = default)
    uint32_t biClrImportant; // 4 bytes: important colors (0 = all)
} BITMAPINFOHEADER;

typedef struct {
    uint32_t biNumColors;     // 4 bytes: number of colors in the color table
    uint8_t *biColorTable;    // pointer to the color table
} BITCOLORTABLE;

typedef struct {
    int32_t biNumPixels;    // number of pixels
    uint8_t *biPixelData;          // pointer to the pixel data
} BIPIXELDATA;

typedef struct {
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER infoHeader;
    BITCOLORTABLE colorTable;
    BIPIXELDATA pixelData;
} bmp_t;

bmp_t* bmpReadFile(char *filename);

#endif
