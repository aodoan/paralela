// Autores
// Claudinei Aparecido Alduan Filho GRR20203920
// Rodrigo Saviam Soffner GRR20205092

#include "maxheap.h"

void maxHeapify(pair_t *heap, int size, int i) {
    pair_t temp;
    while (1) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < size && heap[left].dist > heap[largest].dist)
            largest = left;

        if (right < size && heap[right].dist > heap[largest].dist)
            largest = right;

        if (largest != i) {
            temp = heap[i];
            heap[i] = heap[largest];
            heap[largest] = temp;
            i = largest;

        } else {
            break;
        }
    }
}

pair_t* createMaxHeap(int capacity){
    pair_t *heap = (pair_t *)malloc(sizeof(pair_t) * capacity);
    //for(int i = 0; i < capacity; i++) heap[i] = FLT_MAX;
    return heap;
}

void decreaseMax(pair_t *heap, int *size, int *capacity, float dist, int index) {
    if (*size < *capacity){
        *size = *size + 1;
    }
        
    if (heap[0].dist > dist) {
        heap[0].dist = dist;
        heap[0].index = index;
        maxHeapify(heap, *size, 0);
    }
}


int parent(int pos) { return ((pos - 1) / 2); }

void heapifyUp(pair_t *heap, int pos) {
    pair_t temp = heap[pos];

    while (pos > 0 && temp.dist > heap[parent(pos)].dist) {
        heap[pos] = heap[parent(pos)];
        pos = parent(pos);
    }

    heap[pos] = temp;
}

void insert(pair_t *heap, int *size, float dist, int index) {
    *size += 1;
    int last = *size - 1;
    heap[last].dist = dist;
    heap[last].index = index;
    heapifyUp(heap, last);
}