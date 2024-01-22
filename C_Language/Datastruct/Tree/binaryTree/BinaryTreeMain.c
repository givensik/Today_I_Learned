#include <stdio.h>
#include <stdlib.h>
#include "BinaryTree.h"

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

  printf("bt1의 LeftSubTree의 data : %d\n", GetData(GetLeftSubTree(bt1)));
  printf("bt2의 LeftSubTree의 data : %d\n", GetData(GetLeftSubTree(bt2)));



  return 0;
}