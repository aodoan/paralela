#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define n 10
int nTotalElements, k, nThreads;
float *Input;

#define OK 2012
#define NOK 2000 

int cmpfunc (const void * a, const void * b)
{
  float fa = *(const float*) a;
  float fb = *(const float*) b;
  return (fa > fb) - (fa < fb);
}

int verifyOutput( Input, Output, nTotalElements, k ){ 
    qsort(Input, n, sizeof(float), cmpfunc);

    for(int i = 0; i < nTotalElements; i++){
        if(Input != Output) return NOK;
    }

    return OK;

}


int main(int argc, char **argv){
    //get parameters from command line
    if(argc != 4){
        printf("Erro com os paramêtros.\n");
        exit(1);
    }
    nTotalElements = atoi(argv[1]);
    k = atoi(argv[2]);
    nThreads = atoi(argv[3]);
    int a, b;
    float v;
    Input = malloc(sizeof(float) * n);
    for(int i = 0; i < n; i++){
        a = rand();
        b = rand();
        v = a * 1.0 + b;
        Input[i] = v;
    }    
    

    return 0;
}