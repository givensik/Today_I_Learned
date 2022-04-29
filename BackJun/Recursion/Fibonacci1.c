#include <stdio.h>

int fibonacci(int N);

int main(){
    int N;
    scanf("%d",&N);
    N++;
    printf("%d\n",fibonacci(N));

    return 0;
}
int fibonacci(int N){

    if(N==1){
        return 0;
    }else if(N==2){
        return 1;
    }else{
        return fibonacci(N-1) + fibonacci(N-2);
    }
    
}