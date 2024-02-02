#include <stdio.h>
#include <stdlib.h>
#include "BinaryTree3.h"

// 노드의 생성
BTreeNode * MakeBTreeNode(void){
	BTreeNode* bt = (BTreeNode*)malloc(sizeof(BTreeNode)); 
	bt->left = NULL;
	bt->right = NULL;
	return bt;
}

// 데이터 가져오기
BTDATA GetData(BTreeNode* bt){
	return bt->data;
}
// 데이터 설정하기
void SetData(BTreeNode* bt, BTDATA data){
	bt->data = data;
}
// 왼쪽 서브트리 가져오기
BTreeNode* GetLeftSubTree(BTreeNode* bt){
	return bt->left;
}
// 오른쪽 서브트리 가져오기
BTreeNode* GetRightSubTree(BTreeNode* bt){
	return bt->right;
}
// 왼쪽 서브트리 만들기
void makeLeftSubTree(BTreeNode* main, BTreeNode* sub){
	if(main->left != NULL){
		free(main->left);
	}
	main->left = sub;
}

void makeRightSubTree(BTreeNode* main, BTreeNode* sub) // 오른쪽 서브트리 만들기
{
	if(main->right != NULL){
		free(main->right);
	}
	main->right = sub;
}

typedef void VisitFuncPtr(BTDATA data); // 함수 포인턴

void PreorderTraverse(BTreeNode* bt, VisitFuncPtr action) // 전위 순회
{
	if(bt == NULL){
		return ;
	}
	action(bt->data);
	PreorderTraverse(bt->left, action);
	PreorderTraverse(bt->right, action);

}
void PostorderTraverse(BTreeNode* bt, VisitFuncPtr action) // 후위 순회
{
	if(bt == NULL){
    return ;
 	 }
 	PostorderTraverse(bt->right, action); // 오른쪽 서브트리에도 후위 순회
  	action(bt->data); // 부모 노드 출력(이걸 접근한다고 본거)
  	PostorderTraverse(bt->left, action); // 왼쪽 서브트리도 후위 순회
}
void InorderTraverse(BTreeNode* bt, VisitFuncPtr action) // 중위 순회
{
	if(bt == NULL){
    return ;
  	}
  	InorderTraverse(bt->left, action); // 왼쪽 서브트리도 중위 순회
  	action(bt->data); // 부모 노드 출력(이걸 접근한다고 본거)
  	InorderTraverse(bt->right, action); // 오른쪽 서브트리에도 중위 순회
}

void DeleteTree(BTreeNode* bt){
	if(bt->left != NULL){
		DeleteTree(bt->left);
	}
	if(bt->right != NULL){
		DeleteTree(bt->right);
	}
	printf("노드 %d 삭제 \n", bt->data);
	free(bt);
}

// 서브트리를 삭제할 뿐, 메모리 해제를 하지 않고 그 노드를 반환
BTreeNode * RemoveLeftSubTree(BTreeNode * bt){
	BTreeNode * tmp;
	
	if(bt != NULL){
		tmp = bt->left;
		bt->left = NULL;
	}

	return tmp;

}

BTreeNode * RemoveRightSubTree(BTreeNode * bt){
	BTreeNode * tmp;
	
	if(bt != NULL){
		tmp = bt->right;
		bt->right = NULL;
	}

	return tmp;
}

void ChangeLeftSubTree(BTreeNode * main, BTreeNode * sub){
	main->left = sub;
}
void ChangeRightSubTree(BTreeNode * main, BTreeNode * sub){
	main->right = sub;
}