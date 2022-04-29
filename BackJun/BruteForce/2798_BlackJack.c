#include <stdio.h>

int main(){

    int N,M;
    int x[300000];
    scanf("%d %d",&N,&M);

    int i,j,k;
    for(i=0;i<N;i++){
        scanf("%d",&x[i]);
    }
    int sum =0;
    int max = 0;
    for (i=0;i<N;i++)
    {
        for(j=i+1;j<N;j++){
            for(k=j+1;k<N;k++){
               sum = x[i] + x[j] + x[k];
               if(M < sum){
                   continue;
               } else{
                   if (sum>max)
                   {
                       max = sum;
                   }
                   

               }
            }
        }
    }

    printf("%d\n",max);    

    return 0;
}