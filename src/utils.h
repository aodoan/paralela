#ifndef UTILS_H
#define UTILS_H
#include <stdio.h>
#include <stdlib.h>

struct {
    float key;
    int val;
} typedef pair_t;

extern float *Input;

void verifyOutput(const float *Input, const pair_t *Output, int nTotalElmts,
                  int k);
void drawHeapTree(int heap[], int size, int nLevels);
void maxHeapify(pair_t *heap, int size, int i);
void heapifyUp(int heap[], int *size, int pos);
void insert(int heap[], int *size, int element);
int isMaxHeap(int heap[], int size);
void decreaseMax(pair_t *heap, int size, float new_value, int val);

#endif
