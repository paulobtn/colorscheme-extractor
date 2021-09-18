/* This is a wrapper that proovides a simple interface to the library
 * stb_image. */

#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#define IMAGE_OK    0
#define IMAGE_ERROR -1

typedef struct {
    int width;
    int height;
    int channels;
    size_t size;
    uint8_t *data;
} Image;

//loads the image img_name to a struct Image
int image_load(Image **img_p, unsigned char *img_name);
//free memory
void image_free(Image *img_p);

#endif /* IMAGE_H */
