#include <stdio.h>


//binary tree 노드에 들어가는 
typedef int BTDATA;
typedef int Data;

typedef struct _bTreeNode{
  BTDATA data;
  struct _bTreeNode *left;
  struct _bTreeNode *right;
} BTreeNode;

int main(){
	Data a;
	BTreeNode *bt;

	a = bt;
	// 이런식으로 int 변수에 ptr 주소가 들어갈 수 있다?
	// -> 안된다 왜냐면 옛날에 32비트일 땐 됐는데, 이제는 64비트여서 int가 4바이트, ptr이 8바이트여서 안됨, 그럼 8바이트면 되는건가?
	
	printf("포인터 변수 크기 : %lubyte\n", sizeof(bt));
	printf("int 변수 크기 : %lubyte\n", sizeof(a));
	
	// printf("%ld\n", ptr2->data);

	return 0;
}