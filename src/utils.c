#include "utils.h"

#include <math.h>
#include <stdio.h>

#define SHOW_DECREASE_MAX_STEPS 1

#define MAX_HEAP_SIZE (1024 * 1024)


void maxHeapify(pair_t* heap, int size, int i){
    pair_t temp;
    while (1) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < size && heap[left].key > heap[largest].key) largest = left;

        if (right < size && heap[right].key > heap[largest].key) largest = right;

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

// #define parent(pos) ( pos/2 ) // SE nao usar posicao 0
inline int parent(int pos) { return ((pos - 1) / 2); }

void heapifyUp(int heap[], int *size, int pos) {
    int val = heap[pos];
    while (pos > 0 && val > heap[parent(pos)]) {
        heap[pos] = heap[parent(pos)];
        pos = parent(pos);
    }
    heap[pos] = val;
}

void insert(int heap[], int *size, int element) {
    *size += 1;
    int last = *size - 1;

    heap[last] = element;
    heapifyUp(heap, size, last);
}

int isMaxHeap(int heap[], int size) {
    for (int i = 1; i < size; i++)
        if (heap[i] <= heap[parent(i)])
            continue;
        else {
            printf("\nbroke at [%d]=%d\n", i, heap[i]);
            printf("father at [%d]=%d\n", parent(i), heap[parent(i)]);
            return 0;
        }
    return 1;
}
void decreaseMax(pair_t* heap, int size, float new_value, int val) {
    if (size == 0)  // Heap is empty
        return;

    if (heap[0].key > new_value) {
        heap[0].key = new_value;
        heap[0].val = val;
        maxHeapify(heap, size, 0);
    }
}


int cmpfunc (const void * a, const void * b)
{
  float fa = *(const float*) a;
  float fb = *(const float*) b;
  return (fa > fb) - (fa < fb);
}
