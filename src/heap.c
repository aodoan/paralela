#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
typedef struct {
    float key;   // inserir um valor v float na chave 
                 //  (obtido do vetor de entrada Input)
    int val;     // inserir a posiçao p como valor val
} pair_t;

#define n 10
int nTotalElements, k, nThreads;
float *Input;
pair_t *heap;

int cmpfunc (const void * a, const void * b)
{
  float fa = *(const float*) a;
  float fb = *(const float*) b;
  return (fa > fb) - (fa < fb);
}

inline void swap(int *a, int *b)  //__attribute__((always_inline));
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void maxHeapify(pair_t* heap, int size, int i) {
    pair_t temp;
    while (1) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < size && heap[left].key > heap[largest].key) largest = left;

        if (right < size && heap[right].key > heap[largest].key) largest = right;

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

void decreaseMax(pair_t* heap, int size, float new_value, int val) {
    if (size == 0)  // Heap is empty
        return;

    if (heap[0].key > new_value) {
        heap[0].key = new_value;
        heap[0].val = val;
        maxHeapify(heap, size, 0);
    }
}

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
        //v = a * 100.0 + b;
        v = 100.0 - i;
        Input[i] = v;
    } 
    for(int i = 0; i < nTotalElements; i++) printf("[%f] ", Input[i]);
    printf("\n=====\n");
    heap = malloc(sizeof(pair_t) * k);

    for(int i = 0; i < k; i++){
        heap[i].key = Input[i];
        heap[i].val = i;
    }
    maxHeapify(heap, k, 0);
    for(int i = 0; i < nTotalElements; i++){
        decreaseMax(heap, k, Input[i], i);
    }

    for(int i = 0; i < k; i++){
        printf("[%f] ", heap[i].key);
    }
    printf("\n ===================== \n");

    qsort(Input, nTotalElements, sizeof(float), cmpfunc);
    for(int i = 0; i < nTotalElements; i++) printf("[%f] ", Input[i]);
    printf("\n");
    return 0;
}