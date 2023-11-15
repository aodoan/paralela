// Autores
// Claudinei Aparecido Alduan Filho GRR20203920
// Rodrigo Saviam Soffner GRR20205092

#ifndef MAXHEAP_H
#define MAXHEAP_H
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
typedef struct {
    float *heap;
    int size;
    int capacity;
} maxheap_t;

float* createMaxHeap(int capacity);
void decreaseMax(float *heap, int *size, int *capacity, float new_value);
void maxHeapify(float *heap, int size, int i);
void heapifyUp(float *heap, int pos);
void insert(float *heap, int *size, float element);
#endif
