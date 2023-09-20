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
        a = rand();
        b = rand();
        v = a * 100.0 + b;
        Input[i] = v;
    } 
    pair_t **heap = malloc(sizeof(pair_t*) * nThreads);
    for(int i = 0; i < nThreads; i++)
        heap[i] = malloc(sizeof(pair_t) * k);
    
    int sizeOfDistribution = floor(nTotalElements / nThreads);
    //cria uma max heap com os primeiros k elementos do vetor de entrada
    
    for(int j = 0, offset = 0; j < nThreads; j++){
        for(int i = 0; i < k; i++){
            insert(heap[j], i, Input[i+offset], i+offset);
        }
        offset += sizeOfDistribution;
    }
    

    for(int i = 0; i < nThreads; i++){
        printf("1 heap: ");
        for(int j = 0; j < k && j < nTotalElements; j++)
            printf("[%f %i] ", heap[i][j].key, heap[i][j].val);
        printf("\n");
    }
    
    //itera sobre o restante do vetor
    /*
    for(int i = k; i < nTotalElements; i++)
        decreaseMax(heap[0], k, Input[i], i);
    
    //qsort(Input, nTotalElements, sizeof(float), cmpfunc);
    //colocar ifdef aqui
    verifyOutput(Input, heap[0], nTotalElements, k);
    */
    pair_t *Output = get_one_heap(heap, nTotalElements, k);
    for(int i = 0; i < k; i++){
        printf("[%f %i] ",Output[i].key, Output[i].val);

    }
    printf("\n");
    return 0;
}