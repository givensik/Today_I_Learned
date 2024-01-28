#include <stdlib.h>
#include "BinarySearchTree.h"


//BST의 생성 및 초기화
void BSTMakeAndInit(BTreeNode ** pRoot){
	*pRoot = NULL;
}

// 노드에 저장된 데이터 반환
BSTData BSTGetNodeData(BTreeNode * bst){
	return bst->data;
}

// BST를 대상으로 데이터 저장(노드의 생성과정 포함)
void BSTInsert(BTreeNode ** pRoot, BSTData data){
	
	//노드의 생성과정
	BTreeNode * tmp = (BTreeNode*)malloc(sizeof(BTreeNode));
	SetData(tmp, data);
	
	
	// 루트 노드가 없으면 루트 노드에 저장
	if(*pRoot == NULL){
		*pRoot = tmp;
	}

}

// BST를 대상으로 데이터 탐색
BTreeNode * BSTSearch(BTreeNode * bst, BSTData target);