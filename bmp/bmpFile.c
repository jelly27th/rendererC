#include "bmp.h"

static void bmpRead(void* buffer, uint32_t size, FILE* fd) {
    memset(buffer, 0, size);
    fread(buffer, size, 1, fd);
}

static void bmpReadFileHeader(bmp_t* bmp, FILE* fd) {

    BITMAPFILEHEADER* fileHeader = &(bmp->fileHeader);
    bmpRead(&(fileHeader->bfType[0]), sizeof(fileHeader->bfType), fd);
    bmpRead(&(fileHeader->bfSize), sizeof(fileHeader->bfSize), fd);
    bmpRead(&(fileHeader->bfReserved1), sizeof(fileHeader->bfReserved1), fd);
    bmpRead(&(fileHeader->bfReserved2), sizeof(fileHeader->bfReserved2), fd);
    bmpRead(&(fileHeader->bfOffBits), sizeof(fileHeader->bfOffBits), fd);
}

static void bmpReadInfoHeader(bmp_t* bmp, FILE* fd) {

    BITMAPINFOHEADER* infoHeader = &(bmp->infoHeader);
    bmpRead(&(infoHeader->biSize), sizeof(infoHeader->biSize), fd);
    bmpRead(&(infoHeader->biWidth), sizeof(infoHeader->biWidth), fd);
    bmpRead(&(infoHeader->biHeight), sizeof(infoHeader->biHeight), fd);
    bmpRead(&(infoHeader->biPlanes), sizeof(infoHeader->biPlanes), fd);
    bmpRead(&(infoHeader->biBitCount), sizeof(infoHeader->biBitCount), fd);
    bmpRead(&(infoHeader->biCompression), sizeof(infoHeader->biCompression), fd);
    bmpRead(&(infoHeader->biSizeImage), sizeof(infoHeader->biSizeImage), fd);
    bmpRead(&(infoHeader->biXPelsPerMeter), sizeof(infoHeader->biXPelsPerMeter), fd);
    bmpRead(&(infoHeader->biYPelsPerMeter), sizeof(infoHeader->biYPelsPerMeter), fd);
    bmpRead(&(infoHeader->biClrUsed), sizeof(infoHeader->biClrUsed), fd);
    bmpRead(&(infoHeader->biClrImportant), sizeof(infoHeader->biClrImportant), fd);
}

static void bmpReadColorTable(bmp_t* bmp, FILE* fd) {

    BITMAPINFOHEADER* infoHeader = &(bmp->infoHeader);
    BITCOLORTABLE* colorTable = &(bmp->colorTable);

    if (infoHeader->biBitCount <= 8) {
        colorTable->biNumColors = 1 << infoHeader->biBitCount; // 2^n colors
        colorTable->biColorTable = (uint8_t*)malloc(colorTable->biNumColors * 4); // each color is 4 bytes (RGBA)
        bmpRead(colorTable->biColorTable, colorTable->biNumColors * 4, fd);
    } else {
        colorTable->biNumColors = 0;
        colorTable->biColorTable = NULL;
    }

}

static void bmpReadPixelData(bmp_t* bmp, FILE* fd) {

    BITMAPINFOHEADER* infoHeader = &(bmp->infoHeader);
    BIPIXELDATA* pixelData = &(bmp->pixelData);
    
    // row size (padded to multiple of 4 bytes)
    uint32_t rowSizeUnpadded = infoHeader->biWidth * (infoHeader->biBitCount / 8);
    uint32_t paddingSize = 4 - (rowSizeUnpadded % 4);
    uint32_t rowSize = rowSizeUnpadded + paddingSize;
    uint32_t dataSize = rowSize * abs(infoHeader->biHeight); // total pixel data size

    pixelData->biNumPixels = infoHeader->biWidth * abs(infoHeader->biHeight);
    pixelData->biPixelData = (uint8_t*)malloc(sizeof(uint8_t) * dataSize);
    bmpRead(pixelData->biPixelData, dataSize, fd);
}

/* for more information on BMP files, see: https://geocld.github.io/2021/03/02/bmp/ */
bmp_t* bmpReadFile(char *filename) {

    FILE *fd = fopen(filename,"rb"); 
    if(fd == NULL) {
        perror("open failed!");
        exit(1);        //error, exit the program
    }

    bmp_t* bmp = (bmp_t*)malloc(sizeof(bmp_t));
    bmpReadFileHeader(bmp, fd);
    bmpReadInfoHeader(bmp, fd);
    bmpReadColorTable(bmp, fd);
    bmpReadPixelData(bmp, fd);

    fclose(fd);
    return bmp;
}