#include <stdio.h>
#include "ListBaseStack.h"

int main(){
	Stack stack;
	stackInit(&stack);

	SPush(&stack, 1);
	SPush(&stack, 2);
	SPush(&stack, 3);
	SPush(&stack, 4);
	SPush(&stack, 5);
	SPush(&stack, 6);
	SPush(&stack, 7);

	while(!SIsEmpty(&stack)){
		printf("%d ", SPop(&stack));
	}
	printf("\n");

	return 0;
}