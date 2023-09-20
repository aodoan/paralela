#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct {
    float key;
    int val;
} typedef pair_t;

extern float *Input;

void verifyOutput(const float *Input, const pair_t *Output, int nTotalElmts,
                  int k);
void drawHeapTree(int heap[], int size, int nLevels);
// void maxHeapify(pair_t *heap, int size, int i);
// void heapifyUp(pair_t* heap, int *size, int pos);
// int isMaxHeap(pair_t* heap, int size);
void insert(pair_t* heap, int size, float element, int val);
void decreaseMax(pair_t *heap, int size, float new_value, int val);
int cmpfunc(const void *a, const void *b);
int cmpfuncK(const void *A, const void *B);
pair_t *get_one_heap(pair_t **heap_set, int nTotalThreads, int k);
#endif
