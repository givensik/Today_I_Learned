#include "BinarySearchTree3.h"
#include "BinaryTree3.h"
#include <stdio.h>

int GetHeight(BTreeNode * bst){
	int left;
	int right;
	
	if(bst == NULL){
		return 0;
	}

	left = GetHeight(bst->left);
	right = GetHeight(bst->right);

	// 이 노드도 따지면 높이에 영향을 주므로 +1 해서 위로 올림
	if(left > right){
		return left + 1;
	}else{
		return right + 1;
	}
}

int GetHeightDiff(BTreeNode * bst){
	if(bst == NULL){
		return 0;
	}

	return GetHeight(bst->left) - GetHeight(bst->right);
}


BTreeNode * RotateLL(BTreeNode * bst){
	BTreeNode * pNode = bst;
	BTreeNode * cNode = GetLeftSubTree(pNode);

	// cNode의 오른쪽 서브트리를 pNode의 왼쪽 서브트리로 이동
	ChangeLeftSubTree(pNode, GetRightSubTree(cNode));
	ChangeRightSubTree(cNode, pNode);

	return cNode;


}

BTreeNode * RotateRR(BTreeNode * bst){
	BTreeNode * pNode = bst;
	BTreeNode * cNode = GetRightSubTree(pNode);

	// cNode의 오른쪽 서브트리를 pNode의 왼쪽 서브트리로 이동
	ChangeRightSubTree(pNode, GetLeftSubTree(cNode));
	ChangeLeftSubTree(cNode, pNode);

	return cNode;
}

BTreeNode * RotateLR(BTreeNode * bst){
	BTreeNode * pNode = bst;
	BTreeNode * cNode = GetLeftSubTree(pNode);

	cNode = RotateRR(cNode);
	ChangeLeftSubTree(pNode, cNode);
	
	return RotateLL(pNode);
}


BTreeNode * RotateRL(BTreeNode * bst){
	BTreeNode * pNode = bst;
	BTreeNode * cNode = GetRightSubTree(pNode);

	cNode = RotateLL(cNode);
	ChangeRightSubTree(pNode, cNode);
	
	return RotateRR(pNode);
}

BTreeNode * Rebalance(BTreeNode ** pRoot){
	int hDiff = GetHeightDiff(*pRoot);
	
	// 균형인수가 2 이상일 떄 -> 왼쪽의 균형이 무너졌엉
	if( hDiff > 1 ){
		// printf("rebal\n");
		// 왼쪽의 높이가 더 크다 -> LL
		if(GetHeightDiff(GetLeftSubTree(*pRoot)) > 0 ){
			// printf("LL\n");
			*pRoot= RotateLL(*pRoot);
		}else{
			// printf("LR\n");
			*pRoot= RotateLR(*pRoot);
		}
	}else if(hDiff < -1){
		// 오른쪽의 높이가 더 크다
		
		if(GetHeightDiff(GetRightSubTree(*pRoot)) < 0 ){
			// printf("RR\n");
			*pRoot= RotateRR(*pRoot);
		}else{
			// printf("RL\n");
			*pRoot= RotateRL(*pRoot);
		}
	}
	return *pRoot;
}