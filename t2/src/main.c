// Autores
// Claudinei Aparecido Alduan Filho GRR20203920
// Rodrigo Saviam Soffner GRR20205092

#include <math.h>
//#include <mpich/mpi.h>
#include <mpi.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "chrono.h"
#include "maxheap.h"

// define qual processo envia para os outros
#define ROOT_PROCESS 0
#define TEST_OUTPUT 1

#define IMPRIMIR_MATRIZES 0
// dado dois vetores calcula a distancia entre eles
// OBS como os vetores estao em uma matriz, precisa dar o offset que eles se
// encontram
// TODO talvez mudar pra dar a linha logo

float distancia(float *A, float *B, int dimension) {
    float dist = 0.0;
    float temp;
    for (int i = 0; i < dimension; i++) {
        temp = A[i] - B[i];
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

void knn(float *Q, int size_q, float *P, int size_p, int dimension, int k,
         int *R) {
    double dist;
    int size;      // indica o numero de elementos na heap
    pair_t *heap;  // ponteiro usado para representar heap
    heap = createMaxHeap(k);
    for (int i = 0; i < size_q; i++) {
        size = 0;
        for (int j = 0; j < size_p; j++) {
            // passa a linha I da matriz P
            // passa a linha J da matriz Q

            dist = distancia(&P[j * dimension], &Q[i * dimension], dimension);
            // a heap ainda nao chegou no tamanho maximo insere o elemento e
            // roda o max-heapify
            if (size < k) {
                insert(heap, &size, dist, j);
            }
            // a heap chegou no tamanho maximo, precisa rodar o decreaseMax
            else {
                decreaseMax(heap, &size, &k, dist, j);
            }
        }
        // Coloca os index dos k vizinhos mais proximos no vetor R
        for (int j = 0; j < k; j++) {
            R[i * k + j] = heap[j].index;
        }
    }
}
// retorna 1 se deu certo
// 0 caso contrario
void verificaKNN(float *Q, int nq, float *P, int n, int D, int k,
                 int *resultIndices) {
    int correto = 1;

    for (int i = 0; i < nq; i++) {
        // Array para armazenar as distâncias dos k vizinhos mais próximos
        float *distances = (float *)malloc(k * sizeof(float));
        for (int j = 0; j < k; j++) {
            distances[j] = FLT_MAX;
        }

        // Encontrar os k vizinhos mais próximos para Q[i]
        for (int j = 0; j < n; j++) {
            // float dist = distancia(&Q[i], &P[j], D);
            float dist = distancia(&Q[i * D], &P[j * D], D);

            // Verifica se esta distância é menor que a maior distância no array
            // distances
            if (dist < distances[k - 1]) {
                distances[k - 1] = dist;

                // Ordena o array distances
                for (int l = k - 1; l > 0 && distances[l] < distances[l - 1];
                     l--) {
                    float temp = distances[l];
                    distances[l] = distances[l - 1];
                    distances[l - 1] = temp;
                }
            }
        }

        // Verifica se os resultados obtidos estão corretos
        for (int j = 0; j < k; j++) {
            // float resultDist = distancia(&Q[i], &P[resultIndices[i*n + j]],
            // D);
            float resultDist =
                distancia(&Q[i * D], &P[(resultIndices[i * k + j]) * D], D);
            // float resultDist = 0.0;
            int found = 0;
            for (int l = 0; l < k; l++) {
                if (resultDist == distances[l]) {
                    found = 1;
                    break;
                }
            }
            if (!found) {
                correto = 0;
                break;
            }
        }

        free(distances);

        if (!correto) {
            break;
        }
    }

    if (correto) {
        printf("CORRETO\n");
    } else {
        printf("ERRADO\n");
    }
}

int main(int argc, char **argv) {
    int size_q = atoi(argv[1]);
    int size_p = atoi(argv[2]);
    int dimension = atoi(argv[3]);
    int k = atoi(argv[4]);

    int my_rank, n_proc;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_proc);
    chronometer_t chrono;

    chrono_reset(&chrono);

    srand(1);
    
    float *P = (float *)malloc(size_p * dimension * sizeof(float));
    if (P == NULL) printf("Malloc erro");

    // se for o processo 0, gera o P e o Q
    float *Q = NULL;
    if (my_rank == ROOT_PROCESS) {
        Q = (float *)malloc(size_q * dimension * sizeof(float));

        geraConjuntoDeDados(Q, size_q, dimension);
        geraConjuntoDeDados(P, size_p, dimension);
    }

    int displ[n_proc];
    int sizes[n_proc];
    int sizes_dimension[n_proc];
    int displ_dimension[n_proc];
    int size_qq = (int)ceil((float)size_q / (float)n_proc);
    int extra = (size_qq * n_proc) - size_q;

    sizes[0] = size_qq;
    displ[0] = 0;

    for (int i = 1; i < n_proc; ++i) {
        sizes[i] = size_qq;
        displ[i] = displ[i - 1] + size_qq;
    }
    sizes[n_proc - 1] -= extra;

    for (int i = 0; i < n_proc; ++i) {
        sizes_dimension[i] = sizes[i] * dimension;
        displ_dimension[i] = displ[i] * dimension;
    }

    float *local_Q = (float *)malloc(size_qq * dimension * sizeof(float));
    if (local_Q == NULL) printf("Malloc erro");

    // Faz o scatter da matriz Q para todos os processos
    MPI_Scatterv(Q, sizes_dimension, displ_dimension, MPI_FLOAT, local_Q,
                 size_qq * dimension, MPI_FLOAT, ROOT_PROCESS, MPI_COMM_WORLD);

    // Faz o broadcast para todos os processo da matriz P (dataset)
    MPI_Bcast(P, size_p * dimension, MPI_FLOAT, ROOT_PROCESS, MPI_COMM_WORLD);

    int *local_R = (int *)malloc(size_qq * k * sizeof(int));
    if (local_R == NULL) printf("Malloc erro");


    // Começa a contar o tempo
    chrono_start(&chrono);

    // Calcula o KNN dos pontos Q locais
    knn(local_Q, sizes[my_rank], P, size_p, dimension, k, local_R);

    // Para de contar o tempo
    chrono_stop(&chrono);


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

    // Cria o vetor R que vai receber os resultados de todos os processos
    int *R = NULL;
    if (my_rank == ROOT_PROCESS) {
        R = (int *)malloc(size_q * k * sizeof(int));
    }

    // Ajusta valores para Gatherv
    for (int i = 0; i < n_proc; ++i) {
        sizes_dimension[i] = sizes[i] * k;
        displ_dimension[i] = displ[i] * k;
    }

    // Realiza o Gather de todos os processos MPI
    MPI_Gatherv(local_R, sizes_dimension[my_rank], MPI_INT, R, sizes_dimension,
                displ_dimension, MPI_INT, ROOT_PROCESS, MPI_COMM_WORLD);

    double total_time_in_seconds[1] = {(double)chrono_gettotal(&chrono) /
                                       ((double)1000 * 1000 * 1000)};

    double *times = NULL;

    if (my_rank == ROOT_PROCESS)
        times = (double *)malloc(n_proc * sizeof(double));

    // Tem que fazer o gather dos tempos
    MPI_Gather(total_time_in_seconds, 1, MPI_DOUBLE, times, 1, MPI_DOUBLE,
               ROOT_PROCESS, MPI_COMM_WORLD);

    #if TEST_OUTPUT == 1
        //  tem que atualizar
        // double total_time_in_seconds =
        //     (double)chrono_gettotal(&chrono) / ((double)1000 * 1000 * 1000);
        // chrono_reportTime(&chrono, "parallelReductionTime");
        // double OPS = (size_q * size_p * dimension) / total_time_in_seconds;
        // printf("%lf\n", total_time_in_seconds);
    #else
        double total_time_in_seconds =
            (double)chrono_gettotal(&chrono) / ((double)1000 * 1000 * 1000);
        printf("%lf\n", total_time_in_seconds);
    #endif

    #if TEST_OUTPUT == 1
        if (my_rank == ROOT_PROCESS) {
            verificaKNN(Q, size_q, P, size_p, dimension, k, R);
            printf("\n");
        }
    #endif

    MPI_Finalize();
}