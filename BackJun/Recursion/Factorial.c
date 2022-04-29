#include <stdio.h>

int factorial(int N);

int main(){

    int N;
    scanf("%d",&N);

    printf("%d\n",factorial(N));

    return 0;
}
int factorial(int N){

    if(N==1 || N == 0){
        return 1;
    }else{
        return factorial(N-1)*N;
    }

}