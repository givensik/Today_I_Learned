#include <stdio.h>

int heap[101];
int N=0;

void downHeap(int i) {
   int bigger, temp;
   if ((N < (i * 2)) && (N < (i * 2 + 1))) {//자식이 없으면 함수 종료
      return;
   }
   bigger = i * 2;
   if (N >= i * 2 + 1) {
      if (heap[i * 2 + 1] > heap[bigger]) {
         bigger = i * 2 + 1;
      }
   }
   if (heap[i] >= heap[bigger]) {
      return;
   }
   temp = heap[i];
   heap[i] = heap[bigger];
   heap[bigger] = temp;
   downHeap(bigger);
}

void mkheap(int n){
    int i = n;
    while(1){
        downHeap(i);
        i--;
        if(i==0){
            break;
        }
    }
}

void printHeap() {
   for (int i = 1; i < N + 1; i++) {
      printf(" %d", heap[i]);
   }
   printf("\n");
}
int removeHeap(){
    int r;
    r = heap[N];
    N--;
    downHeap(1);
    return r;
}

void inPlaceHeapSort(){
    int tmp;

    tmp = heap[N];
    heap[N] = heap[1];
    heap[1]=tmp;
    removeHeap();

}


void printArray(int n){
    int i;
    for(i=1;i<n+1;i++){
        printf(" %d",heap[i]);
    }
    printf("\n");
}

int main(){

    
    scanf("%d",&N);
    
    int i;
    for(i=1;i<=N;i++){
        scanf("%d",&heap[i]);
    }
    int MaxN = N;
    mkheap(N/2);
    //printHeap();
    for ( i = 0; i <MaxN; i++)
    {
        inPlaceHeapSort();
    }
    

    printArray(MaxN);

    return 0;
}