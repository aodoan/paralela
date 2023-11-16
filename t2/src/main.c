// Autores
// Claudinei Aparecido Alduan Filho GRR20203920
// Rodrigo Saviam Soffner GRR20205092

#include <math.h>
#include <mpi.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

#include "chrono.h"
#include "maxheap.h"

// define qual processo envia para os outros
#define ROOT_PROCESS 0

#define IMPRIMIR_MATRIZES 0
// dado dois vetores calcula a distancia entre eles
// OBS como os vetores estao em uma matriz, precisa dar o offset que eles se
// encontram
// TODO talvez mudar pra dar a linha logo
float calculate_distance(float *a, float *b, int D, int ia, int ib) {
    float dist = 0.0, temp;

    for (int i = 0; i < D; i++) {
        temp = b[i + ib] - a[i + ia];
        dist = dist + temp * temp;
    }
    return dist;
}

void geraConjuntoDeDados(float *C, int size, int dimension) {
    int a, b;
    float v;

    for (size_t i = 0; i < size * dimension; i++) {
        a = rand();
        b = rand();
        v = a * 100.0 + b;
        C[i] = v;
    }
}

void verificaKNN(float *Q, int size_q, float *P, int size_pacharKNN,
                 int dimension, int k, float *R) {
    // note que R tem nq linhas por k colunas, para qualquer tamanho de k
    // (colunas) entao é linearizado para acesso como um VETOR
    printf(" ------------ VERIFICA KNN --------------- \n");

    for (int i = 0; i < size_q; i++) {
        printf("knn[%d]: ", i);
        for (int j = 0; j < k; j++) printf("%f ", R[i * k + j]);
        printf("\n");
    }
}

// itera sobre o conjunto P para todo ponto Q
void knn(float *Q, int size_q, float *P, int size_p, int dimension, int k,
         int *R) {
    double dist;
    int size;     // indica o numero de elementos na heap
    pair_t *heap;  // ponteiro usado para representar heap
    heap = createMaxHeap(k);
    for (int i = 0; i < size_q; i++) {
        if (P[i * dimension] != FLT_MAX) {
            size = 0;
            for (int j = 0; j < size_p; j++) {
                // passa a linha I da matriz P
                // passa a linha J da matriz Q
                dist = calculate_distance(P, Q, dimension, j * dimension,
                                         i * dimension);
                // a heap ainda nao chegou no tamanho maximo insere o elemento e
                // roda o max-heapify
                if (size < k) insert(heap, &size, dist, j);
                // a heap chegou no tamanho maximo, precisa rodar o decreaseMax
                else {
                    decreaseMax(heap, &size, &k, dist, j);
                }
            }

            // Coloca os index dos k vizinhos mais proximos no vetor R
            for (int j = 0; j < k; j++) {
                R[i * k + j] = heap[j].index;
            }
        } else {
            for (int j = 0; j < k; j++) {
                R[i * k + j] = -1;
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
    chronometer_t chrono;

    chrono_reset(&chrono);

    srand(1);
    printf("|Q|:%i |P|:%i D:%i k:%i\n", size_q, size_p, dimension, k);

    int size_q_divisable =
        ((int)ceil(((float)size_q) / ((float)n_proc))) * n_proc;
    int size_q_proc = size_q_divisable / n_proc;

    printf("%i | %i", size_q_divisable, size_q_proc);

    float *P = (float *)malloc(size_p * dimension * sizeof(float));
    if (P == NULL) printf("Malloc erro");

    // matriz local
    float *local_Q = (float *)malloc(size_q_proc * dimension * sizeof(float));
    if (local_Q == NULL) printf("Malloc erro");

    // matriz PARCIAL da resposta (cada no tem uma)
    int *local_R = (int *)malloc(size_q_proc * k * sizeof(int));
    if (local_R == NULL) printf("Malloc erro");

    // se for o processo 0, gera o P e o Q
    float *Q;
    if (my_rank == ROOT_PROCESS) {
        Q = (float *)malloc(size_q_divisable * dimension * sizeof(float));

        geraConjuntoDeDados(Q, size_q, dimension);
        geraConjuntoDeDados(P, size_p, dimension);

        for (int i = size_q * dimension; i < size_q_divisable * dimension;
             ++i) {
            Q[i] = FLT_MAX;
        }
    }

    // Começa a contar o tempo
    chrono_start(&chrono);

    // Faz o scatter da matriz Q para todos os processos
    MPI_Scatter(Q, size_q_proc * dimension, MPI_FLOAT, local_Q,
                size_q_proc * dimension, MPI_FLOAT, ROOT_PROCESS,
                MPI_COMM_WORLD);

    // Faz o broadcast para todos os processo da matriz P (dataset)
    MPI_Bcast(P, size_p * dimension, MPI_FLOAT, ROOT_PROCESS, MPI_COMM_WORLD);

    // Calcula o KNN dos pontos Q locais
    knn(local_Q, size_q_proc, P, size_p, dimension, k, local_R);

    if (IMPRIMIR_MATRIZES) {
        printf("Q\n");
        for (int i = 0; i < size_q / n_proc; i++) {
            for (int j = 0; j < dimension; j++) {
                printf("%f ", local_Q[i * dimension + j]);
            }
            printf("\n");
        }
        printf("\nP\n");
        for (int i = 0; i < size_p; i++) {
            for (int j = 0; j < dimension; j++) {
                printf("%f ", P[i * dimension + j]);
            }
            printf("\n");
        }
    }

    printf("alo\n");
    // junta todo no processo raiz
    int *R;
    if (my_rank == ROOT_PROCESS) {
        R = (int *)malloc(size_q_divisable * k * sizeof(int));
        printf("%i\n", size_q_divisable * k);
    }
    
    // Realiza o Gather de todos os processos MPI
    MPI_Gather(local_R, size_q_proc * k, MPI_INT, R, size_q_proc * k,
               MPI_INT, ROOT_PROCESS, MPI_COMM_WORLD);

    // Para de contar o tempo
    chrono_stop(&chrono);

    // Tem que fazer o gather dos tempos
    // MPI_Gather(&chrono,

#if TEST_OUTPUT == 1
    double total_time_in_seconds =
        (double)chrono_gettotal(&chrono) / ((double)1000 * 1000 * 1000);
    chrono_reportTime(&chrono, "parallelReductionTime");
    double OPS = (nTotalElements) / total_time_in_seconds;
    printf("Throughput: %lf OP/s\n", OPS);
#else
    double total_time_in_seconds =
        (double)chrono_gettotal(&chrono) / ((double)1000 * 1000 * 1000);
    printf("%lf\n", total_time_in_seconds);
#endif

#if TEST_OUTPUT == 1
    if (my_rank != ROOT_PROCESS) {
        verificaKNN(local_Q, size_q, P, size_p, dimension, k, R);
        printf("\n");
    }
#endif

    MPI_Finalize();
}