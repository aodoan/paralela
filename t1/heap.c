#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int nTotalElements, k, nThreads;
float *Input;

int main(int argc, char **argv){
    //get parameters from command line
    if(argc != 4){
        printf("Erro com os paramêtros.\n");
        exit(1);
    }
    nTotalElements = atoi(argv[1]);
    k = atoi(argv[2]);
    nThreads = atoi(argv[3]);

    printf("nTotal %i \nk %i \nnThreads %i\n", nTotalElements, k, nThreads);

    


    return 0;
}