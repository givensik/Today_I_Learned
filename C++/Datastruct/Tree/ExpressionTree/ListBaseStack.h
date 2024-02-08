#ifndef __LB_STACK_H__
#define __LB_STACK_H__

#define TRUE 1
#define FALSE 0

#include "BinaryTree2.h"

typedef int Data;

typedef struct _node
{
	BTreeNode * bt;
	struct _node* next;
	
}Node;

typedef struct _ListStack
{
	Node* head;
	
} ListStack;

typedef ListStack Stack;

void stackInit(Stack* pstack); // 스택 초기화
int SIsEmpty(Stack* pstack); // 스택이 비었는지 확인

void SPush(Stack * pstack, BTreeNode* bt);
BTreeNode * SPop(Stack * pstack);
Data SPeek(Stack * pstack);

#endif


