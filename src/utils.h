#ifndef UTILS_H
#define UTILS_H

#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

typedef struct {
    float key;
    int val;
} pair_t;

typedef struct {
    int id;
    pthread_t thread;
    pair_t *heap;
    float *startPoint;
    float *endPoint;
    int inputIndex;
    int sizeHeap;
    int sizeSearch;
    int heapS;
    int start_index;
    int end_index;
    float *Input;
} heap_pthread_t;

//extern float *Input;

void *threadedMaxHeap(void *arg);
void verifyOutput(const float *Input, const pair_t *Output, int nTotalElmts,
                  int k);
void drawHeapTree(int heap[], int size, int nLevels);
void maxHeapify(pair_t *heap, int size, int i);
// void heapifyUp(pair_t* heap, int *size, int pos);
// int isMaxHeap(pair_t* heap, int size);
void insert(pair_t* heap, int size, float element, int val);
void decreaseMax(pair_t *heap, int size, float new_value, int val);
int cmpfunc(const void *a, const void *b);
int cmpfuncK(const void *A, const void *B);
pair_t *get_one_heap(heap_pthread_t **heap_set, int nTotalThreads, int k);
void *bodyThread(void *arg);
void *threadedMaxHeap(void *args);
int parent(int pos);
double timestamp(void);
#endif
