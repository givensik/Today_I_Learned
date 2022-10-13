#include <stdio.h>
#include <stdlib.h>

int findK(int *arr, int s, int e, int k){
    printf("s= %d e = %d\n",s,e);
    if(arr[(s+e)/2] == k){//키 값이랑 같을 때
        return (s+e)/2;
    }
    
    if(e-s == 1 && arr[e] <=k ){
        return e;
    }else if(e-s == 1){
        return s;
    }

    if(arr[(s+e)/2] < k){//키 값보다 작다 
        return findK(arr,(s+e)/2,e,k);
    }else{
        return findK(arr,s,(s+e)/2,k);
    }

    
}

int main(){
    int n, k;
    scanf("%d %d",&n,&k);

    int *arr = (int*)malloc(sizeof(int)*n);
    for(int i=0;i<n; i++){
        scanf("%d",&arr[i]);
    }
    printf(" %d\n",findK(arr,0,n-1,k));


    return 0;
}