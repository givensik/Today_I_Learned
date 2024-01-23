#include <stdio.h>
#include <stdlib.h>
#include "BinaryTree2.h"

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


void PreorderTraverse(BTreeNode* bt, VisitFuncPtr action) // 전위 순회
{
  if(bt == NULL){
    return ;
  }
  action(bt->data); // 부모 노드 출력(이걸 접근한다고 본거)
  PreorderTraverse(bt->left, action); // 왼쪽 서브트리도 전위 순회
  PreorderTraverse(bt->right, action); // 오른쪽 서브트리에도 전위 순회
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
void InorderTraverse(BTreeNode* bt, VisitFuncPtr action){
  if(bt == NULL){
    return ;
  }
  InorderTraverse(bt->left, action); // 왼쪽 서브트리도 중위 순회
  action(bt->data); // 부모 노드 출력(이걸 접근한다고 본거)
  InorderTraverse(bt->right, action); // 오른쪽 서브트리에도 중위 순회
}

// 트리 삭제
void DeleteTree(BTreeNode* bt){
  // 왼쪽 자식이 있으면
  if(bt->left){
    DeleteTree(bt->left);
  }
  if(bt->right){
    DeleteTree(bt->right);
  }
  // 자식 노드들 모두 삭제된 상태이므로 노드 삭제
  printf("delete bt%d\n",bt->data);
  free(bt);
  /*
    책 내용
    if(bt == NULL){
      return;
    }
    DeleteTree(bt->left);
    DeleteTree(bt->right);
    printf("delete bt%d\n",bt->data);
    free(bt);
  */

}