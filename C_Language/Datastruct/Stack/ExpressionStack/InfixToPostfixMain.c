#include <stdio.h>
#include "InfixToPostfix.h"

int main(){
	char exp1[] = "1+2*3";
	char exp2[] = "(1+2)*3";
	char exp3[] = "((1-2)+3)*(5-2)";
	
	convToRPNEXP(exp1);
	convToRPNEXP(exp2);
	convToRPNEXP(exp3);

	printf("%s \n",exp1);
	printf("%s \n",exp2);
	printf("%s \n",exp3);
	
	return 0;
}