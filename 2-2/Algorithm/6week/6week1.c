#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int findPivot(int*L, int a, int b){//a<b
    srand(time(NULL));
    int random = rand()%b + a;
    printf("%d\n",rand);
    return rand()%b + a;
}

int inPlacePartition(int* L, int l, int r, int k){
    int p = L[k];
    int left;
    int right;
    int tmp;

    while (1)
    {
        //left, right 움직이기
        while(L[left] <= p and left != right){
            
            left++
        }
        while(L[right] >= p and left != right){
            right++;
        }
        if(left == right){
            break;
        }
        tmp = L[left];
        L[left] = L[right];
        L[right] = tmp;


    }
    


    return 0;
}

void inPlaceQuickSort(int* L, int l, int r){
    if(l>=r){
        return L;
    }
    
    int k = findPivot(L,l,r);
    int a = inPlacePartition(L,l,r,k);
    

}

int main(){
    int N;
    scanf("%d",&N);
    int *arr = (int*)malloc(sizeof(int)*N);

    int i;
    for(i=0;i<N;i++){
        scanf("%d",&arr[i]);
    }

    


    return 0;
}