#ifndef UTILS_H
#define UTILS_H
#include <stdlib.h>
#include <stdio.h>

typedef struct {
    float key;   // inserir um valor v float na chave 
                 //  (obtido do vetor de entrada Input)
    int val;     // inserir a posiçao p como valor val
} pair_t;


void maxHeapify(pair_t* heap, int size, int i);
void heapifyUp(int heap[], int *size, int pos) ;
void insert(int heap[], int *size, int element);
int isMaxHeap(int heap[], int size);
void decreaseMax(pair_t* heap, int size, float new_value, int val);
int cmpfunc (const void * a, const void * b);
#endif 