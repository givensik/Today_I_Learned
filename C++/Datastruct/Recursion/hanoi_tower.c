#include <stdio.h>

void hanoi(int N, char from, char by, char to);

int main(){

    int N;
    scanf("%d",&N);

    hanoi(N,'A','B','C');

    return 0;
}
void hanoi(int N, char from, char by, char to){
    if(N==1){
        printf("원반 1을 %c에서 %c로 이동\n",from,to);
    }else{
        hanoi(N-1,from,to,by);
        printf("원반 %d를 %c에서 %c로 이동\n",N,from,to);//왜냐면 이 전에 n-1개를 다른데다 옮겨두고 젤 큰애를 C로 옮기는거니까
        hanoi(N-1,by,from,to);
    }
}