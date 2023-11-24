#include "threads.h"

void *threadKNN(void *args){
    thread_t *thread = (thread_t *) args;
    float *start_q = thread->start_point;
    
    
    //barreira
    pthread_barrier_wait(thread->start);
    //faz o knn para o grupo Q da thread
    knn(start_q, thread->search_size, thread->P, thread->size_p, 
            thread->dimension, thread->k, thread->local_R);

    //barreira
    pthread_barrier_wait(thread->start);
    pthread_exit(NULL);

}


thread_t *create_thread(int id, float *Q, int search_size, float *P, 
        int size_p, int k, int dimension, pthread_barrier_t *start){
    thread_t *thread = malloc(sizeof(thread_t));
    
    thread->id = id;

    thread->start_point = Q;
    thread->local_R = malloc(sizeof(int) * search_size * k);
    thread->search_size = search_size;
    thread->P = P;
    thread->size_p = size_p;
    thread->k = k;
    thread->dimension = dimension;

    thread->start = start;
    int return_val = pthread_create(&thread->thread, NULL, threadKNN, (void *) thread);
    if(return_val){
        printf("Ocorreu um erro ao criar a thread, abortando.\n");
        exit(1);
    }
    return thread;
}


void junta_resultados(int *R, int k, thread_t **threads, int nt){
	
    int size, contador = 0;
    //itera sobre as threads
    for(int i = 0; i < nt; i++){
		size = threads[i]->search_size;
		//itera sobre os pontos de uma thread
		for(int j = 0; j < size; j++){
			
			for(int m = 0; m < k; m++)
				R[contador*k+m] = threads[i]->local_R[j*k+m];
			
			//para saber qual linha do R vai ser
			contador++;
		}
    }
}

void limpa_threads(thread_t **threads, int nt){
    for(int i = 0; i < nt; i++){
        free(threads[i]->local_R);
        free(threads[i]);
    }

}

