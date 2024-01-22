#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

int n = 100000;
int Limits[4] = {1,100,500,1000};
int Limit = 1;

int* createArray()
{
    srand(time(NULL));                                                              //시간을 시드값으로 설정하여 랜덤값 생성
    int* A = (int*)malloc(sizeof(int)*n);
    for(int i = 0; i < n; i++){
        A[i] = rand() % n + 1;                                                      //원소의 크기는 1 부터 n까지
    }
    return A;
}
int findIndexOfMedianOfThree(int*A, int a, int b, int c)                            //세개의 인덱스중에 value값이 중간값인 인덱스 찾기
{
   if ((A[a] >= A[b] && A[a] <= A[c]) || (A[a] <= A[b] && A[a] >= A[c]))           
   {
        return a; 
   }
   else if((A[b] >= A[a] && A[b] <= A[c]) || (A[b] <= A[a] && A[b] >= A[c]))
   {
        return b;
   }
   else{
        return c;
   }




}

int findPivot(int* A,int l,int r,char* mode)                                        //피벗 설정 함수
{
    if(strcmp(mode,"deterministic1") == 0)                                          //맨 끝의 인덱스 반환
    {
        return r;
    }
    else if(strcmp(mode,"randomized1") == 0)                                        //l부터 r까지 숫자 중에 랜덤한 수 반환
    {
        srand(time(NULL));
        int dif = r - l;
        return l + (rand() % (dif + 1));

    }
    else if(strcmp(mode, "deterministic3") == 0)                                    //처음, 중간, 끝 인덱스 중에 value값이 중간값인 인덱스 반환
    {
        int a = l;
        int b = (l + r)/2;
        int c = r;
        return findIndexOfMedianOfThree(A,a,b,c);
    }
    else if(strcmp(mode,"randomized3") == 0)                                        //랜덤값을 세개를 구한 후 value값이 중간값인 인덱스를 반환
    {
        srand(time(NULL));
        int dif = r - l;
        int a = l + (rand() % (dif + 1));
        int b = l + (rand() % (dif + 1));
        int c = l + (rand() % (dif + 1));
        //printf("%d %d %d\n",a,b,c);
        return findIndexOfMedianOfThree(A,a,b,c);
    }
}
int inPlacePartition(int *L, int l, int r, int k){

   int temp, i = l, j = r - 1, p = L[k];                               //맨 뒤는 피벗자리라서 j는 r-1부터 시작함

   temp = L[k];                                                        //일단 피벗을 맨 뒤로 옮김
    L[k] = L[r];                                        
    L[r] = temp;



   while (i <= j){                                                     //i와 j가 만날때까지 반복함

      while (i <= j && L[i] < p) {                                    //피벗보다 큰수를 만날때까지 반복
            i = i + 1;
        }

      while (j >= i && L[j] >= p) {                                   //피벗보다 작은수를 만날때까지 반복
            j = j - 1;
        }

      if (i < j){                                                     //작은수를 피벗 왼쪽으로, 큰수를 피벗 오른쪽으로 이동
            temp = L[i];                                                
            L[i] = L[j];
            L[j] = temp;
        }
    }

   temp = L[i];                                                        //피벗을 i와 j 만나는 곳으로 되돌림                                                        
    L[i] = L[r];
    L[r] = temp;
    j = r - 1;

   

   return i;

}

void rQuicksort(int* A,int l, int r,char*mode){
    
    
    if(r - l >= Limit){                                         

        int pivot = findPivot(A,l,r,mode);                            //피벗 찾기                       

        int a = inPlacePartition(A,l,r,pivot);                        //제자리 분할 하고 피벗 위치 반환
        

        rQuicksort(A,l,a - 1, mode);                                  //피벗을 기준으로 나눠서 다시 재귀적으로 sort 진행
        rQuicksort(A,a + 1, r, mode);
    }

}
void insertionSort(int* A, int n)                                      //삽입 정렬
{
     for(int i = 1; i < n; i++)
    {
        for(int j = 0; j < i; j++)
        {
            if(A[i] < A[j])
            {
                int tmp1 = A[i];
                int tmp2 = A[j];
                
                for(int k = 0; k < i - j; k++)
                {
                    int tmp3 = A[j + k + 1];
                    A[j + k + 1] = tmp2;
                    tmp2 = tmp3;
                    
                    
                }
                A[j] = tmp1;
                
            }
        }
        
    }
}
void quickSort(int* A, char* mode)            
{   

    rQuicksort(A, 0, n - 1, mode);                                                                                      
    if (Limit > 1)
    {
       insertionSort(A, n);                                                                            //quicksort에서 정렬 못한거는 insertionSort에서 마저 해줌
    }
    
}

int main()
{
    int* A = createArray();
    char mode[4][20] = {"deterministic1", "deterministic3", "randomized1", "randomized3"};
    for(int i = 0; i< 4; i++)
    {
        Limit = Limits[i];
        char*pointer = 0;
        for(int j =0; j< 4; j++)
        {
            int*A1 = (int*)malloc(sizeof(int)*n);
            for(int k = 0; k< n; k++)
            {
                A1[k] = A[k];
            }
            pointer = mode[j];
            clock_t start, end;

            start = clock();
            quickSort(A1, pointer);
            end = clock();

            double duration = (double)(end - start);                          //걸린 시간 나타내기
            printf("%lf ",duration);
            free(A1);
            
        }
        printf("\n");
    }
    free(A);
    return 0;
}