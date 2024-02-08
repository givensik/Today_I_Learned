#include <stdio.h>
#include <stdlib.h>
#include "BinaryTree.h"

// 중위 순회
void InorderTraverse(BTreeNode* bt){
  if(bt == NULL){
    return ;
  }
  InorderTraverse(bt->left); // 왼쪽 서브트리도 중위 순회
  printf("%d",bt->data); // 부모 노드 출력(이걸 접근한다고 본거)
  InorderTraverse(bt->right); // 오른쪽 서브트리에도 중위 순회
}

int main(){
  BTreeNode* bt1 = MakeBTreeNode();
  BTreeNode* bt2 = MakeBTreeNode();
  BTreeNode* bt3 = MakeBTreeNode();
  BTreeNode* bt4 = MakeBTreeNode();
  
  SetData(bt1, 1);
  SetData(bt2, 2);
  SetData(bt3, 3);
  SetData(bt4, 4);

  makeLeftSubTree(bt1, bt2);
  makeRightSubTree(bt1, bt3);
  makeLeftSubTree(bt2, bt4);

  /*
    현재 상태
        1
      2   3
    4
  */
  InorderTraverse(bt1);
  // printf("%d\n",GetLeftSubTree(bt2)->data);


  return 0;
}