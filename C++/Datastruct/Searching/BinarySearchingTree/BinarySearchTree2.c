#include <stdlib.h>
#include "BinarySearchTree2.h"
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

// 노드를 제거하고 제거된 노드의 주소 값을 반환
BTreeNode * BSTRemove(BTreeNode **pRoot, BSTData target){
	
	BTreeNode *pVRoot = MakeBTreeNode();
	BTreeNode *pNode = pVRoot; // parent node 이게 있는 이유는 루트 노드를 삭제하는 경우도 다른 노드를 삭제하는 것처럼 만들기 위해서이다.
	BTreeNode *cNode = *pRoot; // current node
	BTreeNode *dNode;

	ChangeRightSubTree(pVRoot, *pRoot); // 가상 루트 노드의 오른쪽 노드로 원래 루트 노드 넣기

	// 삭제할 노드 선택
	while(cNode != NULL && GetData(cNode) != target){
		// 현재 노드를 부모노드로 바꿔서 자식노드로 검색 대상으로 옮김
		pNode = cNode;

		if(target < GetData(cNode)){
			cNode = GetLeftSubTree(cNode);
		}else{
			cNode = GetRightSubTree(cNode);
		}

	}

	if(cNode == NULL){
		return NULL;
	}

	dNode = cNode;

	// 1. 삭제 노드가 단말 노드 인 경우
	if(GetLeftSubTree(dNode) == NULL && GetRightSubTree(dNode) == NULL){
		if(GetLeftSubTree(pNode) == dNode){
			RemoveLeftSubTree(pNode);
		}else{
			RemoveRightSubTree(pNode);
		}
	}

	// 2. 삭제 대상이 하나의 자식 노드를 가지는 경우
	else if(GetLeftSubTree(dNode) == NULL || GetRightSubTree(dNode) == NULL){
		BTreeNode * dcNode;
		//왼쪽 노드인 경우
		if(GetLeftSubTree(dNode) != NULL ){
			dcNode = GetLeftSubTree(dNode);
		}else{
			dcNode = GetRightSubTree(dNode);
		}
		//삭제 노드가 왼쪽 노드인 경우
		if(GetLeftSubTree(pNode) == dNode){
			ChangeLeftSubTree(pNode, dcNode);
		}else{
			ChangeRightSubTree(pNode, dcNode);
		}
	}
	// 3. 두 개의 자식 노드를 모두 갖는 경우
	else{
		BTreeNode * mNode = GetRightSubTree(dNode);
		BTreeNode * mpNode = dNode;

		// 대체노드 찾기
		while(GetLeftSubTree(mNode) != NULL){
			mpNode = mNode;
			mNode = GetLeftSubTree(mNode);
		}
		int delData = GetData(dNode);
		SetData(dNode, GetData(mNode));

		if(GetLeftSubTree(mpNode) == mNode){
			ChangeLeftSubTree(mpNode, GetRightSubTree(mNode));	
		}else{
			ChangeRightSubTree(mpNode, GetLeftSubTree(mNode));
		}

		// mdata의 값을 dNode로 옮기고 mNode로 dNode값을 넘기는거임
		dNode = mNode;
		SetData(dNode, delData);
	}

	if(GetRightSubTree(pVRoot)!= *pRoot){
		*pRoot = GetRightSubTree(pVRoot);
	}
	free(pVRoot);
	return dNode;
}

// 이진 탐색 트리에 저장된 모든 노드의 데이터를 출력
void ShowIntData(int data) {
	printf("%d ", data);
}

void BSTShowAll(BTreeNode *bst) {
	InorderTraverse(bst, ShowIntData);
}