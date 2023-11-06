// Autores
// Claudinei Aparecido Alduan Filho GRR20203920
// Rodrigo Saviam Soffner GRR20205092

#ifndef MAXHEAP_H
#define MAXHEAP_H

#include <stdlib.h>

typedef struct {
    float *heap;
    int size;
    size_t capacity;
} maxheap_t;

maxheap_t* createMaxHeap(size_t capacity);
void decreaseMax(maxheap_t *heap, float new_value);

#endif
