#include <stdio.h>
#include "BinarySearchTree.h"

int main(){
	BTreeNode * bstRoot;
	BTreeNode * sNode;

	BSTMakeAndInit(&bstRoot);
	BSTInsert(&bstRoot, 1);
	BSTInsert(&bstRoot, 2);
	BSTInsert(&bstRoot, 3);

	sNode = BSTSearch(bstRoot, 1);

	if(sNode == NULL){
		printf("실패\n");
	}else{
		printf("키 값 : %d\n", BSTGetNodeData(sNode));
	}
	
	return 0;
}