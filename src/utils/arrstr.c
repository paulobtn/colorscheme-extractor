#include <utils/arrstr.h>

size_t arr_str_push(char *** str_arr, unsigned int arr_size, char* str){
    
    int str_len = strlen(str);
     
    if(*str_arr == NULL){
        //allocate space for the string
        *str_arr = malloc(1*sizeof(char*));
        if(*str_arr == NULL){
            fprintf(stderr, "Error in allocating memory\n");
            exit(EXIT_FAILURE);
        }
        //allocate characters for the first string
        *(*str_arr + 0) = malloc((str_len+1)*sizeof(char));
        if( *(*str_arr + 0) == NULL){
            fprintf(stderr, "Error in allocating memory\n");
            exit(EXIT_FAILURE);
        }
        //copy string
        strcpy(*(*str_arr + 0), str);

    } else{
        //reallocating to the new strings array size
        *str_arr = realloc(*str_arr, (arr_size+1)*sizeof(char*));
        if(*str_arr == NULL){
            fprintf(stderr, "Error in allocating memory\n");
            exit(EXIT_FAILURE);
        }
        //allocating space for the new string
        *(*str_arr + arr_size) = malloc((str_len+1)*sizeof(char));
        if( *(*str_arr + 0) == NULL){
            fprintf(stderr, "Error in allocating memory\n");
            exit(EXIT_FAILURE);
        }
        //copy string
        strcpy(*(*str_arr + arr_size), str);

    } 

    return (arr_size+1);
}

size_t arr_str_delete(char *** str_arr, size_t arr_size, unsigned int pos){

    if(pos >= arr_size){
        fprintf(stderr, "Index out of bounds\n");
        exit(EXIT_FAILURE);
    }
    
    if(*(*str_arr+pos) != NULL){
        free(*(*str_arr+pos));
    }

    for(int i = pos+1 ; i < arr_size ; i++){
        *(*str_arr + i - 1) = *(*str_arr+i) ;
    }
    
    if(arr_size == 1){
        free(*str_arr);
    } else{
        *str_arr = realloc(*str_arr, (arr_size -1)*sizeof(char*));
        if(*str_arr == NULL){
            fprintf(stderr, "Error in allocating memory\n");
            exit(EXIT_FAILURE);
        }
    }

    return (arr_size-1);
}

void arr_str_free(char **str_arr, size_t arr_size){
    if(str_arr == NULL){
        return;
    }

    for(int i = 0 ; i < arr_size ; i++){
        free(str_arr[i]);
    }
    free(str_arr);
}

