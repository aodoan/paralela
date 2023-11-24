// Autores
// Claudinei Aparecido Alduan Filho GRR20203920
// Rodrigo Saviam Soffner GRR20205092

#ifndef MAXHEAP_H
#define MAXHEAP_H
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <pthread.h>

typedef struct {
    float dist;
    int index;
} pair_t;

//estrutura responsavel por armazenar info 
//de uma thread
typedef struct{
    int id; //id de cada thread
    int my_rank; //rank do processo MPI que o criou (provavelmente nao vai usar)

    pthread_t thread;
    pthread_barrier_t *start;
    float *start_point; //ponteiro para o inicio de sua area de busca de Q
    int offset; 
    int search_size; //quantos pontos Q tem 
    float *P; //ponteiro para a matriz de pontos dataset
    int size_p; 
    int *local_R; //ponteiro para os k pontos mais pertos de local_q
    int dimension;
    int k;

} thread_t;

pair_t* createMaxHeap(int capacity);
void decreaseMax(pair_t *heap, int *size, int *capacity, float dist, int index);
void insert(pair_t *heap, int *size, float dist, int index);
#endif
