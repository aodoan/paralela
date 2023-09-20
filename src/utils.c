#include "utils.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SHOW_DECREASE_MAX_STEPS 1
#define MAX_HEAP_SIZE (1024 * 1024)


void verifyOutput(const float *Input, const pair_t *Output, int nTotalElmts,
                  int k) {
    int ok = 1;

    pair_t *sorted = malloc(sizeof(pair_t) * nTotalElmts);
    for(int i = 0; i < nTotalElmts; i++){
        sorted[i].key = Input[i];
        sorted[i].val = i;
    }
    qsort(sorted, nTotalElmts, sizeof(pair_t), cmpfuncK);
    qsort(Output, k, sizeof(pair_t), cmpfuncK);

    for(int i = 0; i < k; i++){
        if(sorted[i].key != Output[i].key)
            ok = 0;
    }

    // inserir aqui o codigo da verificacao
    // uma implementação possível para o verificador seria
    // (nao precisa seguir essa descrição, voce pode fazer outro método
    //  de verificação)
    //
    // 1) Criar um vetor I de pares (chave, valor)
    //    os os elementos de I devem ser copias
    //    de cada valor (e,p) do vetor de entrada Input
    //    (ou seja, cada valor e que veio da posição p da entrada)
    // 2) Ordenar o vetor I em ordem crescente,
    //    obtendo-se um outro vetor Is (ordenado em ordem crescente de chaves)
    //    usando um algoritmo de ordenação do tipo (chave, valor)
    //    (por exemplo ordenação da stdlib, caso exista)
    // 3) Para cada par (ki,vi) pertencente ao vetor Output
    //      procurar a chave ki dentre K primeiros elementos
    //      de Is.
    //      Se a chave Ki estiver em Is com valor val==vi continue
    //      senão faça ok = 0 e reporte o erro abaixo

    if (ok)
        printf("\nOutput set verifyed correctly.\n");
    else
        printf("\nOutput set DID NOT compute correctly!!!\n");
}

void populateInput(int nTotalElements) {
    int a, b;
    Input = malloc(sizeof(float) * nTotalElements);
    for (int i = 0; i < nTotalElements; ++i) {
        a = rand();
        b = rand();

        float v = a * 100.0 + b;

        Input[i] = v;
    }
}

void drawHeapTree(int heap[], int size, int nLevels)  // FIX ME!
{
    int offset = 0;
    // int space = (int)pow(2, nLevels - 1);

    int nElements = 1;
    for (int level = 0; level < nLevels; level++) {
        // print all elements in this level
        for (int i = offset; i < size && i < (offset + nElements); i++) {
            printf("[%3d]", heap[i]);
        }
        printf("\n");

        offset += nElements;
        // space = nElements - 1;
        nElements *= 2;
    }
}

inline void swap(int *a, int *b)  //__attribute__((always_inline));
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

void maxHeapify(pair_t *heap, int size, int i) {
    pair_t temp;
    float *Input;
    while (1) {
        int largest = i;
        int left = 2 * i + 1;
        int right = 2 * i + 2;

        if (left < size && heap[left].key > heap[largest].key) 
            largest = left;

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

inline int parent(int pos) { return ((pos - 1) / 2); }

void heapifyUp(pair_t* heap, int *size, int pos) {
    int val = heap[pos].key;
    while (pos > 0 && val > heap[parent(pos)].key) {
        heap[pos].key = heap[parent(pos)].key;
        heap[pos].val = heap[parent(pos)].val;
        pos = parent(pos);
    }
    heap[pos].key = val;
}

void insert(pair_t* heap, int size, float element) {
    size += 1;
    int last = size - 1;
    printf("alo\n");
    heap[last].key = element;
    heap[last].val = size-1;
    heapifyUp(heap, size, last);
}

int isMaxHeap(pair_t* heap, int size) {
    for (int i = 1; i < size; i++)
        if (heap[i].key <= heap[parent(i)].key)
            continue;
        else {
            printf("\nbroke at [%d]=%d\n", i, heap[i].key);
            printf("father at [%d]=%d\n", parent(i), heap[parent(i)].key);
            return 0;
        }
    return 1;
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

int cmpfunc(const void *a, const void *b) {
    float fa = *(const float *)a;
    float fb = *(const float *)b;
    return (fa > fb) - (fa < fb);
}

int cmpfuncK(const void *A, const void *B) {
    pair_t *a = A;
    pair_t *b = B;
    float fa = a->key;
    float fb = b->key;
    return (fa > fb) - (fa < fb);
}
