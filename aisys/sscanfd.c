#include <stdio.h>
#include <stdlib.h>

int main(void){
	char str[] = "Hello World!";
	char *p = NULL;
	
	int n = sscanf(str, "%ms", &p);
	if(n == 1){
		printf("Read string: %s\n", p);
		free(p);
	}
	
	return 0;
}