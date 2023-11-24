#ifndef __KNNL__
#define __KNNL__
#include "maxheap.h"

void knn(float *Q, int size_q, float *P, int size_p, int dimension, int k, int *R);
float distancia(float *A, float *B, int dimension);
void verificaKNN(float *Q, int nq, float *P, int n, int D, int k, int *resultIndices);
#endif