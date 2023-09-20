#include <math.h>
#include <stdio.h>
#include "utils.h"

// para compilar:
// gcc -O3 max-heap.c -o max-heap -lm

#define SHOW_DECREASE_MAX_STEPS 1
#define MAX_HEAP_SIZE (1024 * 1024)
int nTotalElements, k, nThreads;
float *Input;
pair_t *heap;

int main(int argc, char **argv){
    //get parameters from command line
    if(argc != 4){
        printf("Erro com os paramêtros.\n");
        exit(1);
    }
    nTotalElements = atoi(argv[1]);
    k = atoi(argv[2]);
    nThreads = atoi(argv[3]);
    int a, b;
    float v;
    Input = malloc(sizeof(float) * nTotalElements);
    
    srand(time(NULL));
    for(int i = 0; i < nTotalElements; i++){
        a = rand() % 100;
        b = rand() % 100;
        v = a * 1.0 + b;
        //v = 100.0 - i;
        Input[i] = v;
    } 
    printf("Vetor original de entrada:\n");
    for(int i = 0; i < nTotalElements; i++) printf("[%f] ", Input[i]);
    printf("\n=====\n");
    heap = malloc(sizeof(pair_t) * k);

    for(int i = 0; i < k; i++){
        /*
        heap[i].key = Input[i];
        heap[i].val = i;
        */
        printf("passei uma vez\n");
        insert(heap, i, Input[i]);
    }
    //maxHeapify(heap, k, 0);
    printf("%i\n", isMaxHeap(heap, k));
    for(int i = k; i < nTotalElements; i++){
        decreaseMax(heap, k, Input[i], i);
    }
    
    printf("heap de saida\n");
    for(int i = 0; i < k; i++){
        printf("[%f] ", heap[i].key);
    }
    
    printf("\n ===================== \n");

    //qsort(Input, nTotalElements, sizeof(float), cmpfunc);
    for(int i = 0; i < nTotalElements; i++) printf("[%f] ", Input[i]);
    printf("\n");

    verifyOutput(Input, heap, nTotalElements, k);
    return 0;
}