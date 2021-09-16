#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <core/test.h>
#include <kmeans/kmeans.h>


int main(int argc, char *argv[]){

  int opt_char = 0;
  char *opt_string = "v:h:g";


  while (1) {

      static struct option long_options[] = {
            {"version",       no_argument,       0, 'v'},
            {"help",          no_argument,       0, 'h'},
            {"create-colors", required_argument, 0, 'c'},
            {0, 0, 0, 0}
      };

      int opt_index = 0;
      opt_char = getopt_long(argc, argv, "vhc:", long_options, &opt_index);

      if(opt_char == -1) 
        break;
      
      switch (opt_char) {
        case 'c': 
            printf("Create colorscheme from image %s\n", optarg);
            break;
        case 'h': 
            printf("Show help\n");
            break;
        case 'v': 
            printf("Show version\n");
            break;

        default:
            fprintf(stderr, "Usage: %s [-vhc] [file...]\n", argv[0]);
            exit(EXIT_FAILURE);
      }
  }

  return 0;
}
