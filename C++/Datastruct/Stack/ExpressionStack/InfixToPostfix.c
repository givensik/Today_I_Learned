#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "ListBaseStack.h"
#include <stdio.h>

/*
	연산자 우선 순위
	*,/ > +- > (
*/
// 연산자 우선 순위를 출력하는 함수
int GetOpPrec(char op){

	switch (op)
	{
	case '*':
	case '/':
		return 5;
	case '+':
	case '-':
		return 3;
	case '(':
		return 1;
	}

	return -1;
}

/*
	스택에 위치한 연산자의 우선 순위가 높다면?
	->스택에 위치한 연산자를 꺼내서 옮김

	스택에 위치한 연산자의 우선순위가 낮다면?
	-> 스택에 위치한 연산자 위에 연산자를 옮김
*/

int WhoPrecOp(char op1, char op2){
	int op1Prec = GetOpPrec(op1);
	int op2Prec = GetOpPrec(op2);
	
	if(op1Prec > op2Prec){
		return 1;
	}else if(op1Prec < op2Prec){
		return -1;
	}else{
		return 0;
	}
}



// 중위 -> 후위
void convToRPNEXP(char exp[]){
	Stack stack;
	stackInit(&stack);
	int length = (int)strlen(exp);
	
	char *result = (char*)malloc(sizeof(char)*(length+1));

	int i, idx=0;
	char tok, popOp;
	char tmp;
	// result로 전달된 주소의 메모리에서부터 sizeof(char)*(length+1) 바이트의 값을 0으로 채운다.
	// memset(result, 0, sizeof(char)*length+1);

	for(i=0;i<length;i++){
		tok = exp[i];
		// printf("i %d, tok = %c\n", i, tok);
		// 정수라면
		if(isdigit(tok)){
			result[idx++] = tok;
		}else{
			switch (tok)
			{
			case '(':
				// printf("PUSH %c\n", tok);
				SPush(&stack, tok);
				break;
			case ')':
				while(1){
					popOp = SPop(&stack);
					// printf(")여서 POP %c\n", popOp);
					if(popOp == '('){
						break;
					}
					result[idx++] = popOp;
				}
				break;
			case '+':
			case '-':
			case '*':
			case '/':
				// tok이 우선순위가 높다? -> stack에 push
				// tok이 우선순위가 낮다? -> stack에 있는애들 먼저 pop하고, 비어있거나 우선순위가 낮은애가 있을 때 stack에 push
				while(!SIsEmpty(&stack) && WhoPrecOp(SPeek(&stack), tok) >= 0 ){
					// printf("스택 : %c tok : %c\n", SPeek(&stack), tok);
					tmp = SPop(&stack);
					// printf("우선순위 POP %c\n", tmp);
					result[idx++] = tmp;
				}
				// printf("PUSH %c\n", tok);
				SPush(&stack, tok);
				break;
			}
		}
		// printf("result %s\n", result);
	}
	// 스택을 다 비울 때 까지 다 털어내기
	while(!SIsEmpty(&stack)){
		result[idx++] = SPop(&stack);
	}



	// result -> exp
	strcpy(exp, result);
	free(result);

}