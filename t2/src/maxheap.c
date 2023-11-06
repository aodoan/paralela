// Autores
// Claudinei Aparecido Alduan Filho GRR20203920
// Rodrigo Saviam Soffner GRR20205092

#include "maxheap.h"

void maxHeapify(maxheap_t *heap, int size, int i) {
    float temp;
    while (1) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < size && heap->heap[left] > heap->heap[largest])
            largest = left;

        if (right < size && heap->heap[right] > heap->heap[largest])
            largest = right;

        if (largest != i) {
            temp = heap->heap[i];
            heap->heap[i] = heap->heap[largest];
            heap->heap[largest] = temp;
            i = largest;

        } else {
            break;
        }
    }
}

maxheap_t* createMaxHeap(size_t capacity){
    maxheap_t *heap = (maxheap_t *)malloc(sizeof(maxheap_t));
    heap->capacity = capacity;
    heap->size = 0;
    heap->heap = (float *)malloc(sizeof(float) * capacity);
    return heap;
}

void decreaseMax(maxheap_t *heap, float new_value) {
    if (heap->size < heap->capacity) heap->size++;

    if (heap->heap[0] > new_value) {
        heap->heap[0] = new_value;
        maxHeapify(heap, heap->size, 0);
    }
}