// Autores
// Claudinei Aparecido Alduan Filho GRR20203920
// Rodrigo Saviam Soffner GRR20205092

#include "maxheap.h"

void maxHeapify(float *heap, int size, int i) {
    float temp;
    while (1) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < size && heap[left] > heap[largest])
            largest = left;

        if (right < size && heap[right] > heap[largest])
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

float* createMaxHeap(int capacity){
    float *heap = (float *)malloc(sizeof(float) * capacity);
    //for(int i = 0; i < capacity; i++) heap[i] = FLT_MAX;
    return heap;
}

void decreaseMax(float *heap, int *size, int *capacity, float new_value) {
    if (*size < *capacity){
        *size = *size + 1;
    }
        
    if (heap[0] > new_value) {
        heap[0] = new_value;
        maxHeapify(heap, *size, 0);
    }
}


int parent(int pos) { return ((pos - 1) / 2); }

void heapifyUp(float *heap, int pos) {
    float val = heap[pos];
    while (pos > 0 && val > heap[parent(pos)]) {
        heap[pos] = heap[parent(pos)];
        pos = parent(pos);
    }
    heap[pos] = val;
}

void insert(float *heap, int *size, float element) {
    *size += 1;
    int last = *size - 1;
    heap[last] = element;
    heapifyUp(heap, last);
}