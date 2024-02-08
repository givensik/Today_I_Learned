#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "BinaryTree2.h"
#include "ListBaseStack.h"

// 후위 표기법 -> Tree 저장
BTreeNode* MakeExpTree(char exp[]){
	Stack stack;
	stackInit(&stack);
	BTreeNode* tmp;
	BTreeNode *op1, *op2;
	int i,length;
	char pv;
	length = (int)strlen(exp);
	
	for(i = 0; i<length ; i++){
		tmp = MakeBTreeNode();
		pv = exp[i];
		// 정수일 때
		if(isdigit(pv)){
			SetData(tmp, pv - '0');
		}else{// 정수가 아닐 때, 스택에서 피연산자 2개 꺼내서 tmp의 자식 노드로 만들기 
			op2 = SPop(&stack);
			op1 = SPop(&stack);
			makeRightSubTree(tmp, op2);
			makeLeftSubTree(tmp, op1);	
			SetData(tmp, pv);		
		}
		// tmp는 BTNode 구조체의 주소인데, 이 값이 stack의 데이터로 들어갈 수 있다 ? -> 안되는거같음 책이 써진 년도가 13년도인데, 이때는 포인터 바이트가 4바이트라 되는 듯?
		// 그러면 SPop, SPush일때, data를 넣고 뺴는게 아니라 BTreeNode를 넣고 빼게 바꾸면 되겠지?
		SPush(&stack, tmp);
	}

	return SPop(&stack); // 왜냐면 연산자 나오면 그게 트리가 되는데 그 루트 노드의 주소를 stack에 저장하기 때문에
}

int EvaluateExpTree(BTreeNode* bt){
	
	int op1, op2;
	if(GetLeftSubTree(bt) == NULL && GetRightSubTree(bt) == NULL){
		return GetData(bt);
	}

	op1 = EvaluateExpTree(GetLeftSubTree(bt));
	op2 = EvaluateExpTree(GetRightSubTree(bt));

	switch(GetData(bt)) {
		case '+':
			// printf("%d + %d\n",op1, op2);
			return op1 + op2;
		case '-':
			// printf("%d - %d\n",op1, op2);
			return op1 - op2;
		case '*':
			// printf("%d * %d\n",op1, op2);
			return op1 * op2;
		case '/':
			// printf("%d / %d\n",op1, op2);
			return op1 / op2;	
	}

	return 0;
}

void ShowNodeData(int data) {
	if(data >= 0 && data <= 9) 
		printf("%d ", data);
	else
		printf("%c ", data);
}

void ShowPrefixTypeExp(BTreeNode *bt) {
	PreorderTraverse(bt, ShowNodeData);
}

void ShowInfixTypeExp(BTreeNode *bt) {
	InorderTraverse(bt, ShowNodeData);
}

void ShowPostfixTypeExp(BTreeNode *bt) {
	PostorderTraverse(bt, ShowNodeData);
}