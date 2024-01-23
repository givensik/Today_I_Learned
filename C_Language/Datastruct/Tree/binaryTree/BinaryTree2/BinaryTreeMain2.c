#include <stdio.h>
#include <stdlib.h>
#include "BinaryTree2.h"

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
 
  // printf("%d\n",GetLeftSubTree(bt2)->data);


  return 0;
}