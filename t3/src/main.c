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
#include <unistd.h>
#include <pthread.h>

#include "chrono.h"
#include "maxheap.h"
#include "threads.h"
// define qual processo envia para os outros
#define ROOT_PROCESS        0
#define TEST_OUTPUT         1

#define IMPRIMIR_MATRIZES   0

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


float *P = NULL;
float *Q = NULL;
float *local_Q;
int main(int argc, char **argv) {
    int size_q = atoi(argv[1]);
    int size_p = atoi(argv[2]);
    int dimension = atoi(argv[3]);
    int k = atoi(argv[4]);
    int nt = atoi(argv[5]);
    int my_rank, n_proc;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &n_proc);
    chronometer_t chrono;

    chrono_reset(&chrono);

    srand(1);
    
    P = (float *)malloc(size_p * dimension * sizeof(float));
    if (P == NULL) printf("Malloc erro");

    // se for o processo 0, gera o P e o Q
    Q = NULL;
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

    local_Q = (float *)malloc(size_qq * dimension * sizeof(float));
    if (local_Q == NULL) printf("Malloc erro");

    // Faz o scatter da matriz Q para todos os processos
    MPI_Scatterv(Q, sizes_dimension, displ_dimension, MPI_FLOAT, local_Q,
                 size_qq * dimension, MPI_FLOAT, ROOT_PROCESS, MPI_COMM_WORLD);

    // Faz o broadcast para todos os processo da matriz P (dataset)
    MPI_Bcast(P, size_p * dimension, MPI_FLOAT, ROOT_PROCESS, MPI_COMM_WORLD);
    //a partir desse ponto, todos os processos tem todos os pontos
    //cada processo separa em nt threads
    
    pthread_barrier_t start;
    pthread_barrier_init(&start, NULL, nt + 1);

    int size_of_each = sizes[my_rank] / nt;
    thread_t **threads = malloc(sizeof(thread_t *) * nt);
    int where = 0;
    for(int i = 0; i < nt; i++){
        //se for a ultima thread, pega o resto que sobrou
        if(i == nt - 1)
            size_of_each = (sizes[my_rank] / nt) + (sizes[my_rank] % nt); //pega o mesmo tanto que todas as threads pegou + o que sobrou
        threads[i] = create_thread(i, &local_Q[where*dimension], size_of_each, P, 
                size_p, k, dimension, &start);
        where = where + size_of_each;
    }


    pthread_barrier_wait(&start);
    // Começa a contar o tempo
    chrono_start(&chrono);
       // Para de contar o tempo
    pthread_barrier_wait(&start);
    chrono_stop(&chrono);
    //junta o R de cada thread em um R local
    int *local_R = malloc(sizeof(int) * k * sizes[my_rank]);
	
    junta_resultados(local_R, k, threads, nt);

    limpa_threads(threads, nt);
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

    if(my_rank == ROOT_PROCESS){
        double m = times[0];
        for(int i = 0; i < n_proc; i++){
            if(times[i] > m)
                m = times[i];
        }
        printf("time spent: %lf\n", m);
    }
    
    #if TEST_OUTPUT == 1
        if (my_rank == ROOT_PROCESS) {
            double total_time_in_seconds =
            (double)chrono_gettotal(&chrono) / ((double)1000 * 1000 * 1000);
            printf("%lf\n", total_time_in_seconds);
            verificaKNN(Q, size_q, P, size_p, dimension, k, R);
            printf("\n");

        }
    #endif
	
    MPI_Finalize();
}
