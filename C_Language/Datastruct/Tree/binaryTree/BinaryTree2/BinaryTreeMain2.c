#include <stdio.h>
#include <stdlib.h>
#include "BinaryTree2.h"

void showData(BTDATA data){
  printf("%d ",data);
}

int main(){
  BTreeNode* bt1 = MakeBTreeNode();
  BTreeNode* bt2 = MakeBTreeNode();
  BTreeNode* bt3 = MakeBTreeNode();
  BTreeNode* bt4 = MakeBTreeNode();
  BTreeNode* bt5 = MakeBTreeNode();
  BTreeNode* bt6 = MakeBTreeNode();
  
  SetData(bt1, 1);
  SetData(bt2, 2);
  SetData(bt3, 3);
  SetData(bt4, 4);
  SetData(bt5, 5);
  SetData(bt6, 6);

  makeLeftSubTree(bt1, bt2);
  makeRightSubTree(bt1, bt3);
  makeLeftSubTree(bt2, bt4);
  makeRightSubTree(bt2, bt5);
  makeRightSubTree(bt3, bt6);

  /*
    현재 상태
        1
      2   3
    4  5    6
  */
  PreorderTraverse(bt1, showData);
  printf("\n");
  InorderTraverse(bt1, showData);
  printf("\n");
  PreorderTraverse(bt1, showData);
  printf("\n");

  // printf("%d\n",GetLeftSubTree(bt2)->data);

  DeleteTree(bt1);
  return 0;
}