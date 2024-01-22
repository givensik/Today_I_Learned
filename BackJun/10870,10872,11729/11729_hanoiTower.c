#include <stdio.h>
#include <math.h>
void hanoi(int N, int from, int by, int to);

int main(){

    int N;
    scanf("%d",&N);

    printf("%d\n",(int)(pow(2,N)-1));
    hanoi(N,1,2,3);

    return 0;
}
void hanoi(int N, int from, int by, int to){
    if(N==1){
        printf("%d %d\n",from,to);
    }else{
        hanoi(N-1,from,to,by);
        printf("%d %d\n",from,to);//왜냐면 이 전에 n-1개를 다른데다 옮겨두고 젤 큰애를 C로 옮기는거니까
        hanoi(N-1,by,from,to);
    }
}