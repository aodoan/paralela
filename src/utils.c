#include "utils.h"

#include <math.h>
#include <stdio.h>

#define SHOW_DECREASE_MAX_STEPS 1

#define MAX_HEAP_SIZE (1024 * 1024)

void drawHeapTree(int heap[], int size, int nLevels)  // FIX ME!
{
    int offset = 0;
    //int space = (int)pow(2, nLevels - 1);

    int nElements = 1;
    for (int level = 0; level < nLevels; level++) {
        // print all elements in this level
        for (int i = offset; i < size && i < (offset + nElements); i++) {
            printf("[%3d]", heap[i]);
        }
        printf("\n");

        offset += nElements;
        //space = nElements - 1;
        nElements *= 2;
    }
}

inline void swap(int *a, int *b)  //__attribute__((always_inline));
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void maxHeapify(int heap[], int size, int i) {
    while (1) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < size && heap[left] > heap[largest]) largest = left;

        if (right < size && heap[right] > heap[largest]) largest = right;

        if (largest != i) {
            swap(&heap[i], &heap[largest]);  // Using the swap function
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

void decreaseMax(int heap[], int size, int new_value) {
    // Heap is empty
    if (size == 0) return;

    if (heap[0] <= new_value) return;

    heap[0] = new_value;
    maxHeapify(heap, size, 0);
}