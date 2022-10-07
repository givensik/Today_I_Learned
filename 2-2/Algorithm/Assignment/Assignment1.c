//O(n+klogn)..
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//function buildList
int* buildList(int n, int min, int max){
    int *L = (int*)calloc(n+1,sizeof(int));//크기가 n인 배열 동적 할당
    int i;
    srand(time(NULL));
    int random =0 ;
    L[0] = max+1;
    //난수함수 rand()를 이용해서 난수로 배열만들기
    for(i=1;i<=n;i++){
        random = rand()%(max+1) + min;// 최대 최소를 지정해서 값 입력하기
        L[i] = random;
    }


    return L;
}

void upheap(int *heap, int n){
    while(n>1){
        if(heap[n/2]>heap[n]){//부모보다 자식이 작을 때
            heap[0] = heap[n/2];
            heap[n/2] = heap[n];
            heap[n] = heap[0];
            n/=2;
        }else{
            break;
        }
        
    }
}

void input_heap(int* heap, int n, int* List){
    
    int i=0;

    for(i=1;i<=n;i++){
        if(i==1){//처음 입력할 때
        heap[i]=List[i-1];
        }else{
         // printf("n=%d and upheap 들어감\n",n);
         heap[i]=List[i-1];
         upheap(heap,i-1);
        }
    }
    
}

void listheap(int* L, int i, int n)
{
    if ((n < (i * 2)) && (n < (i * 2 + 1))) {
        L[i] = L[0] + 1;  //빈자리를 메꿔줄 노드가 없으면 그 빈자리는 최댓값 + 1로 채워줌
      return;
   }
    if(L[i*2] < L[i*2+1] || n == i*2)  //pop해서 생긴 빈자리 설정
    {
        L[i] = L[i*2];
        //listheap으로 다시 정리
        listheap(L,i*2,n);                                          
    }
    else
    {
        L[i] = L[i*2 + 1];  
        //listheap으로 다시 정리                                
        listheap(L,i*2+1,n);               
    }
    
}

int popheap(int* L, int n, int k)
{
    for(int i = 0; i < k - 1; i++)                                          
    {
        //상위 node가 pop되었다 가정하고 재귀적으로 힙 정리
        listheap(L, 1, n);
    }
    return L[1];
}

//function findkthSmallest O(n+ klogn)
//heap 생성 -> log(n)
//heap에서 뺄때 k 가 되도록
int findkthSmallest( int* L, int n, int k){
    //heap생성
    int *heap = (int*)malloc(sizeof(int)*n);
    //heap에 List 저장
    input_heap(heap,n,L);

    // for(int i=1;i<=n;i++){
    //     printf(" %d",heap[i]);
    // }
    // printf("\n");

    int knum = popheap(heap,n,k);
    
    return knum;
}   

int main(){
    int *L = buildList(10,1,100); // buildList로 만든 List를 받아오기
    int i;
    //난수 생성한 후 출력
    for(i=1;i<=10;i++){
        printf(" %d",L[i]);
    }
    printf("\n");
    //k=1,2,3으로 해서 k번째로 작은 값 출력.
    int k;
    for(k=1;k<=3;k++){
        printf(" %d",findkthSmallest(L,10,k));
    }
    printf("\n");
    free(L);


    clock_t start, end;
    //배열 생성
    int* L2 = buildList(100000, 1, 1000000);
    //k를 배열로 만들기
    int K[4] = {1,100,99900,99999};

    for(int k = 0; k <= 3; k++)
    {
        //printf("k=%d\n",K[k]);
        start = clock();
        int tmp = findkthSmallest(L2,100000,K[k]);
        end = clock();
        //걸린 시간 출력
        double duration = (double)(end - start);                            
        printf("%d %lf\n",tmp,duration);
    }
    


    return 0;
}