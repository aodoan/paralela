#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>

#include "utils.h"

// para compilar:
// gcc -O3 max-heap.c -o max-heap -lm

#define SHOW_DECREASE_MAX_STEPS 1
#define MAX_HEAP_SIZE (1024 * 1024)
int nTotalElements, k, nThreads;
float *Input;
pair_t *heap;

float* generateInput(int nTotalElements){
    float* vector = malloc(sizeof(float) * nTotalElements);
    int a, b;
    float v;

    srand(1);
    for(int i = 0; i < nTotalElements; i++){
        a = rand();
        b = rand();
        v = a * 100.0 + b;
        //v = a ;
        vector[i] = v;
    } 

    return vector;
}

//get parameters from command line
void getParams(int argc, char **argv){
    if(argc != 4){
        printf("Erro com os paramêtros.\n");
        exit(1);
    }
    nTotalElements = atoi(argv[1]);
    k = atoi(argv[2]);
    nThreads = atoi(argv[3]);   
}

int main(int argc, char **argv){
    double begin = timestamp();
    getParams(argc, argv);
    
    srand(time(NULL));
    Input = generateInput(nTotalElements);
    
    heap_pthread_t **threads = malloc(sizeof(heap_pthread_t*) * nThreads);
    
    int sizeForEach = floor((double)nTotalElements / nThreads);
    int offset = 0;
    
    for(size_t i = 0; i < nThreads; i++){
        threads[i] = malloc(sizeof(heap_pthread_t));
        threads[i]->Input = Input;
        threads[i]->start_index = offset;
        threads[i]->startPoint = &Input[offset];
        if(offset + sizeForEach < nTotalElements){
            threads[i]->endPoint = &Input[offset + sizeForEach];
            threads[i]->end_index = offset+sizeForEach;
        } else {
            threads[i]->endPoint = &Input[nTotalElements+1];
            threads[i]->end_index = nTotalElements;
        }
        
        threads[i]->sizeHeap = k;
        threads[i]->heapS = k;
        threads[i]->sizeSearch = sizeForEach;
        //threads[i]->id = pthread_create(&threads[i]->thread, NULL, threadedMaxHeap, (void*)threads[i]);
        threads[i]->id = pthread_create(&threads[i]->thread, NULL, bodyThread, (void*)threads[i]);
        threads[i]->heap = malloc(sizeof(pair_t) * k);
        

        offset += sizeForEach;
    }
    /*

    for(int i = 0; i < nThreads; i++){
        printf("%i heap: ", i+1);
        for(int j = 0; j < k; j++)
            printf("[%f %i] ", threads[i]->heap[j].key, threads[i]->heap[j].val);
        printf("\n");
    }
    */
    for (int i = 0; i < nThreads; i++) {
        pthread_join(threads[i]->thread, NULL);
    }
    pair_t *Output = get_one_heap(threads, nThreads, k);
    /*
    for(int i = 0; i < k; i++){
        printf("[%f %i] ",Output[i].key, Output[i].val);

    }
    */

    double end = timestamp();

    printf("tempo %f\n", end-begin);
    verifyOutput(Input, Output, nTotalElements, k);
    printf("\n");
    return 0;
}
