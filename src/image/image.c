#include <image/image.h>
#define STB_IMAGE_IMPLEMENTATION
#include <ext/stb_image/stb_image.h>

int image_load(Image **img_p, unsigned char *img_name){

    *img_p = malloc(sizeof(Image));
    
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
