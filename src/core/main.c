#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <core/test.h>
#include <kmeans/kmeans.h>

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
static int  has_img_input;
static char *img_name;

int main(int argc, char *argv[]){
  
    if(parse_cmd_input(argc, argv) != F_OK){
        return 1;
    }

    if(has_img_input){
        printf("Input provided: %s\n", img_name);
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


  return 0;
}

int parse_cmd_input( int argc, char** argv){

    int opt_char = 0; //command line option
    char *opt_string = "vhc";

    while (1) {

        static struct option long_options[] = {
            {"version",       no_argument,       0,   'v'},
            {"help",          no_argument,       0,   'h'},
            {"create-colors", no_argument,       0,   'c'},
            {0,               0,                 0,    0 }
        };

        int opt_index = 0;
        opt_char = getopt_long(argc, argv, opt_string, long_options, &opt_index);

        if(opt_char == -1) 
            break;

        switch (opt_char) {
            case 'c': create_colors = 1; break;
            case 'h': show_help     = 1; break;
            case 'v': show_version  = 1; break;
            default: goto ERR;
        }
    }
    
    //check if the image input was provided as an independent arg
    if (optind < argc) {
        //should be only one image input
        if(optind == argc-1){
            has_img_input = 1;
            img_name = argv[optind];
        } else {
            goto ERR;
        }
    }

    return F_OK;

    ERR:
        fprintf(stderr, "Usage: %s [-vh]\n", argv[0]);
        fprintf(stderr, "       %s [file...] [-vhc]\n", argv[0]);
        return F_ERROR;
}
