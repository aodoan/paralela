#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>

float geraPoint(size_t dimension) {
    int a, b, v;

    srand(1);
}

void geraConjuntoDeDados(float *C, size_t size, size_t dimension) {
    for (int i = 0; i < size; i++)
        for (int j = 0; j < dimension; j++)
            C[i * dimension + j] = rand() * 100.0 + rand();
}

void verificaKNN(float *Q, int size_q, float *P, int size_pacharKNN, int dimension,
                 int k, float *R) {
    // note que R tem nq linhas por k colunas, para qualquer tamanho de k
    // (colunas) entao é linearizado para acesso como um VETOR
    printf(" ------------ VERIFICA KNN --------------- ");
    for (int i = 0; i < size_q; i++) {
        printf("knn[%d]: ", i);
        for (int j = 0; j < k; j++) printf("%f ", R[i * k + j]);
        printf("\n");
    }
}

void knn(float *Q, size_t size_q, float *P, size_t size_p, size_t dimension,
         size_t k) {}

int main(int argc, char **argv) {
    int size_q = atoi(argv[1]);
    int size_p = atoi(argv[2]);
    int dimension = atoi(argv[3]);
    int k = atoi(argv[4]);

    printf("|Q|:%i |P|:%i D:%i k:%i\n", size_q, size_p, dimension, k);

    float *P = (float *)malloc(size_p * dimension * sizeof(float));
    if (P == NULL) printf("Malloc erro");

    float *Q = (float *)malloc(size_q * dimension * sizeof(float));
    if (P == NULL) printf("Malloc erro");

    float *R = (float *)malloc(size_q * k * sizeof(float));
    if (P == NULL) printf("Malloc erro");

    geraConjuntoDeDados(P, size_p, dimension);
    geraConjuntoDeDados(Q, size_q, dimension);

    knn(Q, size_q, P, size_p, dimension, k);

    verificaKNN(Q, size_q, P, size_p, dimension, k, R);

    free(P);
    free(Q);
    free(R);

    return 0;
}