#ifndef __THLIB__
#define __THLIB__
#include <pthread.h>
#include "maxheap.h"
#include "knn.h"
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

void limpa_threads(thread_t **threads, int nt);
void junta_resultados(int *R, int k, thread_t **threads, int nt);
thread_t *create_thread(int id, float *Q, int search_size, float *P, 
        int size_p, int k, int dimension, pthread_barrier_t *start);
void *threadKNN(void *args);
void knn(float *Q, int size_q, float *P, int size_p, int dimension, int k, int *R);
#endif