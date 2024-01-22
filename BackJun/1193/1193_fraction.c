#include <stdio.h>
int Nsum(int N);
int main(){
    
    int i;
    int N;
    scanf("%d",&N);
    //printf("%d\n",Nsum(2));
    if(N==1){
        i=-1;
    }else{
        for(i=0;i<1000000;i++){
            if( Nsum(i) >= N ){
                // printf("i= %d일때 N보다 큼\n",i);
                break;
        }
    }
    }
    
    // printf("i = %d\n",i);
    int num = Nsum(i);
    // printf("num = %d\n",num);
    int gap = num - N;
    // printf("gap = %d\n",gap);
    if(i==-1){
        printf("1/1\n");
    }else if(i%2==0){ // i 가 짝수 -> i/1로 시작
        printf("%d/%d\n",i-gap, 1+gap);
    }else{
        printf("%d/%d\n",1+gap,i-gap);
    }
   
    




    return 0;
}
int Nsum(int N){

    return N*(N+1)/2;
}