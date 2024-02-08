#include <stdio.h>
#include "PostCalculator.h"

int main(){
	char postExp1[] = "42*8+";
	char postExp2[] = "123+*4/";

	printf("%s = %d \n", postExp1, EvalRPNEXP(postExp1));
	printf("%s = %d \n", postExp2, EvalRPNEXP(postExp2));

	return 0;
}