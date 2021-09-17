#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <core/test.h>
#include <kmeans/kmeans.h>

#define STB_IMAGE_IMPLEMENTATION
#include <ext/stb_image/stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <ext/stb_image/stb_image_write.h>

#define F_OK       0
#define F_ERROR   -1

int parse_cmd_input( int argc, char** argv);

/* cmd input variables:
 * set automatically by parse_cmd_input
 * they indicate the flags for each command line option
 * and the working image file name
 */
static int  show_version;
static int  show_help;
static int  create_colors;
static int  dump_colors;
static char *img_name;

int main(int argc, char *argv[]){
    
    int width, height, channels = 0;
    unsigned char *img = NULL;
    
    if(parse_cmd_input(argc, argv) != F_OK){
        return 1;
    }
    
    if(img_name){

        img = stbi_load(img_name, &width, &height, &channels, 0);
        if(img == NULL){
            fprintf(stderr, "Error in loading the image\n");
            return 1; 
        }
        printf("Loaded image %s\n", img_name);
        printf("width:    \t%dpx\n", width);
        printf("height:   \t%dpx\n", height);
        printf("channels: \t%dpx\n", channels);
    }

    if(dump_colors){

        if(img_name == NULL){
            fprintf(stderr, "Provide an image input\n");
            fprintf(stderr, "Usage: %s [-vh]\n", argv[0]);
            fprintf(stderr, "       %s [file...] [-options]\n", argv[0]);
            return 1;
        }

        printf("Dump colors from image %s\n", img_name);
    }

    if(show_version){
      printf("Show version\n");
    }

    if(show_help){
      printf("Show help\n");
    }

    if(create_colors){
      printf("Create colorscheme from image %s\n", img_name);
    }
    
    if(img != NULL){
        stbi_image_free(img);
    }

    return 0;
}

int parse_cmd_input( int argc, char** argv){

    int opt_char = 0; //command line option
    char *opt_string = "vhcd";

    while (1) {

        static struct option long_options[] = {
            {"version",       no_argument,       0,   'v'},
            {"help",          no_argument,       0,   'h'},
            {"create-colors", no_argument,       0,   'c'},
            {"dump-colors",   no_argument,       0,   'd'},
            {0,               0,                 0,    0 }
        };

        int opt_index = 0;
        opt_char = getopt_long(argc, argv, opt_string, long_options, &opt_index);

        if(opt_char == -1) 
            break;

        switch (opt_char) {
            case 'v': show_version  = 1; break;
            case 'h': show_help     = 1; break;
            case 'c': create_colors = 1; break;
            case 'd': dump_colors   = 1; break;
            default: goto PARSE_CMD_INPUT_ERR;
        }
    }
    
    //check if the image input was provided as an independent arg
    if (optind < argc) {
        //should be only one image input
        if(optind != argc-1){
            goto PARSE_CMD_INPUT_ERR;
        }
        
        img_name = argv[optind];
    }

    return F_OK;

    PARSE_CMD_INPUT_ERR:
        fprintf(stderr, "Usage: %s [-vh]\n", argv[0]);
        fprintf(stderr, "       %s [file...] [-options]\n", argv[0]);
        return F_ERROR;
}
