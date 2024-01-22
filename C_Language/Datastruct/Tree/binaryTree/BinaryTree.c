#include <stdio.h>
#include <stdlib.h>
#include "BinaryTree.h"

// 노드의 생성
BTreeNode* MakeBTreeNode(void){ 
  BTreeNode *p =  (BTreeNode*)malloc(sizeof(BTreeNode));
  p->left = NULL;
  p->right = NULL;

  return p;
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
// 왼쪽 서브트리 만들기
void makeRightSubTree(BTreeNode* main, BTreeNode* sub){
  if(main->right != NULL){
    free(main->right);
  }
  main->right = sub;
}