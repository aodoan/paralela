#ifndef UTILS_H
#define UTILS_H

#include <pthread.h>

typedef struct {
    float key;
    int val;
} pair_t;

typedef struct {
    int id;
    pthread_t thread;
    pair_t *heap;

    float *startPoint;
    long int MOP;
    int heapSize;
    int searchSize;
    int startIndex;
} heap_pthread_t;

void verifyOutput(const float *Input, const pair_t *Output, int nTotalElmts,
                  int k);

pair_t *join_heaps(heap_pthread_t **heap_set, int nTotalThreads, int k);

void *bodyThread(void *arg);
void *threadedMaxHeap(void *args);
pair_t *sequencial(float *Input, int nTotalElements, int k, long int *MOP);
double timestamp(void);

#endif
