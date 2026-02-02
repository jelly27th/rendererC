#include "png.h"
#include <string.h>
#include <stdlib.h>
#include "../utils/utils.h"
#include "../dyarray/dyarray.h"

/* https://en.wikipedia.org/wiki/PNG */
/* https://www.libpng.org/pub/png/spec/1.2/PNG-Chunks.html */
/* https://www.rfc-editor.org/rfc/rfc1950 */
/* https://www.rfc-editor.org/rfc/rfc1951 */

void pngDecode(const char* filename, png_t* png) {
  unsigned error;
  unsigned char* image = 0;
  unsigned width, height;
  unsigned char* png = 0;
  size_t pngsize;

  error = lodepng_load_file(&png, &pngsize, filename);
  if(!error) error = lodepng_decode32(&image, &width, &height, png, pngsize);
  if(error) printf("error %u: %s\n", error, lodepng_error_text(error));

  free(png);

  /*use image here*/
  png->width = width;
  png->height = height;
  png->imageData = image;

}

void pngFree(png_t* png) {
    if (png->imageData) {
        free(png->imageData);
        png->imageData = NULL;
    }
}