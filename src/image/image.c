#include <image/image.h>
#define STB_IMAGE_IMPLEMENTATION
#include <ext/stb_image/stb_image.h>

int image_load(Image **img_p, unsigned char *img_name){

    *img_p = malloc(sizeof(Image));
    if(*img_p == NULL){
        fprintf(stderr, "Error in allocating memory\n");
    }
    
    (*img_p)->data = stbi_load(img_name,
                          &((*img_p)->width),
                          &((*img_p)->height),
                          &((*img_p)->channels),
                          0);
    if((*img_p)->data == NULL){
        fprintf(stderr, "Error in loading the image\n");
        return IMAGE_ERROR; 
    }

    (*img_p)->size = ((*img_p)->width) * ((*img_p)->height) * ((*img_p)->channels);

    return IMAGE_OK;
}

void image_free(Image *img_p){

    if(img_p == NULL){
        return;
    }
    if(img_p->data != NULL)
        stbi_image_free(img_p->data);

    free(img_p);
}

int image_get_pixel(Image *img_p, unsigned int x, unsigned int y, Pixel* p){

    if(img_p == NULL){
        fprintf(stderr, "invalid image\n");
        return IMAGE_ERROR;
    }

    if(x >= img_p->width || y >= img_p->height){
        fprintf(stderr, "invalid pixel position\n");
        return IMAGE_ERROR;
    }

    int position = (y*img_p->width + x)*img_p->channels;
    
    for(int i = 0 ; i < img_p->channels ; i++){
        p->val[i] = *(img_p->data + position + i);
    }

    return IMAGE_OK;

}

int image_print_pixel(Pixel pixel, unsigned int channels, int hex){
    
    if(channels == 0 || channels > 4 ){
        fprintf(stderr, "channels must be 1-4\n");
        return IMAGE_ERROR;
    }

    if(hex){
        printf("#");
        for(int i = 0 ; i < channels ; i++){
            printf("%02" PRIx8 "", pixel.val[i]) ;
        }
        printf("\n");
    } else{
        int i = 0;
        for(i = 0 ; i < channels-1; i++){
            printf("%" PRIu8 " ",  pixel.val[i]);
        }
        printf("%" PRIu8 "\n",  pixel.val[i]);
    }

    return IMAGE_OK;

}

uint32_t image_pixel_to_32(Pixel pixel){
    uint32_t ret = 0;
    ret = pixel.val[0];
    ret = (ret << 8)  | pixel.val[1];
    ret = (ret << 8)  | pixel.val[2];
    ret = (ret << 8)  | pixel.val[3];
    return ret;
}

Pixel image_32_to_pixel(uint32_t num){
    Pixel pixel;
    pixel.val[3] = 0x0000FF & num; 
    num >>= 8;
    pixel.val[2] = 0x0000FF & num; 
    num >>= 8;
    pixel.val[1] = 0x0000FF & num; 
    num >>= 8;
    pixel.val[0] = 0x0000FF & num; 
    num >>= 8;
    return pixel;
}
