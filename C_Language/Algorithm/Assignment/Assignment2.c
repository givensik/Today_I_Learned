#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int n = 100000; //100000
int Limits[4] = {1,100,500,1000};
int limit;
int mode;

int* createArray(){
    int i;
    int *A = (int*)malloc(sizeof(int)*n);//사이즈가 n인 리스트 선언
    srand((unsigned)time(NULL));
    for(i=0;i<n;i++){//0~n사이의 수 넣기
        A[i]= random()%n +1;
    }
    return A;
}

//인덱스가 a,b,c인 애들의 중앙값 찾기
int findIndexOfMedianOfThree(int *A, int a, int b, int c){
    if ((A[a] >= A[b] && A[a] <= A[c]) || (A[a] <= A[b] && A[a] >= A[c])){
        return a; 
   }else if((A[b] >= A[a] && A[b] <= A[c]) || (A[b] <= A[a] && A[b] >= A[c])){
        return b;
   }else{
        return c;
   }
}

int findPivot(int *A, int l, int r){
    // printf("findPivot mode = %d\n",mode );
    int p;
    if(mode == 0){//deterministic1
        return r;
    }else if(mode == 2){//randomized1
        srand((unsigned)time(NULL));
        p = random()%(r-l+1)+l;
        return p; // l과 r사이의 수 반환
    }
    int a,b,c;
    switch(mode){
        case 1:
            //a,b,c에 결정된 값
            a = l;
            b = (l+r)/2;
            c = r;
            break;
        case 3:
            //a,b,c에 랜덤값
            srand((unsigned)time(NULL));
            a = random()%(r-l+1)+l;
            b = random()%(r-l+1)+l;
            c = random()%(r-l+1)+l;
            break;
    }
    
    return findIndexOfMedianOfThree(A, a, b, c);//A[a], A[b], A[c] 중 중간값의 index 반환
    
}
int inPlacePartition(int *A,int l,int r,int k){
    // printf("inPlacePartition\n");
    int p = A[k];//pivot
    //pivot을 r자리에 옮기기
    int tmp = A[k];
    A[k] = A[r];
    A[r] = tmp;
    int i = l,j = r-1;
    //printf("i = %d, j =%d\n",i,j);
    while(i<=j){
        while(i<=j && A[i]<=p){//왼쪽일때, p보다 작은 값이면 i++
            
            i++;
        }
        while(i<=j && A[j]>p){
            j--;
        }
        
        if(i<j){//교환
            tmp = A[i];
            A[i] = A[j];
            A[j] = tmp;
        }
    }

    //중간값인애들 데려오기 i > j일때, i = a

    tmp = A[i];                                                        
    A[i] = A[r];
    A[r] = tmp;

    
    return i;
}



int rQuickSort(int* A, int l, int r){

    int k;
    int a;
    // printf("r = %d,l=%d, limit = %d\n",r,l,limit);
    if(r-l >= limit){ //l과 r 사이의 차이값이 limit보다 클때 quickSort 진행, 아니면 insertionSort으로 보내기
        k = findPivot(A,l,r);
        //printf("k=%d\n",k);
        a = inPlacePartition(A,l,r,k);
        //printf("a= %d, b= %d\n",a,b);
        rQuickSort(A,l,a-1);
        rQuickSort(A,a+1,r);
    }
    
    return 0;
}


void insertionSort(int* A, int n){
     for (int i = 1; i < n; i++){
        int j = i;
        while (j > 0 && A[j - 1] > A[j]){//작은 인덱스인애가 큰 인덱스인애 보다 클때, 교체
            int tmp = A[j];
            A[j] = A[j - 1];
            A[j - 1] = tmp;
            j--;
        }
    }

}

int* quickSort(int *A, int n){

    //printf("quickSort\n");
    //printf("limit =%d\n",limit);
    rQuickSort(A,0,n-1);

    if(limit > 1){
        //printf("insertionSort\n");
        insertionSort(A, n);
    }
    // for(int i =0;i<n;i++){
    //     printf(" %d",A[i]);
    // }
    // printf("\n");

    return A;
}


int main(){

    int i,j,k;
    
    // for(i=0;i<n;i++){
    //     printf(" %d",A[i]);
    // }
    // printf("\n");
    
    for(i=0;i<4;i++){
        int *A = createArray();
        limit = Limits[i];//limit 지정
        // printf("Limit = %d\n",limit);
        
        for(j=0;j<4;j++){
            int *B = (int*)malloc(sizeof(int)*n);
            mode = j;
            //printf("limit = %d, mode = %d\n",limit,mode);
            clock_t start, end;
            // printf("mode = %d\n",mode);
            //A를 B에 복사해서 B에 quickSort 실행
            for(k=0;k<n;k++){
                B[k]=A[k];
            }
            start = clock();
            quickSort(B,n);//quicksort
            end = clock();
            double duration = (double)(end - start);    //걸린 시간 나타내기
            printf("%lf ",duration);
            free(B);
        }
        printf("\n");
        free(A);
    }

    return 0;
}