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

size_t image_unique_colors(Image* img_p, uint32_t **colors ){

    if(img_p == NULL){
        fprintf(stderr, "Provide an image input\n");
        return 0;
    }

    // the keys to the hash table are the colors in hex
    // 9 characters because of the \0
    char **keys = NULL;
    size_t keys_num = 0;
    hcreate(img_p->size);
    
    // temporary values
    uint32_t color = 0;
    Pixel pixel = { { 0,0,0,0 } };
    char key[9];
    ENTRY e;
    ENTRY *ep;

    // loop through all pixels and put the unique ones
    // in a hashmap
    for(int y = 0 ; y < img_p->height ; y++){
        for(int x = 0 ; x < img_p->width ; x++){
            image_get_pixel(img_p, x, y, &pixel);
            color = image_pixel_to_32(pixel);
            sprintf(key, "%"PRIx32, color );
            
            //find out if key exists in the hashmap
            e.key = key;
            ep = hsearch(e, FIND);
            
            // if doesn't exist, put key in the keys array
            if(ep == 0){
                keys_num = arr_str_push(&keys, keys_num, key);
                
                // put color in the hash table
                e.key = strdup(key);
                e.data = (void *) (uintptr_t) color;
                ep = hsearch(e, ENTER);
                if(ep == NULL){
                    fprintf(stderr, "hash table entry failed\n");
                    return 0;
                }
            } 
        }
    }
    
    // put color values in the array colors[]
    *colors = malloc(keys_num * sizeof(uint32_t));
    if(*colors == NULL){
        fprintf(stderr, "error in allocating memory\n");
        return 0;
    }
    for(int i = 0 ; i < keys_num ; i++){
        e.key = keys[i];
        ep = hsearch(e, FIND);

        if(ep){
            *(*colors + i) = (uint32_t) (uintptr_t) ep->data;
            free(ep->key);
        }
    }

    hdestroy();
    arr_str_free(keys, keys_num);

    return keys_num;
}

/* typedef struct { */
    /* int width; */
    /* int height; */
    /* int channels; */
    /* size_t size; */
    /* uint8_t *data; */
/* } Image; */
/* double** image_split_to_double(uint8_t* arr, size_t size, size_t cols ){ */
double** image_split_to_double(Image* img_p ){
    /* if(img_p->size % img_p%channels != 0){ */
        /* return NULL; */
    /* } */
    if(img_p == NULL) return NULL;

    double** mat = NULL;
    size_t lines = img_p->size/img_p->channels;

    size_t i = 0, j = 0;
    mat = (double**) malloc2d(lines, img_p->channels, sizeof(double) );

    for(int count = 0; count < img_p->size ; count++ ){
        i = (size_t) floor(count/img_p->channels);
        j = (size_t) count%img_p->channels;
        /* printf("%d %d\n", i, j); */
        mat[i][j] = (double) img_p->data[count];
    }
    
    return mat;
}

void image_destroy_split(Image* img_p, void** mat){
    size_t lines = (img_p->size/img_p->channels);
    free2d((void**)mat, lines);
}
