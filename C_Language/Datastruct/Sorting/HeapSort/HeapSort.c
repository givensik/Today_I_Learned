#include <stdio.h>
#include "UsefulHeap.h"

/*
	d1 > d2 -> return값 < 0
	d1 = d2 -> return값 = 0
	d1 < d2 -> return값 > 0
*/
int compare(int d1, int d2){
	return d2 - d1;
}

void HeapSort(int arr[], int n){
	int i;

	Heap heap;
	HeapInit(&heap, compare);

	for(i=0;i<n;i++){
		HInsert(&heap, arr[i]);
	}

	for(i=0;i<n;i++){
		arr[i] = HDelete(&heap);
	}

}

int main(){
	int arr[4] = { 3, 2, 4, 1};
	int i;

	HeapSort(arr, 4);
	for(i=0;i<4;i++){
		printf("%d ", arr[i]);
	}
	printf("\n");
	return 0;
}