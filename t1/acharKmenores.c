// Autores
// Claudinei Aparecido Alduan Filho GRR20203920
// Rodrigo Saviam Soffner GRR20205092
// gcc -Wall -o acharKmenores acharKmenores.c -lpthread -lm

#include <math.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//#include "utils.c"
//Injeção do utils.c + structs do utils.h
#define SHOW_DECREASE_MAX_STEPS 1
#define MAX_HEAP_SIZE (1024 * 1024)

typedef struct {
    float key;
    int val;
} pair_t;

typedef struct {
    int id;
    pthread_t thread;
    pair_t *heap;

    pthread_barrier_t *start_barrier;

    float *startPoint;
    int heapSize;
    int searchSize;
    int startIndex;
} heap_pthread_t;

int parent(int pos) { return ((pos - 1) / 2); }

int cmpfuncK(const void *A, const void *B) {
    float fa = ((pair_t *)A)->key;
    float fb = ((pair_t *)B)->key;
    return (fa > fb) - (fa < fb);
}

void verifyOutput(const float *Input, const pair_t *Output, int nTotalElmts,
                  int k) {
    int ok = 1;

    pair_t *sortedInput = malloc(sizeof(pair_t) * nTotalElmts);

    for (int i = 0; i < nTotalElmts; ++i) {
        sortedInput[i].key = Input[i];
        sortedInput[i].val = i;
    }

    pair_t *sortedOutput = malloc(sizeof(pair_t) * k);
    memcpy(sortedOutput, Output, sizeof(pair_t) * k);

    qsort(sortedInput, nTotalElmts, sizeof(pair_t), cmpfuncK);
    qsort(sortedOutput, k, sizeof(pair_t), cmpfuncK);

    // verify if the heap is correct
    for (int i = 0; i < k; i++) {
        if (sortedInput[i].key != sortedOutput[i].key) ok = 0;
    }

    if (ok == 1)
        printf("\nOutput set verified correctly.\n");
    else
        printf("\nOutput set DID NOT compute correctly!!!\n");
}

void maxHeapify(pair_t *heap, int size, int i) {
    pair_t temp;
    while (1) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < size && heap[left].key > heap[largest].key) largest = left;

        if (right < size && heap[right].key > heap[largest].key)
            largest = right;

        if (largest != i) {
            temp.key = heap[i].key;
            temp.val = heap[i].val;
            heap[i].key = heap[largest].key;
            heap[i].val = heap[largest].val;
            heap[largest].key = temp.key;
            heap[largest].val = temp.val;
            i = largest;

        } else {
            break;
        }
    }
}

void heapifyUp(pair_t *heap, int pos) {
    float val = heap[pos].key;
    while (pos > 0 && val > heap[parent(pos)].key) {
        heap[pos].key = heap[parent(pos)].key;
        heap[pos].val = heap[parent(pos)].val;
        pos = parent(pos);
    }
    heap[pos].key = val;
}

void insert(pair_t *heap, int size, float element, int val) {
    size += 1;
    int last = size - 1;
    heap[last].key = element;
    heap[last].val = val;
    heapifyUp(heap, last);
}

void decreaseMax(pair_t *heap, int size, float new_value, int val) {
    if (size == 0)  // Heap is empty
        return;

    if (heap[0].key > new_value) {
        heap[0].key = new_value;
        heap[0].val = val;
        maxHeapify(heap, size, 0);
    }
}

// recebe nTotalThreads heaps e retorna uma max-heap com os k menores elementos
pair_t *join_heaps(heap_pthread_t **heap_set, int nTotalThreads, int k) {
    pair_t *heap = malloc(sizeof(pair_t) * k);

    for (int i = 0; i < k; i++) {
        insert(heap, i, heap_set[0]->heap[i].key, heap_set[0]->heap[i].val);
    }
    if (k == 1) return heap;
    for (int i = 1; i < nTotalThreads; i++) {
        for (int j = 0; j < k; j++) {
            decreaseMax(heap, k, heap_set[i]->heap[j].key,
                        heap_set[i]->heap[j].val);
        }
    }
    return heap;
}

//pThread body realiza o max-heap em uma região delimitada do Input. Atua em cima da struct heap_pthread_t passada como ponteiro void
void *threadedMaxHeap(void *args) {
    heap_pthread_t *this = (heap_pthread_t *)args;
    float *inputPointer = this->startPoint;
    int inputIndex = this->startIndex;
    int i = 0;
    
    pthread_barrier_wait(this->start_barrier);    

    for (; i < this->heapSize; ++i, ++inputPointer, ++inputIndex){
        insert(this->heap, i, *inputPointer, inputIndex);
    }

    for (; i < this->searchSize; ++inputPointer, ++inputIndex, ++i){
        decreaseMax(this->heap, this->heapSize, *inputPointer, inputIndex);
    }

    pthread_barrier_wait(this->start_barrier);
    pthread_exit(NULL);
}
//Fim do arquivo utils.c

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