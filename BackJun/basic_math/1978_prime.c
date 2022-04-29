#include <stdio.h>
#include <math.h>
int is_prime(int num);

int main(){

    int N;
    scanf("%d",&N);
    int i,num;
    int cnt = 0;
    for(i=0;i<N;i++){
        scanf("%d",&num);
        if(is_prime(num)==1){//소수일 때
            cnt++;
        }
    }
    
    printf("%d\n",cnt);

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