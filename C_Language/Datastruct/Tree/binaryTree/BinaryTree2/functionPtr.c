#include <stdio.h>

/*
	함수 포인터
	void (*fp)(반환값);
*/ 

typedef void FuncPtr(int a);

void print(int a){
	printf("%d\n", a);
}

void Test(int a, FuncPtr ptr){
	ptr(a);
}

int main(){
	// 함수 포인터 ptr_print를 선언 하고 print 함수의 주소 대입
	void (*ptr_print)(int) = print;

	Test(5, ptr_print);
	(*ptr_print)(3);
	ptr_print(4);


	return 0;
}