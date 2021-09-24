#include <utils/alloc2d.h>

void** malloc2d(size_t lin, size_t col, size_t data_length){
    void** mat = NULL;
    mat = malloc(lin*sizeof(void*));
    for(int i = 0 ; i < lin ; i++){
        mat[i] = malloc(col*data_length);
    }
    return mat;
}

void free2d(void** mat, size_t lin){
    if(mat == NULL) return;
    for (int i = 0 ; i < lin; i++){
        free(mat[i]);
    }
    free(mat);
}
