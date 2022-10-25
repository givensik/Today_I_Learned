#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int n = 100000;
int Limits = {1,100,500,1000};


int* createArray(){
    int i;
    int *A = (int*)malloc(sizeof(int)*n);
    srand(time(NULL));
    for(i=0;i<n;i++){
        A[i]= random() % n +1;
    }
    return A;
}



int* quickSort(int *A, int n){

    
}


int main(){

    int *A = createArray();
    int *B = (int*)malloc(sizeof(int)*n);
    int i,j,k;
    for(i=0;i<4;i++){
        for(j=0;j<4;j++){
            //A를 B에 복사해서 B에 quickSort 실행
            for(k=0;k<n;k++){
                B[k]=A[k];
            }
            

        }
    }

    return 0;
}