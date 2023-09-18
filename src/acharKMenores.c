#include <math.h>
#include <stdio.h>
#include "utils.h"

// para compilar:
// gcc -O3 max-heap.c -o max-heap -lm

#define SHOW_DECREASE_MAX_STEPS 1

#define MAX_HEAP_SIZE (1024 * 1024)

int main() {
    int heap[MAX_HEAP_SIZE] = {60, 50, 40,
                               30, 20, 10};  //{70, 40, 50, 30, 20, 10};
    int heapSize = 6;

    printf(">>>Max-Heap before decrease operation:\n");
    for (int i = 0; i < heapSize; ++i) {
        printf("%d ", heap[i]);
    }
    printf("\n");

    printf("------Max-Heap Tree------\n");
    drawHeapTree(heap, heapSize, 3);

    decreaseMax(heap, heapSize, 5);  // Decreasing the maximum value to 5

    printf("\nMax-Heap after decrease operation:\n");
    for (int i = 0; i < heapSize; ++i) {
        printf("%d ", heap[i]);
    }
    printf("\n");

    printf("------Max-Heap Tree------\n");
    drawHeapTree(heap, heapSize, 3);

    printf("=========================\n");
    int data[] = {40, 10, 30, 70, 50, 20, 4, 5, 44, 40, 55, 50};
    int n = sizeof(data) / sizeof(data[0]);

    // empty heap
    heapSize = 0;
    printf("EMPTY heap!\n");

    printf("will insert: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", data[i]);
    }
    printf("\n");

    for (int i = 0; i < n; i++) {
        printf("inserting %d\n", data[i]);
        insert(heap, &heapSize, data[i]);
        printf("------Max-Heap Tree------ ");
        if (isMaxHeap(heap, heapSize))
            printf("is a heap!\n");
        else
            printf("is NOT a heap!\n");
    }

#ifndef SHOW_DECREASE_MAX_STEPS
        drawHeapTree(heap, heapSize, 4);
#endif
#ifdef SHOW_DECREASE_MAX_STEPS
    drawHeapTree(heap, heapSize, 4);
#endif

    printf("=========================\n");
    printf("=====decreaseMAX tests===\n");
    int data2[] = {4, 10, 30, 70, 55, 20, 4, 5, 25};
    n = sizeof(data2) / sizeof(data2[0]);

    printf("will decreaseMAX to the following values: ");
    for (int i = 0; i < n; i++) {
        printf("%d ", data2[i]);
    }
    printf("\n");

    for (int i = 0; i < n; i++) {
        printf("decreaseMAX to %d\n", data2[i]);
        int new_value = data2[i];
        decreaseMax(heap, heapSize, new_value);

        printf("------Max-Heap Tree (after decrease)------ ");
        if (isMaxHeap(heap, heapSize))
            printf("is a max heap!\n");
        else
            printf("is NOT a max heap!\n");

        // #ifndef SHOW_DECREASE_MAX_STEPS
        drawHeapTree(heap, heapSize, 4);
        // #endif
    }

    return 0;
}