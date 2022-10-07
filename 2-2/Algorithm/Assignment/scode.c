#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int* buildList(int n, int min, int max)                                      //랜덤 함수를 사용해서 배열 생성
{
    
    int* array = (int*)calloc(n + 1,sizeof(int));
    array[0] = max;
    srand(time(NULL));                                                       //항상 다른 랜덤값을 생성하기 위해 seed값을 현재 시간으로 지정
    int random = 0;
    for(int i = 1; i < n + 1;)
    {
        int tmp = rand() % (max + 1);                                        //최댓값을 넘지 않도록 (random값/(최댓값 + 1))의 나머지를 값으로 사용 
        if(tmp >= min)                                                       //최솟값을 못넘으면 다시 랜덤값 생성
        {
            array[i++] = tmp;   
        }
    }
    return array;
    
}
void downHeap(int* array, int i, int n){                                    //재귀적으로 작은 값이 상위노드로 오도록 함
    
   int bigger, temp;
   if ((n < (i * 2)) && (n < (i * 2 + 1))) {                                
      return;
   }
   bigger = i * 2;
   if (n >= i * 2 + 1) {
      if (array[i * 2 + 1] < array[bigger]) {
         bigger = i * 2 + 1;
      }
   }
   if (array[i] <= array[bigger]) {
      return;
   }
   temp = array[i];
   array[i] = array[bigger];
   array[bigger] = temp;
   downHeap(array,bigger,n);
}

void rBuildHeap(int* array,int i,int n)                                      //재귀적으로 최소힙 구축
{
    if(i > n)
     return;
    rBuildHeap(array,2*i,n);
    rBuildHeap(array,2*i + 1, n);
    downHeap(array,i,n);                                                    //작은 값이 상위 노드로 오도록 구현
    return;

}
void listheap(int* array, int index, int n)
{
    if ((n < (index * 2)) && (n < (index * 2 + 1))) {
        array[index] = array[0] + 1;                                        //빈자리를 메꿔줄 노드가 없으면 그 빈자리는 최댓값 + 1로 채워줌
      return;
   }
    if(array[index*2] < array[index*2+1] || n == index*2)                   //pop되서 생긴 빈자리를 뭘로 채울지 결정
    {
        array[index] = array[index*2];
        listheap(array,index*2,n);                                          //빈자리를 다른 노드로 메꿨으니 또 다른 빈자리가 다시 날 것이기 때문에 다시 listheap() 호출
    }
    else
    {
        array[index] = array[index*2 + 1];                                  
        listheap(array,index*2+1,n);                                        //빈자리를 다른 노드로 메꿨으니 또 다른 빈자리가 다시 날 것이기 때문에 다시 listhea() 호출 
    }
    
}
int popheap(int* array, int n,int k)
{
    for(int i = 0; i < k - 1; i++)                                          
    {
        listheap(array, 1, n);                                              //상위 node가 pop되었다 가정하고 재귀적으로 힙 정리
    }
    return array[1];
}

int findKthSmallest(int* L, int n, int k)
{
                                                       
    int*newarr = (int*)calloc(n + 1,sizeof(int));                           //배열을 조작하면서 k번째 작은 수를 찾을 것이기 때문에 새로운 배열 생성
    for(int j = 0; j < n + 1; j++)                                          //새로운 배열에 원래 배열을 copy
    {
        newarr[j] = L[j];
    }
    rBuildHeap(newarr, 1, n);                                               //배열을 최소힙으로 조작함
    int num = popheap(newarr,n,k);                                          //가장 상위node가 가장 작은수이기 때문에 k번 반복해서 상위 node pop하고 힙 정리해서 k번째 pop되는 수 찾기  
    free(newarr);
    return num;
}
void writeList(int* L)                                                      //배열에 있는 수 10개 출력
{
   for(int i = 0; i < 10; i ++)
    {
        printf("%d ",L[i + 1]);
        
    }
    printf("\n");
}
int main()
{

    int* array = buildList(10, 1, 100);                                     //무작위 배열 생성
    writeList(array);
    int output[3];
    for(int k = 1; k < 4; k++)
    {
        output[k] = findKthSmallest(array,10,k);                            //k번째로 작은 수 찾기
    }
    printf("%d, %d, %d\n",output[1],output[2],output[3]);
    free(array);                                                            
    
    clock_t start, end;
    int* array2 = buildList(100000, 1, 1000000);                            //무작위 배열 생성
    int karray[4] = {1,100,99900,99999};                                    //k 값 세팅
    for(int k = 0; k < 4; k++)
    {
        start = clock();
        int tmp = findKthSmallest(array2,100000,karray[k]);                 //k번째 값 찾기
        end = clock();
        double duration = (double)(end - start);                            //걸린 시간 나타내기
        printf("%d %lf\n",tmp,duration);
    }
    free(array2);
    return 0;
}