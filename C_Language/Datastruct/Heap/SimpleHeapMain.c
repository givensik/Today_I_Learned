#include <stdio.h>
#include "SimpleHeap.h"

int main(void) {
	Heap heap;
	HeapInit(&heap);
	int i = 1;
	HInsert(&heap, 'A', 1);
	HInsert(&heap, 'B', 2);
	HInsert(&heap, 'C', 3);
	printf("%c\n", HDelete(&heap));

	// for(i = 1; i<heap.numOfData + 1 ; i ++){
	// 	printf("i = %d, %c\n", i, heap.heapArr[i].data);
	// }
	HInsert(&heap, 'A', 1);
	HInsert(&heap, 'B', 2);
	HInsert(&heap, 'C', 3);
	printf("%c\n", HDelete(&heap));
	// for(i = 1; i<heap.numOfData + 1 ; i ++){
	// 	printf("i = %d, %c\n", i, heap.heapArr[i].data);
	// }
	// printf("%d\n", HIsEmpty(&heap));
	while(!HIsEmpty(&heap)){
		printf("%c\n", HDelete(&heap));
	}

	return 0;
}
/*	1 2 3 4 5 6
	A B C C B 
	1 2 3 3 2 
*/