#ifndef UTILS_H
#define UTILS_H

void drawHeapTree(int heap[], int size, int nLevels);
void maxHeapify(int heap[], int size, int i);
void heapifyUp(int heap[], int *size, int pos) ;
void insert(int heap[], int *size, int element);
int isMaxHeap(int heap[], int size);
void decreaseMax(int heap[], int size, int new_value);

#endif 