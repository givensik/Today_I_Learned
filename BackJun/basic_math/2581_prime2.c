#include <stdio.h>
int is_prime(int num);

int main(){

    int M,N;

    scanf("%d",&M);
    scanf("%d",&N);

    int i;
    int sum=0,min;
    for(i=M;i<=N;i++){
        if(is_prime(i)==1){//소수임
            if(sum==0){
                min = i;
            }
            sum += i;
        }
    }

    if(sum == 0){
        printf("-1");
    }else{
        printf("%d\n",sum);
        printf("%d\n",min);
    }

    return 0;
}
int is_prime(int num){
    int i;
    if(num == 1){
        return 0;
    }else{
        for(i=2;i<num;i++){
            if(num%i == 0){
                return 0;
            }
        }

        return 1;   
    }
}