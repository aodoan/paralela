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
#include <math.h>
#include "maxheap.h"


//define qual processo envia para os outros
#define ROOT_PROCESS 0

#define IMPRIMIR_MATRIZES 0
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
        if(P[i*dimension] != FLT_MAX){
            size = 0;
            for(int j = 0; j < size_p; j++){
                //passa a linha I da matriz P
                //passa a linha J da matriz Q
                val = calculate_distance(P, Q, dimension, j*dimension, i*dimension);
                //a heap ainda nao chegou no tamanho maximo insere o elemento e roda o max-heapify
                if(size < k)
                    insert(heap, &size, val);
                //a heap chegou no tamanho maximo, precisa rodar o decreaseMax
                else{
                    decreaseMax(heap, &size, &k, val);
                }
            }
            
            //TODO usar o memcpy aqui deu preguiça
            //coloca a heap na matriz resposta
            for(int j = 0; j < k; j++){
                R[i*k + j] = heap[j];
            }
        }
        else{
            for(int j = 0; j < k; j++){
                R[i*k + j] = FLT_MAX;
            }
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
    printf("|Q|:%i |P|:%i D:%i k:%i\n", size_q, size_p, dimension, k);

    float *P = (float *)malloc(size_p * dimension * sizeof(float));
    if (P == NULL) printf("Malloc erro");
    
    //matriz local 
    float *local_Q = (float *)malloc(size_q * dimension * sizeof(float));
    if (local_Q == NULL) printf("Malloc erro");

    //matriz PARCIAL da resposta (cada no tem uma)
    float *local_R = (float *)malloc(size_q * k * sizeof(float));
    if (local_R == NULL) printf("Malloc erro");
    
    //se for o processo 0, gera o P e o Q
    float *Q;
    if(my_rank == ROOT_PROCESS){
        Q = (float *)malloc(size_q * dimension * sizeof(float));
        
        geraConjuntoDeDados(Q, size_q, dimension);
        geraConjuntoDeDados(P, size_p, dimension);
    }
    
    int displ[n_proc];
    int sizes[n_proc];
    int extra = size_q - ((int) floor((float)size_q / (float)n_proc) * n_proc);
    int size_qq = (int) ceil((float)size_q / (float)n_proc);

    displ[0] = 0;
    sizes[0] = size_q - extra;

    for(int i = 1; i < n_proc; ++i){
        sizes[i] = size_qq;
        displ[i] = displ[i-1] + size_qq;
    }

    for(int i = 0; i < n_proc; ++i){
        sizes[i] = sizes[i] * dimension;
        displ[i] = displ[i] * size_qq ;
    }

    //Faz o scatter da matriz Q para todos os processos
    MPI_Scatterv(Q, sizes, displ, MPI_FLOAT, 
    local_Q, size_q * dimension, MPI_FLOAT, ROOT_PROCESS,
    MPI_COMM_WORLD);
    
   
    //Faz o broadcast para todos os processo da matriz P (database)
    MPI_Bcast(P, size_p * dimension, MPI_FLOAT, ROOT_PROCESS, MPI_COMM_WORLD);

    if(my_rank == ROOT_PROCESS){
        knn(local_Q, (size_q - extra) * dimension, P, size_p, dimension, k, local_R);
    } else {
        knn(local_Q, size_q * dimension, P, size_p, dimension, k, local_R);
    }

    if(IMPRIMIR_MATRIZES){
    printf("Q\n");
    for(int i = 0; i < size_q/n_proc; i++){
        for (int j = 0; j < dimension; j++){
            printf("%f ", local_Q[i*dimension + j]);
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

    printf("alo\n");
    //junta todo no processo raiz
    if(my_rank == 0){
        float *R = (float *)malloc(size_q * k * sizeof(float));

        MPI_Gatherv(local_R, size_q * k, MPI_FLOAT, 
        R, sizes, displ, MPI_FLOAT, ROOT_PROCESS, MPI_COMM_WORLD);
        verificaKNN(local_Q, size_q, P, size_p, dimension, k, R); 
    }
    else{
        MPI_Gather(local_R, size_q * k, MPI_FLOAT, 
        NULL, size_q * k, MPI_FLOAT, ROOT_PROCESS, MPI_COMM_WORLD);
    }

    MPI_Finalize();
}