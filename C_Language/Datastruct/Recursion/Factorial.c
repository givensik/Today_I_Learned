#include <stdio.h>

int factorial(int N);

int main(){

    int N;
    printf("input : ");
    scanf("%d",&N);

    printf("factorial %d! = %d\n",N,factorial(N));

    return 0;
}
int factorial(int N){

    if(N==1){
        return 1;
    }else{
        return factorial(N-1)*N;
    }

}