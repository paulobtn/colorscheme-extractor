#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <core/test.h>
#include <kmeans/kmeans.h>

#include <stdint.h>

#define STB_IMAGE_IMPLEMENTATION
#include <ext/stb_image/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <ext/stb_image/stb_image_write.h>

#define F_OK       0
#define F_ERROR   -1

int parse_cmd_input( int argc, char** argv);

typedef struct {
    int width;
    int height;
    int channels;
    size_t size;
    uint8_t *data;
} Image;

int image_load(Image **img_p, unsigned char *img_name){

    *img_p = malloc(sizeof(Image));
    
    (*img_p)->data = stbi_load(img_name,
                          &((*img_p)->width),
                          &((*img_p)->height),
                          &((*img_p)->channels),
                          0);
    if((*img_p)->data == NULL){
        fprintf(stderr, "Error in loading the image\n");
        return F_ERROR; 
    }

    (*img_p)->size = ((*img_p)->width) * ((*img_p)->height) * ((*img_p)->channels);

    return F_OK;
}

void image_free(Image *img_p){

    if(img_p == NULL){
        return;
    }
    if(img_p->data != NULL)
        stbi_image_free(img_p->data);

    free(img_p);
}

/* cmd input variables:
 * set automatically by parse_cmd_input
 * they indicate the flags for each command line option
 * and the working image file name
 */
struct Args {
    int  show_version;
    int  show_help;
    int  create_colors;
    int  dump_colors;
    int  print_hex;
    char *img_name;
};

struct Args cmd_args = {
    .show_version       = 0,
    .show_help          = 0,
    .create_colors      = 0,
    .dump_colors        = 0,
    .print_hex          = 0,
    .img_name          = NULL
};

int dump_colors(Image* img_p, int hex){
    
    if(img_p == NULL){
        fprintf(stderr, "Error in dumping the image\n");
        return F_ERROR;
    }


    for (unsigned char *p = img_p->data ; p != img_p->data + img_p->size; p+= img_p->channels){
        if(hex){
            printf("#%02x%02x%02x\n", *p, *(p+1), *(p+2));
        } else{
            printf("%d\t%d\t%d\n", *p, *(p+1), *(p+2));
        }
    }

    return F_OK;

}

int main(int argc, char *argv[]){
    
    Image* img = NULL;
    
    if(parse_cmd_input(argc, argv) != F_OK){
        return 1;
    }
    
    //load image if provided
    if(cmd_args.img_name){

        if(image_load( &img, cmd_args.img_name ) != F_OK){
            return 1;
        }
    }
    
    //dump colors in rgb or hex
    if(cmd_args.dump_colors){
        
        if(img == NULL ){
            fprintf(stderr, "Provide an image input\n");
            fprintf(stderr, "Usage: %s [-vh]\n", argv[0]);
            fprintf(stderr, "       %s [file...] [-options]\n", argv[0]);
            return F_ERROR;
        }

        if(dump_colors(img, cmd_args.print_hex) != F_OK){
            return 1;
        }

    }

    if(cmd_args.create_colors){
      printf("Create colorscheme from image %s\n", cmd_args.img_name);
    }

    if(cmd_args.show_version){
      printf("Show version\n");
    }

    if(cmd_args.show_help){
      printf("Show help\n");
    }
    
    image_free(img);

    return 0;
}

int parse_cmd_input( int argc, char** argv){

    int opt_char = 0; //command line option
    char *opt_string = "vhcdx";

    while (1) {

        static struct option long_options[] = {
            {"version",       no_argument,       0,   'v'},
            {"help",          no_argument,       0,   'h'},
            {"create-colors", no_argument,       0,   'c'},
            {"dump-colors",   no_argument,       0,   'd'},
            {"hex",           no_argument,       0,   'x'},
            {0,               0,                 0,    0 }
        };

        int opt_index = 0;
        opt_char = getopt_long(argc, argv, opt_string, long_options, &opt_index);

        if(opt_char == -1) 
            break;

        switch (opt_char) {
            case 'v': cmd_args.show_version  = 1; break;
            case 'h': cmd_args.show_help     = 1; break;
            case 'c': cmd_args.create_colors = 1; break;
            case 'd': cmd_args.dump_colors   = 1; break;
            case 'x': cmd_args.print_hex     = 1; break;
            default: goto PARSE_CMD_INPUT_ERR;
        }
    }
    
    //check if the image input was provided as an independent arg
    if (optind < argc) {
        //should be only one image input
        if(optind != argc-1){
            goto PARSE_CMD_INPUT_ERR;
        }
        
        cmd_args.img_name = argv[optind];
    }

    return F_OK;

    PARSE_CMD_INPUT_ERR:
        fprintf(stderr, "Usage: %s [-vh]\n", argv[0]);
        fprintf(stderr, "       %s [file...] [-options]\n", argv[0]);
        return F_ERROR;
}
