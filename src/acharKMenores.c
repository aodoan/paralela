#include <math.h>
#include <stdio.h>
#include "utils.h"

// para compilar:
// gcc -O3 max-heap.c -o max-heap -lm

#define SHOW_DECREASE_MAX_STEPS 1
#define MAX_HEAP_SIZE (1024 * 1024)

int main() {
    float heap[MAX_HEAP_SIZE] = {60, 50, 40, 30, 20, 10};
    pair_t Output[3];

    verifyOutput(heap, Output, 0, 0);
}