#include <stdio.h>
int Creator(int N);
int main(){

    int N;
    scanf("%d",&N);

    int i,M=0;
    int ans =0;
    for(i=N-1;i>0;i--){
        //printf("i = %d\n",i);
        M = Creator(i);
        if(M==N){
            ans=i;
        }

    }
    printf("%d\n",ans);

    return 0;
}

int Creator(int M){
    int x = M;
    int sum =M;

    while(x>0){
        sum += x%10;
        x/=10;
    } 
    
    return sum;
}