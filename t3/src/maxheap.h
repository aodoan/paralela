// Autores
// Claudinei Aparecido Alduan Filho GRR20203920
// Rodrigo Saviam Soffner GRR20205092

#ifndef MAXHEAP_H
#define MAXHEAP_H
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <pthread.h>

typedef struct {
    float dist;
    int index;
} pair_t;


pair_t* createMaxHeap(int capacity);
void decreaseMax(pair_t *heap, int *size, int *capacity, float dist, int index);
void insert(pair_t *heap, int *size, float dist, int index);
#endif
