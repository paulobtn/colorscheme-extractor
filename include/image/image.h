/* This is a wrapper that provides a simple interface to the library
 * stb_image. */

#ifndef IMAGE_H
#define IMAGE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <inttypes.h>
#include <search.h>
#include <utils/arrstr.h>
#include <utils/alloc2d.h>

#define IMAGE_OK    0
#define IMAGE_ERROR -1

// object that store image data
typedef struct {
    int width;
    int height;
    int channels;
    size_t size;
    uint8_t *data;
} Image;

// pixel type
// stores [r, g, b, a] if rgba
//        [r, g, b, 0] if rgb
//        [v, 0, 0, 0] if grayscale with one channel
//        [v, a, 0, 0] if grayscale with transparency
typedef struct {
    uint8_t val[4];
} Pixel;

// Loads the image img_name to a struct Image
int image_load(Image **img_p, unsigned char *img_name);

// Free memory
void image_free(Image *img_p);

// get the image pixel in the position (x,y)
int image_get_pixel(Image *img_p, unsigned int x, unsigned int y, Pixel* p);

// print a pixel with a customizable format
// channels indicate how many channels to print
// hex = 0 means that it will print in the format R G B
// hex = 1 means that it will print in hexadecimal
int image_print_pixel(Pixel pixel, unsigned int channels, int hex);

// Get a 32 bits number from the rgba pixel
uint32_t image_pixel_to_32(Pixel pixel);

// given a 32 bits number, extract the rgba values to the pixel
Pixel image_32_to_pixel(uint32_t num);

// extract unique colors from an image and put in an array uint32_t *colors
// the user should pass the reference &colors and colors should be dealocated
// after the usage.
size_t image_unique_colors(Image* img_p, uint32_t **colors );

double** image_split_to_double(Image* img_p );

void image_destroy_split(Image* img_p, void** mat);

#endif /* IMAGE_H */
