#include <stdio.h>
#include <stdlib.h>
#include "ListBaseStack.h"

void stackInit(Stack* pstack) // 스택 초기화
{
	pstack->head = NULL;
}

int SIsEmpty(Stack* pstack) // 스택이 비었는지 확인
{
	// 스택이 비었으면 1 출력
	if(pstack->head == NULL){
		return TRUE;
	}
	// 스택이 비어있지 않으면 0 출력
	return FALSE;
}

void SPush(Stack * pstack, Data data){
	// 데이터 들어갈 새로운 노드 생성
	Node *newNode = (Node*)malloc(sizeof(Node));
	
	// 새로운 노드에 데이터 저장
	newNode->data = data;
	newNode->next = pstack->head;
	
	// 스택에 저장
	pstack->head = newNode;
	
}

Data SPop(Stack * pstack){
	Data pData;
	pData = pstack->head->data;
	
	if(SIsEmpty(pstack)){ // 비어있다면
		printf("Stack is Empty\n");
		exit(-1);
	}


	// head의 다음 노드를 head로 지정
	Node* tmp = pstack->head;
	pstack->head = tmp->next;
	
	// 원래의 head 삭제
	free(tmp);

	return pData; // 삭제한 원소 반환
}

Data SPeek(Stack * pstack){
	if(SIsEmpty(pstack)){
		printf("Stack is Empty\n");
		exit(-1);
	}
	return pstack->head->data;
}