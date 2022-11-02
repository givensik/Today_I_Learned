#include <stdio.h>
#include <stdlib.h>

int n;
int k;
int l, r;
//k보다 큰애 중 가장 작은애
// k = 11
// l=11 , r=10, p = 10
//0 1 2 3 4 5 6 7 8 9 10
int findKey(int *arr){
    int p;
    while(1){
        p = (l+r)/2;
        if(l>r){
            if(l == n){
                return n;
            }else if(l == -1){
                return 0;
            }
            return l;
        }
        if(k == arr[p]){//k랑 같을때
            return p;
        }else if(k > arr[p]){//k보다 작을 때
            l = p+1;
        }else if(k < arr[p]){//k보다 클 때
            r = p-1;
        }
    }
}

int main(){
    scanf("%d %d",&n,&k);
    int* arr = (int*)malloc(sizeof(int)*n);
    
    for(int i= 0; i < n; i++){
        scanf("%d",&arr[i]);
    }
    l=0;
    r=n-1;
    printf(" %d\n",findKey(arr));


    return 0;
}