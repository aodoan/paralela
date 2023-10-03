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

// 1 -> Modo de check de output
// 2 -> Modo de metricas
#define TEST_OUTPUT 2

int nTotalElements, k, nThreads;
float *Input;
pair_t *Output;

heap_pthread_t *create_heap_pthread(int offset, int sizeForEach, int i,
                                    pthread_barrier_t *start_barrier) {
    heap_pthread_t *thread = malloc(sizeof(heap_pthread_t));

    thread->startIndex = offset;
    thread->startPoint = &Input[offset];
    thread->heapSize = k;
    thread->searchSize = sizeForEach;
    thread->start_barrier = start_barrier;
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
    if (nTotalElements <= 0) {
        printf("nTotalElements deve ser maior que 0\n");
        exit(1);
    }
    if (k <= 0) {
        printf("k deve ser maior que 0\n");
        exit(1);
    }
    if (nThreads <= 0) {
        printf("nThreads deve ser maior que 0\n");
        exit(1);
    }
}

int main(int argc, char **argv) {
    getParams(argc, argv);

    srand(time(NULL));
    Input = generateInput(nTotalElements);
    chronometer_t chrono;

    chrono_reset(&chrono);

#if TEST_OUTPUT == 1
    printf("paralelizado com %i threads\n", nThreads);
#endif
    pthread_barrier_t start_barrier;
    pthread_barrier_init(&start_barrier, NULL, nThreads + 1);
    heap_pthread_t **threads = malloc(sizeof(heap_pthread_t *) * nThreads);

    int offset = 0;
    int sizeForEach = floor((double)nTotalElements / nThreads);

    for (int i = 0; i < nThreads; i++) {
        threads[i] =
            create_heap_pthread(offset, sizeForEach, i, &start_barrier);
        offset += sizeForEach;
    }

    // Realizar os calculos de tempo de execução mais precisamente
    pthread_barrier_wait(&start_barrier);
    chrono_start(&chrono);
    pthread_barrier_wait(&start_barrier);
    chrono_stop(&chrono);

    for (int i = 0; i < nThreads; i++) {
        pthread_join(threads[i]->thread, NULL);
    }

    Output = join_heaps(threads, nThreads, k);

#if TEST_OUTPUT == 1
    chrono_reportTime(&chrono, "parallelReductionTime");
    double OPS = (nTotalElements) / total_time_in_seconds;
    printf("Throughput: %lf OP/s\n", OPS);
#else
    double total_time_in_seconds =
        (double)chrono_gettotal(&chrono) / ((double)1000 * 1000 * 1000);
    printf("%lf\n", total_time_in_seconds);
#endif

#if TEST_OUTPUT == 1
    verifyOutput(Input, Output, nTotalElements, k);
    printf("\n");
#endif
    return 0;
}
