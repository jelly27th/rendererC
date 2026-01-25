
#include "type.h"
#include <stdio.h>

#define COLOR_TYPE_GRAYSCALE       0
#define COLOR_TYPE_RGB             2
#define COLOR_TYPE_PALETTE         3
#define COLOR_TYPE_GRAYSCALE_ALPHA 4
#define COLOR_TYPE_RGBA            6

typedef struct {
    uint8_t header[8];
} FileHeader;

typedef struct {
    uint32_t length;
    uint8_t type[4];
    uint8_t *data;
    uint32_t crc;
}Chunk_t;

typedef struct {

    uint32_t length;
    uint8_t type[4];

    uint32_t width;
    uint32_t height;
    uint8_t bitDepth;
    uint8_t colorType;
    uint8_t compressionMethod;
    uint8_t filterMethod;
    uint8_t interlaceMethod;
    
    uint32_t crc;
} Chunk_IDHR_t;

typedef struct {
    /* PLTE */
    unsigned char palette[1024];
    uint16_t length;

    /* tRNS */
    uint16_t keyGray;
    uint16_t keyRed;
    uint16_t keyGreen;
    uint16_t keyBlue;
} Chunk_PLTE_tRNS_t;

typedef struct {
    Chunk_IDHR_t IHDR;
    Chunk_PLTE_tRNS_t PLTE_tRNS;
    Chunk_t IDAT;
    Chunk_t IEND;
} CriticalChunk_t;

typedef struct {
    Chunk_t bKGD;
    Chunk_t cHRM;
    Chunk_t cICP;
    Chunk_t dSIG;
    Chunk_t eXIf;
    Chunk_t gAMA;
    Chunk_t hIST;
    Chunk_t iCCP;
    Chunk_t iTXt;
    Chunk_t pHYs;
    Chunk_t sBIT;
    Chunk_t sPLT;
    Chunk_t sRGB;
    Chunk_t sTER;
    Chunk_t tEXt;
    Chunk_t tIME;
    //Chunk_t tRNS;
    Chunk_t zTXt;
} AncillaryChunk_t;

typedef struct {
    FileHeader fileHeader;
    CriticalChunk_t criticalChunks;
    AncillaryChunk_t ancillaryChunks;
} png_t;

png_t* pngReadFile(char *filename);