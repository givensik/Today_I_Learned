#include <stdio.h>

int one()    // 반환값이 int형인 one 함수 정의
{
    return 1;    // 1을 반환
}

void print_hello(){ // 반환값이 void이면 반환값 없어도됨
    printf("Hello!\n");  
};

int main()
{
    int num1;

    num1 = one();    // int형을 반환했으므로 int형 변수에 저장

    printf("%d\n", num1);    // 1

    print_hello();

    return 0;
}