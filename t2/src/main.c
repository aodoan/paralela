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

#include "chrono.h"
#include "maxheap.h"

// define qual processo envia para os outros
#define ROOT_PROCESS 0
#define TEST_OUTPUT 1

#define IMPRIMIR_MATRIZES 1
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

float squaredDistance(float *A, float *B, int dimension){
    float dist = 0.0;
    float temp;
    for(int i = 0; i < dimension; i++){
        temp = A[i] - B[i];
        dist = dist + temp*temp;
    }
    return dist;
}

void geraConjuntoDeDados(float *C, int size, int dimension) {
    int a, b;
    float v;

    for (size_t i = 0; i < size * dimension; i++) {
        a = rand();
        b = rand();
        /*
        a = i * 2 + 7;
        b = a/2;
        */
        v = (a * 100.0 + b)/10000000000;
        C[i] = v;
    }
}

void verificaKNN(float *Q, int size_q, float *P, int size_pacharKNN,
                 int dimension, int k, int *R) {
    // note que R tem nq linhas por k colunas, para qualquer tamanho de k
    // (colunas) entao é linearizado para acesso como um VETOR
    printf(" ------------ VERIFICA KNN --------------- \n");

    for (int i = 0; i < size_q; i++) {
        printf("knn[%d]: ", i);
        for (int j = 0; j < k; j++) 
            printf("%i ", R[i * k + j]);
        printf("\n");
    }
}

// itera sobre o conjunto P para todo ponto Q
void knn(float *Q, int size_q, float *P, int size_p, int dimension, int k, int *R) {
    double dist;
    int size;     // indica o numero de elementos na heap
    pair_t *heap;  // ponteiro usado para representar heap
    heap = createMaxHeap(k);
    int oii;
    for (int i = 0; i < size_q; i++) {
        if (P[i * dimension] != FLT_MAX) {
            size = 0;
            for (int j = 0; j < size_p; j++) {
                // passa a linha I da matriz P
                // passa a linha J da matriz Q
                //dist = calculate_distance(P, Q, dimension, j * dimension, i * dimension);
                dist = squaredDistance(&P[j*dimension], &Q[i*dimension], dimension);
                // a heap ainda nao chegou no tamanho maximo insere o elemento e
                // roda o max-heapify
                if (size < k){ 
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
        } else {
            printf("posso ser adm?\n");
            for (int j = 0; j < k; j++) {
                R[i * k + j] = -1;
            }
        }
    }
}
//retorna 1 se deu certo
//0 caso contrario
void verificaSolucao(float *Q, int nq, float *P, int n, int D, int k, int *resultIndices)
{
    int correto = 1;

    for (int i = 0; i < nq; i++)
    {
        // Array para armazenar as distâncias dos k vizinhos mais próximos
        float *distances = (float *)malloc(k * sizeof(float));
        for (int j = 0; j < k; j++)
        {
            distances[j] = FLT_MAX;
        }

        // Encontrar os k vizinhos mais próximos para Q[i]
        for (int j = 0; j < n; j++)
        {
            //float dist = squaredDistance(&Q[i], &P[j], D);
            float dist = squaredDistance(&Q[i*D], &P[j*D], D);

            // Verifica se esta distância é menor que a maior distância no array distances
            if (dist < distances[k - 1])
            {
                distances[k - 1] = dist;

                // Ordena o array distances
                for (int l = k - 1; l > 0 && distances[l] < distances[l - 1]; l--)
                {
                    float temp = distances[l];
                    distances[l] = distances[l - 1];
                    distances[l - 1] = temp;
                }
            }
        }

        // Verifica se os resultados obtidos estão corretos
        for (int j = 0; j < k; j++)
        {
            //float resultDist = squaredDistance(&Q[i], &P[resultIndices[i*n + j]], D);
            float resultDist = squaredDistance(&Q[i*D], &P[(resultIndices[i*k + j])*D], D);
            //float resultDist = 0.0;
            int found = 0;
            for (int l = 0; l < k; l++)
            {
                if (resultDist == distances[l])
                {
                    found = 1;
                    break;
                }
            }
            if (!found)
            {
                correto = 0;
                break;
            }
        }

        free(distances);

        if (!correto)
        {
            break;
        }
    }

    if (correto)
    {
        printf("CORRETO\n");
    } else
    {
        printf("ERRADO\n");
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


    float *P = (float *)malloc(size_p * dimension * sizeof(float));
    if (P == NULL) printf("Malloc erro");

    // matriz local
    float *local_Q = (float *)malloc(size_q * dimension * sizeof(float));
    if (local_Q == NULL) printf("Malloc erro");

    // matriz PARCIAL da resposta (cada no tem uma)
    int *local_R = (int *)malloc(size_q / n_proc * k * sizeof(int));
    if (local_R == NULL) printf("Malloc erro");

    // se for o processo 0, gera o P e o Q
    float *Q;
    if (my_rank == ROOT_PROCESS) {
        Q = (float *)malloc(size_q * dimension * sizeof(float));

        geraConjuntoDeDados(Q, size_q, dimension);
        geraConjuntoDeDados(P, size_p, dimension);

        for (int i = size_q * dimension; i < size_q * dimension; ++i) {
            Q[i] = FLT_MAX;
        }
    }

    // Começa a contar o tempo
    chrono_start(&chrono);

    // Faz o scatter da matriz Q para todos os processos
    MPI_Scatter(Q, size_q/n_proc * dimension, MPI_FLOAT, local_Q,
                size_q/n_proc * dimension, MPI_FLOAT, ROOT_PROCESS,
                MPI_COMM_WORLD);

    // Faz o broadcast para todos os processo da matriz P (dataset)
    MPI_Bcast(P, size_p * dimension, MPI_FLOAT, ROOT_PROCESS, MPI_COMM_WORLD);

    // Calcula o KNN dos pontos Q locais
    knn(local_Q, size_q/n_proc, P, size_p, dimension, k, local_R);
    
    for(int i = 0; i < size_q; i++){
        printf("resultados[%i] ", my_rank);
        for(int j = 0; j < k; j++)
            printf("[%i] ", local_R[i*k+j]);
        printf("\n");
    }
    printf("\n");
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

    // junta todo no processo raiz
    int *R;
    if (my_rank == ROOT_PROCESS) {
        R = (int *)malloc(size_q * k * sizeof(int));
    }
    
    // Realiza o Gather de todos os processos MPI
    MPI_Gather(local_R, size_q / n_proc * k, MPI_INT, R, size_q / n_proc * k,
               MPI_INT, ROOT_PROCESS, MPI_COMM_WORLD);

    // Para de contar o tempo
    chrono_stop(&chrono);

    // Tem que fazer o gather dos tempos
    // MPI_Gather(&chrono,

#if TEST_OUTPUT == 1
    double total_time_in_seconds =
        (double)chrono_gettotal(&chrono) / ((double)1000 * 1000 * 1000);
    chrono_reportTime(&chrono, "parallelReductionTime");
    double OPS = (size_q * size_p * dimension) / total_time_in_seconds;
    printf("%lf\n", total_time_in_seconds);
#else
    double total_time_in_seconds =
        (double)chrono_gettotal(&chrono) / ((double)1000 * 1000 * 1000);
    printf("%lf\n", total_time_in_seconds);
#endif

#if TEST_OUTPUT == 1
    if (my_rank == ROOT_PROCESS) {
        verificaKNN(Q, size_q, P, size_p, dimension, k, R);
        
        
        
        verificaSolucao(Q, size_q, P, size_p, dimension, k, R);
        printf("\n");
    }
#endif

    MPI_Finalize();
}