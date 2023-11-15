// Autores
// Claudinei Aparecido Alduan Filho GRR20203920
// Rodrigo Saviam Soffner GRR20205092

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include "maxheap.h"

#define IMPRIMIR_MATRIZES 1
//dado dois vetores calcula a distancia entre eles
//OBS como os vetores estao em uma matriz, precisa dar o offset que eles se encontram
//TODO talvez mudar pra dar a linha logo
float calculate_distance(float *a, float *b , int D, int ia, int ib){
    float dist = 0.0, temp;

    for(int i = 0; i < D; i++){
        temp = b[i+ib]-a[i+ia];
        dist = dist + temp*temp;  
    }
    return dist;
}

void geraConjuntoDeDados(float *C, int size, int dimension) {
    int val = 0;
    
    for (int i = 0; i < size; i++){
        for (int j = 0; j < dimension; j++){
            //C[i * dimension + j] = (rand() * 100.0 + rand())/181275869184.000000;
            C[i * dimension + j] = (rand() * 100.0 + rand());
            //C[i * dimension + j] = val;
            val++;
        }
    }
}

void verificaKNN(float *Q, int size_q, float *P, int size_pacharKNN, int dimension,
                 int k, float *R) {
    // note que R tem nq linhas por k colunas, para qualquer tamanho de k
    // (colunas) entao é linearizado para acesso como um VETOR
    printf(" ------------ VERIFICA KNN --------------- \n");
 
    for (int i = 0; i < size_q; i++) {
        printf("knn[%d]: ", i);
        for (int j = 0; j < k; j++) printf("%f ", R[i * k + j]);
        printf("\n");
    }

}


//itera sobre o conjunto P para todo ponto Q
void knn(float *Q, int size_q, float *P, int size_p, int dimension, int k, float *R) {
    double val;
    int size; // indica o numero de elementos na heap
    float *heap; //ponteiro usado para representar heap
    heap = createMaxHeap(k);
    for(int i = 0; i < size_q; i++){
        size = 0;
        for(int j = 0; j < size_p; j++){
            //passa a linha I da matriz P
            //passa a linha J da matriz Q
            val = calculate_distance(P, Q, dimension, j*dimension, i*dimension);
            printf("%f ", val);
            //a heap ainda nao chegou no tamanho maximo insere o elemento e roda o max-heapify
            if(size < k)
                insert(heap, &size, val);
            //a heap chegou no tamanho maximo, precisa rodar o decreaseMax
            else{
                decreaseMax(heap, &size, &k, val);
            }
        }
        printf("\n");
        //TODO usar o memcpy aqui deu preguiça
        //coloca a heap na matriz resposta
        for(int j = 0; j < k; j++){
            R[i*k + j] = heap[j];
        }
    }


}

int main(int argc, char **argv) {
    int size_q = atoi(argv[1]);
    int size_p = atoi(argv[2]);
    int dimension = atoi(argv[3]);
    int k = atoi(argv[4]);
    
    int i, my_rank, n_proc;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_proc);
    
    
    srand(1);
    printf("|Q|:%zu |P|:%zu D:%zu k:%zu\n", size_q, size_p, dimension, k);

    /*
    float *P = (float *)malloc(size_p * dimension * sizeof(float));
    if (P == NULL) printf("Malloc erro");

    float *Q = (float *)malloc(size_q * dimension * sizeof(float));
    if (P == NULL) printf("Malloc erro");

    float *R = (float *)malloc(size_q * k * sizeof(float));
    if (P == NULL) printf("Malloc erro");
    */
    //matriz contendo uma parcela dos pontos
    float *P = (float *)malloc(size_p * dimension * sizeof(float));
    if (P == NULL) printf("Malloc erro");

    //matriz contendo todos os pontos de busca
    float *local_Q = (float *)malloc(size_q/n_proc * dimension * sizeof(float));
    if (local_Q == NULL) printf("Malloc erro");

    //matriz PARCIAL da resposta (cada no tem uma)
    float *R_local = (float *)malloc(size_q/n_proc * k * sizeof(float));
    if (R_local == NULL) printf("Malloc erro");
    //se for o processo 0, gera o P e o Q
    float *Q;
    if(my_rank == 0){
        Q = (float *)malloc(size_q * dimension * sizeof(float));
        geraConjuntoDeDados(Q, size_q, dimension);
        
    }
    printf("size %i\n", size_q*dimension/n_proc);
    MPI_Scatter(Q, size_q*dimension/n_proc, MPI_FLOAT, 
    local_Q, size_q*dimension/n_proc, MPI_FLOAT, 0,
    MPI_COMM_WORLD);
    

    for(int i = 0; i < size_q/n_proc; i++){
        for(int j = 0; j < dimension; j++){
            printf("[%i]%f ", my_rank, local_Q[i*dimension + j]);
        }
        printf("\n");
    }
    /*
    geraConjuntoDeDados(P, size_p, dimension);
    knn(local_Q, size_q/n_proc, P, size_p, dimension, k, R_local);

    if(IMPRIMIR_MATRIZES){
    printf("Q\n");
    for(int i = 0; i < size_q; i++){
        for (int j = 0; j < dimension; j++){
            printf("%f ", Q[i*dimension + j]);
        }
        printf("\n");
    }
    printf("\nP\n");
    for(int i = 0; i < size_p; i++){
        for (int j = 0; j < dimension; j++){
            printf("%f ", P[i*dimension + j]);
        }
        printf("\n");
    }
    }
    verificaKNN(local_Q, size_q/n_proc, P, size_p, dimension, k, R_local);

    free(P);
    free(Q);
    */

    MPI_Finalize();
}