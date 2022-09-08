#include <stdio.h>
#include <stdlib.h>

void sort(int* arr, int N){
    int i,tmp;
    int maxi = 0;
    //find maxi
    for(i=0;i<N;i++){
        if(arr[maxi]<arr[i]){
            maxi = i;
        }
    }
    //printf("maxi=%d, max = %d\n",maxi,arr[maxi]);

    tmp = arr[maxi];
    arr[maxi] = arr[N-1];
    arr[N-1] = tmp;

}


int main(){
    
    int n;

    scanf("%d",&n);
    
    int *arr = (int*)malloc(sizeof(int)*n);

    int i,j;
    for(i=0;i<n;i++){
        scanf("%d",arr+i);
    }

    for(i=n;i>=1;i--){
        sort(arr,i);
    }
    
    for(i=0;i<n;i++){
        printf(" %d",arr[i]);
    }
    printf("\n");

    free(arr);




    return 0;
}