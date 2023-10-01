// Autores
// Claudinei Aparecido Alduan Filho GRR20203920
// Rodrigo Saviam Soffner GRR20205092

#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "chrono.h"
#include "utils.h"

// 1 -> roda o verify
// 2 -> skipa verify
#define TEST_OUTPUT 1

int nTotalElements, k, nThreads;
float *Input;
pair_t *Output;

heap_pthread_t *create_heap_pthread(int offset, int sizeForEach, int i) {
    heap_pthread_t *thread = malloc(sizeof(heap_pthread_t));

    thread->startIndex = offset;
    thread->startPoint = &Input[offset];
    thread->heapSize = k;
    thread->searchSize = sizeForEach;
    thread->heap = malloc(sizeof(pair_t) * k);
    thread->id = i;
    int res =
        pthread_create(&thread->thread, NULL, threadedMaxHeap, (void *)thread);
    if (res) {
        printf("Erro ao criar thread %i\n", i);
        exit(1);
    }

    return thread;
}

float *generateInput(int nTotalElements) {
    float *vector = malloc(sizeof(float) * nTotalElements);
    int a, b;
    float v;

    srand(1);
    for (int i = 0; i < nTotalElements; i++) {
        a = rand();
        b = rand();
        v = a * 100.0 + b;
        vector[i] = v;
    }

    return vector;
}

// get parameters from command line
void getParams(int argc, char **argv) {
    if (argc != 4) {
        printf("Erro com os paramêtros.\n");
        exit(1);
    }
    nTotalElements = atoi(argv[1]);
    k = atoi(argv[2]);
    nThreads = atoi(argv[3]);
}

int main(int argc, char **argv) {
    getParams(argc, argv);

    srand(time(NULL));

    Input = generateInput(nTotalElements);
    chronometer_t chrono;
    chrono_reset(&chrono);
    chrono_start(&chrono);

    if (nThreads > 1) {
#if TEST_OUTPUT == 1
        printf("paralelizado com %i threads\n", nThreads);
#endif
        heap_pthread_t **threads = malloc(sizeof(heap_pthread_t *) * nThreads);

        int offset = 0;
        int sizeForEach = floor((double)nTotalElements / nThreads);

        for (int i = 0; i < nThreads; i++) {
            threads[i] = create_heap_pthread(offset, sizeForEach, i);
            offset += sizeForEach;
        }

        for (int i = 0; i < nThreads; i++) {
            pthread_join(threads[i]->thread, NULL);
        }

        Output = join_heaps(threads, nThreads, k);

    } else {
#if TEST_OUTPUT == 1
        printf("sequencial\n");
#endif
        Output = sequencial(Input, nTotalElements, k);
    }

    chrono_stop(&chrono);

    chrono_reportTime(&chrono, "parallelReductionTime");
    double total_time_in_seconds =
        (double)chrono_gettotal(&chrono) / ((double)1000 * 1000 * 1000);
    printf("total_time_in_seconds: %lf s\n", total_time_in_seconds);

    double OPS = (nTotalElements) / total_time_in_seconds;
    printf("Throughput: %lf OP/s\n", OPS);

#if TEST_OUTPUT == 1
    verifyOutput(Input, Output, nTotalElements, k);
    printf("\n");
#endif
    return 0;
}
