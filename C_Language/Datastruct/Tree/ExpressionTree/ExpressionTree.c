#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "BinaryTree2.h"
#include "ListBaseStack.h"

// 후위 표기법 -> Tree 저장
BTreeNode* MakeExpTree(char exp[]){
	Stack stack;
	stackInit(&stack);
	BTreeNode* tmp;
	int i,length;
	char pv;
	char op1, op2;
	length = (int)strlen(exp);
	
	for(i = 0; i<length ; i++){
		tmp = MakeBTreeNode();
		pv = exp[i];
		// 정수일 때
		if(isdigit(pv)){
			SetData(tmp, pv - '0');
		}else{
			makeRightSubTree(tmp, SPop(&stack));
			makeLeftSubTree(tmp, SPop(&stack));			
		}
		// tmp는 BTNode 구조체의 주소인데, 이 값이 stack의 데이터로 들어갈 수 있다 -> 개미침
		SPush(&stack, tmp);
	}

	return Spop(&stack); // 왜냐면 연산자 나오면 그게 트리가 되는데 그 루트 노드의 주소를 stack에 저장하기 때문에
}
int EvaluateExpTree(BTreeNode* bt);

void ShowPrefixTypeExp(BTreeNode* bt);
void ShowInfixTypeExp(BTreeNode* bt);
void ShowPostfixTypeExp(BTreeNode* bt);