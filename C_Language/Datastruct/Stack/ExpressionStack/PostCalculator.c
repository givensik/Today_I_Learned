#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "ListBaseStack.h"


int EvalRPNEXP(char exp[]){
	// 피연산자 스택
	Stack stack;
	stackInit(&stack);
	int i;
	int length = (int)strlen(exp);	
	char tmp, op1, op2;
	for(i=0;i<length; i++){
		tmp = exp[i];
		// 정수일 때 -> 스택에 넣기
		if(isdigit(tmp)){
			//정수로 변환하고 넣기
			SPush(&stack, tmp - '0');
			continue;	
		}
		
		// 연산자일 때 -> 늦게 들어온 애가 뒤에 잇어야함
 		op2 = SPop(&stack);
		op1 = SPop(&stack);
		

		switch (tmp)
		{
		case '+':
			// printf("%c + %c = %c\n", op1, op2, op1+op2);
			SPush(&stack, op1 + op2);
			break;
		case '-':
			// printf("%c - %c = %c\n", op1, op2, op1-op2);
			SPush(&stack, op1 - op2);
			break;
		case '*':
			// printf("%c * %c = %c\n", op1, op2, op1*op2);
			SPush(&stack, op1 * op2);
			break;
		case '/':
			// printf("%c / %c = %c\n", op1, op2, op1/op2);
			SPush(&stack, op1 / op2);
			break;
		
		default:
			break;
		}

	}
	return SPop(&stack);
}