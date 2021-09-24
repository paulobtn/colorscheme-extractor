#ifndef ALLOC2D_H
#define ALLOC2D_H

#include <stdlib.h>

void** malloc2d(size_t lin, size_t col, size_t data_length);

void free2d(void** mat, size_t lin);

#endif /* ALLOC2D_H */
