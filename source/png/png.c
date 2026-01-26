#include "png.h"
#include <string.h>
#include <stdlib.h>
#include "../utils/utils.h"
#include "../dyarray/dyarray.h"

/* https://en.wikipedia.org/wiki/PNG */
/* https://www.libpng.org/pub/png/spec/1.2/PNG-Chunks.html */

UseVector(uint8_t);

static void pngRead(void* buffer, uint32_t size, FILE* fd) {
    memset(buffer, 0, size);
    fread(buffer, size, 1, fd);
}

static void pngReadUint32(void* buffer, uint32_t size, FILE* fd) {
    uint8_t bytes[4] = {0}; /* chunk length is 4 bytes */
    uint32_t value = 0;

    memset(buffer, 0, size);
    fread(bytes, size, 1, fd);

    /* convert big-endian to little-endian */
    value = (bytes[0] << 24) | (bytes[1] << 16) | (bytes[2] << 8) | bytes[3];
    (*(uint32_t*)buffer) = value;
}

static void pngReadFileHeader(png_t* png, FILE* fd) {

    FileHeader* fileHeader = &(png->fileHeader);
    pngRead(&(fileHeader->header[0]), sizeof(fileHeader->header), fd);
}

/* RGBA 8bits */
static void pngReadIDAT(Chunk_t* IDAT, Chunk_t* chunk) {

    IDAT->length += chunk->length;
    for (int i = 0; i < 4; i++) {
        IDAT->type[i] = chunk->type[i];
    }
    if (IDAT->data == NULL) {
        IDAT->data = (uint8_t*)malloc(chunk->length);
        memcpy(IDAT->data, chunk->data, chunk->length);
    } else {
        IDAT->data = (uint8_t*)realloc(IDAT->data, IDAT->length);
        memcpy(IDAT->data + (IDAT->length - chunk->length), chunk->data, chunk->length);
    }

}

static void pngReadIHDR(Chunk_IDHR_t* IHDR, Chunk_t* chunk) {

    IHDR->length = chunk->length;
    for (int i = 0; i < 4; i++) {
        IHDR->type[i] = chunk->type[i];
    }

    IHDR->width = (uint32_t)(chunk->data[0] << 24) | (chunk->data[1] << 16) | (chunk->data[2] << 8) | chunk->data[3];
    IHDR->height = (uint32_t)(chunk->data[4] << 24) | (chunk->data[5] << 16) | (chunk->data[6] << 8) | chunk->data[7];
    IHDR->bitDepth = chunk->data[8];
    IHDR->colorType = chunk->data[9];
    IHDR->compressionMethod = chunk->data[10];
    IHDR->filterMethod = chunk->data[11];
    IHDR->interlaceMethod = chunk->data[12];

    IHDR->crc = chunk->crc;
}

static void pngReadPLTE(Chunk_PLTE_tRNS_t* PLTE, Chunk_t* chunk) {

    /* initial */
    for (uint16_t i = 0;  i < 256; i++) {
        PLTE->palette[i * 4 + 0] = 0;
        PLTE->palette[i * 4 + 1] = 0;
        PLTE->palette[i * 4 + 2] = 0;
        PLTE->palette[i * 4 + 3] = 255;
    }
    PLTE->length = chunk->length / 3;

    for (uint16_t i = 0; i < PLTE->length; i++) {
        PLTE->palette[i * 4 + 0] = chunk->data[i * 3 + 0]; /* R */
        PLTE->palette[i * 4 + 1] = chunk->data[i * 3 + 1]; /* G */
        PLTE->palette[i * 4 + 2] = chunk->data[i * 3 + 2]; /* B */
        PLTE->palette[i * 4 + 3] = 255;                    /* A */
    }

}

static void pngReadtRNS(CriticalChunk_t* CriticalChunk, Chunk_t* chunk) {

    Chunk_PLTE_tRNS_t* PLTE_tRNS = &(CriticalChunk->PLTE_tRNS);
    Chunk_IDHR_t* IHDR = &(CriticalChunk->IHDR);

    if (COLOR_TYPE_PALETTE == IHDR->colorType) {
        for (uint16_t i = 0; i < chunk->length; i++) {
            PLTE_tRNS->palette[i * 4 + 3] = chunk->data[i]; /* A */
        }
    } else if (COLOR_TYPE_GRAYSCALE == IHDR->colorType) {
        PLTE_tRNS->keyGray = (uint16_t)(chunk->data[0] << 8) | chunk->data[1];
    } else if (COLOR_TYPE_RGB == IHDR->colorType) {
        PLTE_tRNS->keyRed = (uint16_t)(chunk->data[0] << 8) | chunk->data[1];
        PLTE_tRNS->keyGreen = (uint16_t)(chunk->data[2] << 8) | chunk->data[3];
        PLTE_tRNS->keyBlue = (uint16_t)(chunk->data[4] << 8) | chunk->data[5];
    }
}

typedef enum {
  LCT_GREY = 0, /*grayscale: 1,2,4,8,16 bit*/
  LCT_RGB = 2, /*RGB: 8,16 bit*/
  LCT_PALETTE = 3, /*palette: 1,2,4,8 bit*/
  LCT_GREY_ALPHA = 4, /*grayscale with alpha: 8,16 bit*/
  LCT_RGBA = 6, /*RGB with alpha: 8,16 bit*/
  LCT_MAX_OCTET_VALUE = 255 /* invalid color type */
} pngColorType;

static unsigned pngGetNumColorChannels(uint8_t colortype) {
  switch(colortype) {
    case LCT_GREY: return 1;
    case LCT_RGB: return 3;
    case LCT_PALETTE: return 1;
    case LCT_GREY_ALPHA: return 2;
    case LCT_RGBA: return 4;
    case LCT_MAX_OCTET_VALUE: return 0; /* invalid color type */
    default: return 0; /*invalid color type*/
  }
}

static uint8_t pngGetbpp(Chunk_IDHR_t* IHDR) {
    return pngGetNumColorChannels(IHDR->colorType) * IHDR->bitDepth; 
}

/* in an idat chunk, each scanline is a multiple of 8 bits */
static uint32_t pngGetRawSizeIdat(unsigned w, unsigned h, unsigned bpp) {
  /* + 1 for the filter byte, and possibly plus padding bits per line. */
  /* Ignoring casts, the expression is equal to (w * bpp + 7) / 8 + 1, but avoids overflow of w * bpp */
  uint32_t line = ((uint32_t)(w / 8u) * bpp) + 1u + ((w & 7u) * bpp + 7u) / 8u;
  return (uint32_t)h * line;
}

static uint32_t pngCalRawIDATSize(png_t* png) {

    Chunk_IDHR_t* IHDR = &(png->criticalChunks).IHDR;

    uint8_t bpp = pngGetbpp(IHDR);
    uint32_t expectedSize = 0;
    if (0 == IHDR->interlaceMethod) {
        /* no interlaced */
        expectedSize = pngGetRawSizeIdat(IHDR->width, IHDR->height, bpp);
    } else {
        /* Adam-7 interlaced */
        expectedSize += pngGetRawSizeIdat((IHDR->width + 7) >> 3, (IHDR->height + 7) >> 3, bpp);
        if(IHDR->width > 4) expectedSize += pngGetRawSizeIdat((IHDR->width + 3) >> 3, (IHDR->height + 7) >> 3, bpp);
        expectedSize += pngGetRawSizeIdat((IHDR->width + 3) >> 2, (IHDR->height + 3) >> 3, bpp);
        if(IHDR->width > 2) expectedSize += pngGetRawSizeIdat((IHDR->width + 1) >> 2, (IHDR->height + 3) >> 2, bpp);
        expectedSize += pngGetRawSizeIdat((IHDR->width + 1) >> 1, (IHDR->height + 1) >> 2, bpp);
        if(IHDR->width > 1) expectedSize += pngGetRawSizeIdat((IHDR->width + 0) >> 1, (IHDR->height + 1) >> 1, bpp);
        expectedSize += pngGetRawSizeIdat((IHDR->width + 0), (IHDR->height + 0) >> 1, bpp);
    }

    return expectedSize;
}

typedef struct {
  const uint8_t* data;
  uint64_t size; /*size of data in bytes*/
  uint64_t bitsize; /*size of data in bits, end of valid bp values, should be 8*size*/
  uint64_t bp; /* bit pointer in the data */
  unsigned buffer; /*buffer for reading bits. NOTE: 'unsigned' must support at least 32 bits*/
} pngBitReader;

static pngBitReader* LodePNGBitReader_init(const uint8_t* data, uint64_t size) {
    pngBitReader *reader = (pngBitReader*)malloc(sizeof(pngBitReader));
    reader->data = data;
    reader->size = size;
    reader->bitsize = size * 8u;
    reader->bp = 0;
    reader->buffer = 0;
    return reader;
}

static void pngBitReader_cleanup(pngBitReader* reader) {
    free(reader);
}

static void pngEnsureBits9(pngBitReader* reader) {
  size_t start = reader->bp >> 3u;
  size_t size = reader->size;
  if(start + 1u < size) {
    reader->buffer = (unsigned)reader->data[start + 0] | ((unsigned)reader->data[start + 1] << 8u);
    reader->buffer >>= (reader->bp & 7u);
  } else {
    reader->buffer = 0;
    if(start + 0u < size) reader->buffer = reader->data[start + 0];
    reader->buffer >>= (reader->bp & 7u);
  }
}

static unsigned pngPeekBits(pngBitReader* reader, uint8_t nbits) {
  return reader->buffer & (((unsigned)1u << nbits) - 1u);
}

static void pngSkipBits(pngBitReader* reader, uint8_t nbits) {
  reader->bp += nbits;
  reader->buffer >>= nbits;
}

static unsigned pngReadBits(pngBitReader* reader, uint8_t nbits) {
  unsigned result = pngPeekBits(reader, nbits);
  pngSkipBits(reader, nbits);
  return result;
}

static unsigned pngReverseBits(unsigned bits, unsigned num) {
  unsigned i, result = 0;
  for(i = 0; i < num; i++) result |= ((bits >> (num - i - 1u)) & 1u) << i;
  return result;
}

/* 
    no compression block format
    0   1   2   3   4...
   +---+---+---+---+================================+
   |  LEN  | NLEN  |... LEN bytes of literal data...|
   +---+---+---+---+================================+

*/
static void pngInflateNoCompression(vector_uint8_t* out, pngBitReader* bitReader, png_t* png) {

    uint32_t bytePos = (bitReader->bp + 7) >> 3; /*go to first boundary of byte*/

    uint16_t LEN = (uint16_t)(bitReader->data[bytePos] | (bitReader->data[bytePos + 1] << 8));
    uint16_t NLEN = (uint16_t)(bitReader->data[bytePos + 2] | (bitReader->data[bytePos + 3] << 8));
    bytePos += 4;

    if ((LEN + out->len) > out->cap) {
        vector_uint8_t_realloc(out, LEN + out->len);
    }

    if (LEN) {
        memcpy(out->data + out->len, bitReader->data + bytePos, LEN);
        bytePos += LEN;
    }

    bitReader->bp = bytePos << 3; /*update bit pointer*/
}

typedef struct HuffmanTree {
  unsigned* codes; /*the huffman codes (bit patterns representing the symbols)*/
  unsigned* lengths; /*the lengths of the huffman codes*/
  unsigned maxbitlen; /*maximum number of bits a single code can get*/
  unsigned numcodes; /*number of symbols in the alphabet = number of codes*/
  /* for reading only */
  unsigned char* table_len; /*length of symbol from lookup table, or max length if secondary lookup needed*/
  unsigned short* table_value; /*value of symbol from lookup table, or pointer to secondary table if needed*/
} HuffmanTree;

/*256 literals, the end code, some length codes, and 2 unused codes*/
#define NUM_DEFLATE_CODE_SYMBOLS 288
/*the distance codes have their own symbols, 30 used, 2 unused*/
#define NUM_DISTANCE_SYMBOLS 32

static void HuffmanTree_init(HuffmanTree* tree) {
    tree->codes = NULL;
    tree->lengths = NULL;
    tree->maxbitlen = 0;
    tree->numcodes = 0;
    tree->table_len = NULL;
    tree->table_value = NULL;
}

#define FIRSTBITS 9u
#define INVALIDBITS 16u
#define INVALIDSYMBOL 65535u

static void HuffmanTree_makeTable(HuffmanTree* tree) {
  static const unsigned headsize = 1u << FIRSTBITS; /*size of the first table*/
  static const unsigned mask = (1u << FIRSTBITS) - 1u;
  size_t i, numpresent, pointer, size; /*total table size*/
  unsigned* maxlens = (unsigned*)malloc(headsize * sizeof(unsigned));

  /* compute maxlens: max total bit length of symbols sharing prefix in the first table*/
  memset(maxlens, 0, headsize * sizeof(*maxlens));
  for(i = 0; i < tree->numcodes; i++) {
    unsigned symbol = tree->codes[i];
    unsigned bitlen = tree->lengths[i];
    unsigned index;
    if(bitlen <= FIRSTBITS) continue; /*symbols that fit in first table don't increase secondary table size*/
    /*get the FIRSTBITS MSBs, the MSBs of the symbol are encoded first. */
    index = pngReverseBits(symbol >> (bitlen - FIRSTBITS), FIRSTBITS);
    maxlens[index] = MAX(maxlens[index], bitlen);
  }

  /* compute total table size: size of first table plus all secondary tables for symbols longer than FIRSTBITS */
  size = headsize;
  for(i = 0; i < headsize; ++i) {
    unsigned bitLen = maxlens[i];
    if(bitLen > FIRSTBITS) size += (((size_t)1) << (bitLen - FIRSTBITS));
  }
  tree->table_len = (unsigned char*)malloc(size * sizeof(*tree->table_len));
  tree->table_value = (unsigned short*)malloc(size * sizeof(*tree->table_value));

  /*initialize with an invalid length to indicate unused entries*/
  for(i = 0; i < size; ++i) tree->table_len[i] = INVALIDBITS;

  /*fill in the first table for long symbols: max prefix size and pointer to secondary tables*/
  pointer = headsize;
  for(i = 0; i < headsize; ++i) {
    unsigned bitLen = maxlens[i];
    if(bitLen <= FIRSTBITS) continue;
    tree->table_len[i] = bitLen;
    tree->table_value[i] = (unsigned short)pointer;
    pointer += (((size_t)1) << (bitLen - FIRSTBITS));
  }
  free(maxlens);

  /*fill in the first table for short symbols, or secondary table for long symbols*/
  numpresent = 0;
  for(i = 0; i < tree->numcodes; ++i) {
    unsigned bitLen = tree->lengths[i];
    unsigned symbol, reverse;
    if(bitLen == 0) continue;
    symbol = tree->codes[i]; /*the huffman bit pattern. i itself is the value.*/
    /*reverse bits, because the huffman bits are given in MSB first order but the bit reader reads LSB first*/
    reverse = pngReverseBits(symbol, bitLen);
    numpresent++;

    if(bitLen <= FIRSTBITS) {
      /*short symbol, fully in first table, replicated num times if bitLen < FIRSTBITS*/
      unsigned num = 1u << (FIRSTBITS - bitLen);
      unsigned j;
      for(j = 0; j < num; ++j) {
        /*bit reader will read the bitLen bits of symbol first, the remaining FIRSTBITS - bitLen bits go to the MSB's*/
        unsigned index = reverse | (j << bitLen);
        if(tree->table_len[index] != INVALIDBITS) return ; /*invalid tree: long symbol shares prefix with short symbol*/
        tree->table_len[index] = bitLen;
        tree->table_value[index] = (unsigned short)i;
      }
    } else {
      /*long symbol, shares prefix with other long symbols in first lookup table, needs second lookup*/
      /*the FIRSTBITS MSBs of the symbol are the first table index*/
      unsigned index = reverse & mask;
      unsigned maxlen = tree->table_len[index];
      /*log2 of secondary table length, should be >= bitLen - FIRSTBITS*/
      unsigned tablelen = maxlen - FIRSTBITS;
      unsigned start = tree->table_value[index]; /*starting index in secondary table*/
      unsigned num = 1u << (tablelen - (bitLen - FIRSTBITS)); /*amount of entries of this symbol in secondary table*/
      unsigned j;
      if(maxlen < bitLen) return ; /*invalid tree: long symbol shares prefix with short symbol*/
      for(j = 0; j < num; ++j) {
        unsigned reverse2 = reverse >> FIRSTBITS; /* bitLen - FIRSTBITS bits */
        unsigned index2 = start + (reverse2 | (j << (bitLen - FIRSTBITS)));
        tree->table_len[index2] = bitLen;
        tree->table_value[index2] = (unsigned short)i;
      }
    }
  }

  /* error check */
  if(numpresent < 2) {
    /* if only has 1 symbol, fill in the remaining table with invalid values */
    for(i = 0; i < size; ++i) {
      if(tree->table_len[i] == INVALIDBITS) {
        tree->table_len[i] = (i < headsize) ? 1 : (FIRSTBITS + 1);
        tree->table_value[i] = INVALIDSYMBOL;
      }
    }
  } else {
    /* check huffman tree whether is invaild */
    for(i = 0; i < size; ++i) {
      if(tree->table_len[i] == INVALIDBITS) return; 
    }
  }

}

static void HuffmanTree_makeFromLengths2(HuffmanTree* tree) {

    unsigned bits = 0, n = 0;
    unsigned* bl_count = (unsigned*)malloc((tree->maxbitlen + 1) * sizeof(unsigned));
    unsigned* next_code = (unsigned*)malloc((tree->maxbitlen + 1) * sizeof(unsigned));
    tree->codes = (unsigned*)malloc(tree->numcodes * sizeof(unsigned));

    for(n = 0; n != tree->maxbitlen + 1; n++) bl_count[n] = next_code[n] = 0;

    /*step 1: count number of instances of each code length*/
    for(bits = 0; bits != tree->numcodes; ++bits) ++bl_count[tree->lengths[bits]];
    /*step 2: generate the nextcode values*/
    for(bits = 1; bits <= tree->maxbitlen; ++bits) {
      next_code[bits] = (next_code[bits - 1] + bl_count[bits - 1]) << 1u;
    }
    /*step 3: generate all the codes*/
    for(n = 0; n != tree->numcodes; ++n) {
      if(tree->lengths[n] != 0) {
        tree->codes[n] = next_code[tree->lengths[n]]++;
        /*remove superfluous bits from the code*/
        tree->codes[n] &= ((1u << tree->lengths[n]) - 1u);
      }
    }

    free(bl_count);
    free(next_code);

    HuffmanTree_makeTable(tree);
}

static void HuffmanTree_makeFromLengths(HuffmanTree* tree, unsigned* bitlen, unsigned numcodes, unsigned maxbitlen) {

    tree->lengths = (unsigned*)malloc(numcodes * sizeof(unsigned));
    memcpy(tree->lengths, bitlen, numcodes * sizeof(unsigned));
    tree->numcodes = numcodes;
    tree->maxbitlen = maxbitlen;

    HuffmanTree_makeFromLengths2(tree);
}

static void generateFixedLitLenTree(HuffmanTree* tree) {

    unsigned* bitLen = (unsigned*)malloc(NUM_DEFLATE_CODE_SYMBOLS * sizeof(unsigned));
    memset(bitLen, 0, NUM_DEFLATE_CODE_SYMBOLS * sizeof(unsigned));

    /*288 possible codes: 0-255=literals, 256=endcode, 257-285=lengthcodes, 286-287=unused*/
    for (unsigned i = 0;   i <= 143; i++) bitLen[i] = 8;
    for (unsigned i = 144; i <= 255; i++) bitLen[i] = 9;
    for (unsigned i = 256; i <= 279; i++) bitLen[i] = 7;
    for (unsigned i = 280; i <= 287; i++) bitLen[i] = 8;

    HuffmanTree_makeFromLengths(tree, bitLen, NUM_DEFLATE_CODE_SYMBOLS, 15);
    free(bitLen);
}

static void generateFixedDistanceTree(HuffmanTree* tree) {
  unsigned* bitlen = (unsigned*)lodepng_malloc(NUM_DISTANCE_SYMBOLS * sizeof(unsigned));

  /*there are 32 distance codes, but 30-31 are unused*/
  for(unsigned i = 0; i != NUM_DISTANCE_SYMBOLS; ++i) bitlen[i] = 5;
  HuffmanTree_makeFromLengths(tree, bitlen, NUM_DISTANCE_SYMBOLS, 15);

  free(bitlen);
}

static void getTreeInflateFixed(HuffmanTree* literalLengthTree, HuffmanTree* distanceTree) {
    generateFixedLitLenTree(literalLengthTree);
    generateFixedDistanceTree(distanceTree);
}

static void getTreeInflateDynamic(HuffmanTree* literalLengthTree, HuffmanTree* distanceTree, pngBitReader* bitReader) {
    /* to be implemented */
}

static void pngInflateHuffmanBlock(vector_uint8_t* out, pngBitReader* bitReader, uint8_t BTYPE, png_t* png) {
    HuffmanTree literalLengthTree;
    HuffmanTree distanceTree;

    HuffmanTree_init(&literalLengthTree);
    HuffmanTree_init(&distanceTree);

    if (1 == BTYPE) {
        /* fixed Huffman codes */
        getTreeInflateFixed(&literalLengthTree, &distanceTree);
    } else if (2 == BTYPE) {
        /* dynamic Huffman codes */
        getTreeInflateDynamic(&literalLengthTree, &distanceTree, bitReader);
    }

    /* to be implemented */
}
/*
 deflate compressed data format (for each block)
+-------+-------+-------+-------+-------+
|BFINAL| BTYPE |   compressed data...   |
+------+-------+-----------------+------+
*/
static void pngInflate(vector_uint8_t* out, vector_uint8_t* in, png_t* png) {

    pngBitReader* bitReader = LodePNGBitReader_init(in->data, in->len);
    uint8_t BFINAL = 0;

    while (!BFINAL) {
        pngEnsureBits9(bitReader);
        BFINAL = pngReadBits(bitReader, 1);
        uint8_t BTYPE = pngReadBits(bitReader, 2);

        if (3 == BTYPE) {
            /*error: reserved BTYPE*/
            pngBitReader_cleanup(bitReader);
            return ;
        } else if (0 == BTYPE) {
            pngInflateNoCompression(out, bitReader, png); /* no compression */
        } else {
            pngInflateHuffmanBlock(out, bitReader, BTYPE, png); /* compressed with fixed/dynamic Huffman codes */
        }
    }
}

/* 
    zlib compressed data format
    0     1
    +----+-----+
    |CMF|FLG|    （more-->）
    +----+-----+

    (if FLG.FDICT set)
       0     1     2     3
    +-----+-----+-----+-----+
    |       DICTID          | （more-->）
    +-----+-----+-----+-----+
 
    +==============+-----+-----+-----+-----+
    | ...compressed data...  |      ADLER32|
    +==============+-----+-----+-----+-----+

    For compression method 8, the compressed data is deflate compressed data format.

    for zlib data format more details, see https://www.rfc-editor.org/rfc/rfc1950#section-2 
    and https://datatracker.ietf.org/doc/html/rfc1951#autoid-1
*/
static void pngZlibDecompressv(vector_uint8_t* out, png_t* png) {

    uint8_t CM, CINFO, FDICT;

    vector_uint8_t in = vector_uint8_t_new();
    in.len = png->criticalChunks.IDAT.length;
    in.data = png->criticalChunks.IDAT.data;

    /* zlib header */
    if((in.data[0] * 256 + in.data[1]) % 31 != 0) {
        /*error: 256 * in[0] + in[1] must be a multiple of 31, the FCHECK value is supposed to be made that way*/
        return ;
    }

    CM = in.data[0] & 0x0F;
    CINFO = (in.data[0] >> 4) & 0x0F;
    /*FCHECK = in.data[1] & 31;*/ /*FCHECK is already tested above*/
    FDICT = (in.data[1] >> 5) & 0x01;
    /*FLEVEL = (in.data[1] >> 6) & 3;*/ /*FLEVEL is not used here*/

    if(CM != 8 || CINFO > 7) {
        /*error: only compression method 8: inflate with sliding window of 32k is supported by the PNG spec*/
        return ;
    }
    if(FDICT != 0) {
        /*error: the specification of PNG says about the zlib stream:
        "The additional flags shall not specify a preset dictionary."*/
        return ;
    }

    in.data += 2; /* skip zlib header */
    in.len -= 2;
    pngInflate(out, &in, png);
}

static void pngZlibDecompress(vector_uint8_t* out, size_t expectedSize, png_t* png) {

    if (expectedSize) {
        out->data = (uint8_t*)malloc(expectedSize);
        out->cap = expectedSize;
        return;
    }
    pngZlibDecompressv(out, png);
    
}

static void pngDecodeRawImageData(png_t* png) {

    vector_uint8_t scanlines = vector_uint8_t_new();
    uint32_t expectedSize = pngCalRawIDATSize(png);
    pngZlibDecompress(&scanlines, expectedSize, png);
    
}
/* chunk data type */
/* Length 	   Chunk type 	Chunk data 	      CRC   */
/* 4 bytes 	    4 bytes 	Length bytes 	4 bytes */ 
static void pngReadChunk(png_t* png, FILE* fd) {

    CriticalChunk_t* criticalChunks = &(png->criticalChunks);
    AncillaryChunk_t* ancillaryChunks = &(png->ancillaryChunks);

    Chunk_t chunk;
    do {
        
        memset(&chunk, 0, sizeof(Chunk_t));

        pngReadUint32(&(chunk.length), sizeof(chunk.length), fd);
        
        pngRead(&(chunk.type), sizeof(chunk.type), fd);
        
        chunk.data = (uint8_t*)malloc(chunk.length);
        pngRead(chunk.data, chunk.length, fd);
        
        pngReadUint32(&(chunk.crc), sizeof(chunk.crc), fd);

        if (memcmp(chunk.type, "IHDR", 4) == 0) {
            pngReadIHDR(&criticalChunks->IHDR, &chunk); /* first check */
        } else if (memcmp(chunk.type, "PLTE", 4) == 0) {
            pngReadPLTE(&criticalChunks->PLTE_tRNS, &chunk);
        } else if (memcmp(chunk.type, "IDAT", 4) == 0) {
            pngReadIDAT(&criticalChunks->IDAT, &chunk);
        } else if (memcmp(chunk.type, "IEND", 4) == 0) {
            criticalChunks->IEND = chunk;
        } else if (memcmp(chunk.type, "bKGD", 4) == 0) {
            ancillaryChunks->bKGD = chunk;
        } else if (memcmp(chunk.type, "cHRM", 4) == 0) {
            ancillaryChunks->cHRM = chunk;
        } else if (memcmp(chunk.type, "cICP", 4) == 0) {
            ancillaryChunks->cICP = chunk;
        } else if (memcmp(chunk.type, "dSIG", 4) == 0) {
            ancillaryChunks->dSIG = chunk;
        } else if (memcmp(chunk.type, "eXIf", 4) == 0) {
            ancillaryChunks->eXIf = chunk;
        } else if (memcmp(chunk.type, "gAMA", 4) == 0) {
            ancillaryChunks->gAMA = chunk;
        } else if (memcmp(chunk.type, "hIST", 4) == 0) {
            ancillaryChunks->hIST = chunk;
        } else if (memcmp(chunk.type, "iCCP", 4) == 0) {
            ancillaryChunks->iCCP = chunk;
        } else if (memcmp(chunk.type, "iTXt", 4) == 0) {
            ancillaryChunks->iTXt = chunk;
        } else if (memcmp(chunk.type, "pHYs", 4) == 0) {
            ancillaryChunks->pHYs = chunk;
        } else if (memcmp(chunk.type, "sBIT", 4) == 0) {
            ancillaryChunks->sBIT = chunk;
        } else if (memcmp(chunk.type, "sPLT", 4) == 0) {
            ancillaryChunks->sPLT = chunk;
        } else if (memcmp(chunk.type, "sRGB", 4) == 0) {
            ancillaryChunks->sRGB = chunk;
        } else if (memcmp(chunk.type, "sTER", 4) == 0) {
            ancillaryChunks->sTER = chunk;
        } else if (memcmp(chunk.type, "tEXt", 4) == 0) {
            ancillaryChunks->tEXt = chunk;
        } else if (memcmp(chunk.type, "tIME", 4) == 0) {
            ancillaryChunks->tIME = chunk;
        } else if (memcmp(chunk.type, "tRNS", 4) == 0) {
            pngReadtRNS(criticalChunks, &chunk);
        } else if (memcmp(chunk.type, "zTXt", 4) == 0) {
            ancillaryChunks->zTXt = chunk;
        }
    } while(memcmp(chunk.type, "IEND", 4) != 0);

    pngDecodeRawImageData(png);

}

png_t* pngReadFile(char *filename) {
    FILE *fd = fopen(filename,"rb"); 
    if(fd == NULL) {
        perror("open failed!");
        exit(1);        //error, exit the program
    }

    png_t* png = (png_t*)malloc(sizeof(png_t));
    memset(png, 0, sizeof(png_t));
    pngReadFileHeader(png, fd); /* PNG signature */
    pngReadChunk(png, fd);
    fclose(fd);
    return png;
}