#ifndef PNG_H
#define PNG_H

#include "../common/type.h"

typedef struct {
    unsigned width, height;
    unsigned char* imageData;
} png_t;

void pngDecode(const char* filename , png_t* png);
void pngFree(png_t* png);

#endif