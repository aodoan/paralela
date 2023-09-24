#include "utils.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define SHOW_DECREASE_MAX_STEPS 1
#define MAX_HEAP_SIZE (1024 * 1024)

int parent(int pos){
    return ((pos - 1) / 2); 
}

void verifyOutput(const float *Input, const pair_t *Output, int nTotalElmts,
                  int k) {
    int ok = 1;

    pair_t *sortedInput = malloc(sizeof(pair_t) * nTotalElmts);

    for (int i = 0; i < nTotalElmts; ++i) {
        sortedInput[i].key = Input[i];
        sortedInput[i].val = i;
    }

    pair_t *sortedOutput = malloc(sizeof(pair_t) * k);
    memcpy(sortedOutput, Output, sizeof(pair_t) * k);

    qsort(sortedInput, nTotalElmts, sizeof(pair_t), cmpfuncK);
    qsort(sortedOutput, k, sizeof(pair_t), cmpfuncK);

    

    // verify if the heap is correct
    for (int i = 0; i < k; i++) {
        if (sortedInput[i].key != sortedOutput[i].key) ok = 0;
    }
    /*

    printf("ORIGINAL: ");
    for(int i = 0; i < nTotalElmts; i++) printf("[%f %i] ", sortedInput[i].key, sortedInput[i].val); 
    printf("\nSAIDA: "); 
    
    for(int i = 0; i < k; i++)  printf("[%f %i] ",sortedOutput[i].key, sortedOutput[i].val); printf("\n");
    */
    if (ok == 1)
        printf("\nOutput set verified correctly.\n");
    else
        printf("\nOutput set DID NOT compute correctly!!!\n");
}


void drawHeapTree(int heap[], int size, int nLevels)  // FIX ME!
{
    int offset = 0;
    // int space = (int)pow(2, nLevels - 1);

    int nElements = 1;
    for (int level = 0; level < nLevels; level++) {
        // print all elements in this level
        for (int i = offset; i < size && i < (offset + nElements); i++) {
            printf("[%3d]", heap[i]);
        }
        printf("\n");

        offset += nElements;
        // space = nElements - 1;
        nElements *= 2;
    }
}

inline void swap(int *a, int *b)  //__attribute__((always_inline));
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void maxHeapify(pair_t *heap, int size, int i) {
    pair_t temp;
    while (1) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < size && heap[left].key > heap[largest].key) largest = left;

        if (right < size && heap[right].key > heap[largest].key)
            largest = right;

        if (largest != i) {
            temp.key = heap[i].key;
            temp.val = heap[i].val;
            heap[i].key = heap[largest].key;
            heap[i].val = heap[largest].val;
            heap[largest].key = temp.key;
            heap[largest].val = temp.val;
            i = largest;

        } else {
            break;
        }
    }
}



void heapifyUp(pair_t *heap, int pos) {
    float val = heap[pos].key;
    while (pos > 0 && val > heap[parent(pos)].key) {
        heap[pos].key = heap[parent(pos)].key;
        heap[pos].val = heap[parent(pos)].val;
        pos = parent(pos);
    }
    heap[pos].key = val;
}

void insert(pair_t* heap, int size, float element, int val) {
    size += 1;
    int last = size - 1;
    heap[last].key = element;
    heap[last].val = val;
    heapifyUp(heap, last);
}

int isMaxHeap(pair_t *heap, int size) {
    for (int i = 1; i < size; i++)
        if (heap[i].key <= heap[parent(i)].key)
            continue;
        else {
            printf("\nbroke at [%d]=%f\n", i, heap[i].key);
            printf("father at [%d]=%f\n", parent(i), heap[parent(i)].key);
            return 0;
        }
    return 1;
}

void decreaseMax(pair_t *heap, int size, float new_value, int val) {
    if (size == 0)  // Heap is empty
        return;

    if (heap[0].key > new_value) {
        heap[0].key = new_value;
        heap[0].val = val;
        maxHeapify(heap, size, 0);
    }
}

int cmpfunc(const void *a, const void *b) {
    float fa = *(const float *)a;
    float fb = *(const float *)b;
    return (fa > fb) - (fa < fb);
}

int cmpfuncK(const void *A, const void *B) {
    float fa = ((pair_t *)A)->key;
    float fb = ((pair_t *)B)->key;
    return (fa > fb) - (fa < fb);
}

//recebe nTotalThreads heaps e retorna uma max-heap com os k menores elementos
pair_t *get_one_heap(heap_pthread_t **heap_set, int nTotalThreads, int k){
    pair_t *heap = malloc(sizeof(pair_t) * k);

    for(int i = 0; i < k; i++){
        insert(heap, i, heap_set[0]->heap[i].key, heap_set[0]->heap[i].val);
    }
    if(k == 1) return heap;
    for(int i = 1; i < nTotalThreads; i++){
        for(int j = 0; j < k; j++){
            decreaseMax(heap, k, heap_set[i]->heap[j].key, heap_set[i]->heap[j].val);
        }
    }
    return heap;
}

/*
void *threadedMaxHeap(void *args) {
    heap_pthread_t *this = (heap_pthread_t *)args;
    float* inputPointer = this->startPoint;
    int inputIndex = this->inputIndex;

    this->heap = malloc(sizeof(pair_t) * this->sizeHeap);

    for (int i = 0; i < this->sizeHeap; ++i, ++inputPointer, ++inputIndex)
        insert(this->heap, this->sizeHeap, *inputPointer, inputIndex);

    this->sizeSearch -= this->sizeHeap;
    this->startPoint += sizeof(float) * this->sizeHeap;

    for (; inputPointer != this->endPoint; ++inputPointer, ++inputIndex)
        decreaseMax(this->heap, this->sizeHeap, *inputPointer, inputIndex);
}
*/

void *bodyThread(void *arg){
    heap_pthread_t *info = (heap_pthread_t *) arg;
    for(int j = 0; j < info->heapS; j++){
            insert(info->heap, j, info->Input[info->start_index+j], info->start_index+j);            
        }
    for(int i = info->heapS; i < info->sizeSearch; i++){
        decreaseMax(info->heap, info->heapS, info->Input[info->start_index + i], info->start_index + i);
    }
    
    pthread_exit(NULL); //finalização da thread

}

double timestamp(void)
{
  struct timeval tp;
  gettimeofday(&tp, NULL);
  return((double)(tp.tv_sec*1000.0 + tp.tv_usec/1000.0));
}
