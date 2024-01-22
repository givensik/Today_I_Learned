#ifndef __BINARY_TREE_H__
#define __BINARY_TREE_H__

typedef int BTDATA;

typedef struct _bTreeNode{
  BTDATA data;
  struct _bTreeNode *left;
  struct _bTreeNode *right;
} BTreeNode;

BTreeNode * MakeBTreeNode(void); // 노드의 생성
BTDATA GetData(BTreeNode* bt); // 데이터 가져오기
void SetData(BTreeNode* bt, BTDATA data); // 데이터 설정하기

BTreeNode* GetLeftSubTree(BTreeNode* bt); // 왼쪽 서브트리 가져오기
BTreeNode* GetRightSubTree(BTreeNode* bt); // 오른쪽 서브트리 가져오기

void makeLeftSubTree(BTreeNode* main, BTreeNode* sub); // 왼쪽 서브트리 만들기
void makeRightSubTree(BTreeNode* main, BTreeNode* sub); // 오른쪽 서브트리 만들기

#endif