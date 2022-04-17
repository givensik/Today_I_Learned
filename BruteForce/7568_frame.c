#include <stdio.h>
typedef struct spec
{
    int height, weight, rank;
}spec;

int frame(spec x,spec y);

int main(){

    int N;
    scanf("%d",&N);
    spec x[50];

    int i,j;
    for(i=0;i<N;i++){
        scanf("%d %d",&x[i].weight,&x[i].height);
    }

    for(i=0;i<N;i++){
        x[i].rank = 1;
        //printf("i = %d\n",i);
        for(j=0;j<N;j++){
            if(frame(x[i],x[j])==0){//x[i]가 클때
                x[i].rank++;
                //printf("j=%d -> x[i]<x[j]\n",j);                
            }
        }
    }

    for(i=0;i<N;i++){
        printf("%d ",x[i].rank);
    }  
    printf("\n");



    return 0;
}
int frame(spec x,spec y){
    int X=0, Y=0;
    if(x.height < y.height){
        Y++;
    }else{
        X++;
    }
    if(x.weight < y.weight){
        Y++;
    }else{
        X++;
    }
    if(X>=Y){//X와Y가 같으면 X가 큼
        return 1;
    }else{
        return 0;
    }

    printf("function error!\n");
}
