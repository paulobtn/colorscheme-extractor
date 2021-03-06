#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdint.h>
#include <inttypes.h>
#include <search.h>
#include <string.h>
#include <time.h>

#include <kmeans/kmeans.h>
#include <image/image.h>
#include <utils/arrstr.h>
#include <utils/alloc2d.h>

#define CORE_OK       0
#define CORE_ERROR   -1

int parse_cmd_input( int argc, char** argv);
int dump_colors(Image* img_p, int hex);

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

/* void** split(int arr, int n_arr, int cols, size_t el_size){ */
    /* void** mat = NULL; */
    /* int lines = n_arr/cols; */
    /* mat = malloc(lines*sizeof(void*)); */

    /* for(int i = 0 ; i < lines ; i++){ */

    /* } */
/* } */


int main(int argc, char *argv[]){

    
    Image* img = NULL;
    
    //parse command line input
    if(parse_cmd_input(argc, argv) != CORE_OK){
        return 1;
    }
    
    //load image if provided
    if(cmd_args.img_name){

        if(image_load( &img, cmd_args.img_name ) != IMAGE_OK){
            return 1;
        }
    }
    
    //dump colors in rgb or hex
    if(cmd_args.dump_colors){
        
        if(dump_colors(img, cmd_args.print_hex) != CORE_OK){
            return 1;
        }
    }

    if(cmd_args.create_colors){
        printf("This will extract colors from the image %s using Kmeans.\n", cmd_args.img_name);

        double** img_sp = NULL;
        
        img_sp = image_split_to_double(img);
        
        size_t size = img->size/img->channels;

        KMEANS_T *kp = NULL;
        kp = kmeans_alloc( img_sp, size, img->channels, 8);
        /* kmeans_apply(kp, KMEANS_KPP, 100); */
        kmeans_init(kp, KMEANS_KPP);

        
        for(int i = 0 ; i < 8 ; i++){
            printf("centroid %d: %lf\t%lf\t%lf\n", i, kp->centroids[i].val[0], kp->centroids[i].val[1], kp->centroids[i].val[2]);
        }

        image_destroy_split(img, (void**)img_sp);
        kmeans_destroy(kp);



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

    return CORE_OK;

    PARSE_CMD_INPUT_ERR:
        fprintf(stderr, "Usage: %s [-vh]\n", argv[0]);
        fprintf(stderr, "       %s [file...] [-options]\n", argv[0]);
        return CORE_ERROR;
}

int dump_colors(Image* img_p, int hex){
    
    if(img_p == NULL){
        fprintf(stderr, "Provide an image input\n");
        return CORE_ERROR;
    }
    
    uint32_t* unique_colors = NULL;
    size_t unique_colors_size = 0;

    unique_colors_size = image_unique_colors(img_p, &unique_colors );
    if(unique_colors_size == 0){
        fprintf(stderr, "error in extracting unique colors\n");
        return CORE_ERROR;
    }

    for(int i = 0 ; i < unique_colors_size ; i++){
        Pixel p = image_32_to_pixel(unique_colors[i]);
        image_print_pixel(p, img_p->channels, hex);
    }

    free(unique_colors);

    return CORE_OK;
}
