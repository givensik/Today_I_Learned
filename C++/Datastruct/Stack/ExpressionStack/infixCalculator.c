#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "InfixToPostfix.h"
#include "PostCalculator.h"

int EvalInfixExp(char exp[]){

	int len = (int)strlen(exp);
	int ret;
	char* expcpy = (char*)malloc(len+1);
	strcpy(expcpy, exp);

	convToRPNEXP(expcpy);

	ret = EvalRPNEXP(expcpy);

	free(expcpy);
	return ret;
}