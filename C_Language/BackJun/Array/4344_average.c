#include <stdio.h>

int main(){

    int N;
    scanf("%d",&N);
    
    int i=0,j=0;
    int C, x[1000];
    double sum=0;
    int cnt;
    for(i=0;i<N;i++){
        scanf("%d",&C);
        sum=0;
        cnt=0;
        for(j=0;j<C;j++){//입력받고 합하기
            scanf("%d",&x[j]);
            sum += x[j];
        }
        sum /= C;//평균 구하기
        // printf("sum = %f\n",sum);
        for(j=0;j<C;j++){
            if(sum < x[j]){
                cnt++;
            }
        }
        
        printf("%.3f%%\n",cnt*1.0/C*100.0);


    }

    return 0;
}