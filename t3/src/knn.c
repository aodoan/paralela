#include "knn.h"
void knn(float *Q, int size_q, float *P, int size_p, int dimension, int k, int *R) {
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
            if (size < k) 
                insert(heap, &size, dist, j);
            // a heap chegou no tamanho maximo, precisa rodar o decreaseMax
            else 
                decreaseMax(heap, &size, &k, dist, j);
            
        }
        // Coloca os index dos k vizinhos mais proximos no vetor R
        for (int j = 0; j < k; j++) 
            R[i * k + j] = heap[j].index;
        
    }
}

float distancia(float *A, float *B, int dimension) {
    float dist = 0.0;
    float temp;
    for (int i = 0; i < dimension; i++) {
        temp = A[i] - B[i];
        dist = dist + temp * temp;
    }
    return dist;
}

void verificaKNN(float *Q, int nq, float *P, int n, int D, int k, int *resultIndices) {
    int correto = 1, found;
    float *distances, dist, temp, resultDist;
    for (int i = 0; i < nq; i++) {
        // Array para armazenar as distâncias dos k vizinhos mais próximos
        distances = (float *)malloc(k * sizeof(float));
        for (int j = 0; j < k; j++) 
            distances[j] = FLT_MAX;
        

        // Encontrar os k vizinhos mais próximos para Q[i]
        for (int j = 0; j < n; j++) {
            // float dist = distancia(&Q[i], &P[j], D);
            dist = distancia(&Q[i * D], &P[j * D], D);

            // Verifica se esta distância é menor que a maior distância no array
            // distances
            if (dist < distances[k - 1]) {
                distances[k - 1] = dist;

                // Ordena o array distances
                for (int l = k - 1; l > 0 && distances[l] < distances[l - 1];
                     l--) {
                    temp = distances[l];
                    distances[l] = distances[l - 1];
                    distances[l - 1] = temp;
                }
            }
        }

        // Verifica se os resultados obtidos estão corretos
        for (int j = 0; j < k; j++) {
            // float resultDist = distancia(&Q[i], &P[resultIndices[i*n + j]],
            // D);
            resultDist = distancia(&Q[i * D], &P[(resultIndices[i * k + j]) * D], D);
            found = 0;
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
