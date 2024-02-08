#include <stdlib.h>
#include "BinarySearchTree.h"
#include <stdio.h>

//BST의 생성 및 초기화
void BSTMakeAndInit(BTreeNode ** pRoot){
	// printf("BST make and init\n");
	*pRoot = NULL;
}

// 노드에 저장된 데이터 반환
BSTData BSTGetNodeData(BTreeNode * bst){
	return bst->data;
}

// BST를 대상으로 데이터 저장(노드의 생성과정 포함)
void BSTInsert(BTreeNode ** pRoot, BSTData data){
	// printf("BST Insert %d\n", data);
	BTreeNode * pNode = NULL;
	BTreeNode * cNode = *pRoot;
	BTreeNode * tmp = NULL;
	// 노드가 들어갈 자리 찾기 -> cNode가 NULL이 아닐 때 까지
	// 비교대상이 없을 때까지 내려감
	while(cNode != NULL){
		// printf("cNode = %d\n", cNode->data);
		// 데이터가 같다? 중복 허용 x
		if(GetData(cNode) == data){
			// printf("중복 x\n");
			return;
		}
		// cNode는 pNode(부모 노드)가 됨
		pNode = cNode;

		// 서브 트리를 구했을 때 NULL이 나오면, 부모 노드인 pNode에 원소 추가를 해야함
		if(GetData(cNode) < data){
			// printf("오른쪽으로\n");
			cNode = GetRightSubTree(cNode);
		}else if(GetData(cNode) > data){
			// printf("왼쪽으로\n");
			cNode = GetLeftSubTree(cNode);
		}
	}

	// 들어갈 노드 생성
	tmp = MakeBTreeNode();
	SetData(tmp, data);

	
	// 트리에 추가 
	if(pNode != NULL){
		if(GetData(pNode) < data){
			makeRightSubTree(pNode, tmp);
		}else{
			makeLeftSubTree(pNode, tmp);
		}
	}else{// root노드가 없다면 넣기
		*pRoot = tmp;
	}

}

// BST를 대상으로 데이터 탐색
BTreeNode * BSTSearch(BTreeNode * bst, BSTData target){

	BTreeNode * tmp = bst;

	while(tmp != NULL){
		// target이 더 크다? -> 오른쪽으로
		if(GetData(tmp) == target){
			return tmp;
		}else if(GetData(tmp) < target){
			tmp = GetRightSubTree(tmp);
		}else{
			tmp = GetLeftSubTree(tmp);
		}
	}
	
	return NULL;
}

/*
	삭제
	1. 삭제할 노드가 단말 노드인 경우
	2. 자식 노드가 1개일 경우
	3. 자식 노드가 2개일 경우

*/

