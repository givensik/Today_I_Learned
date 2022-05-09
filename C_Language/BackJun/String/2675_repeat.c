#include <stdio.h>

void new_string(char *x, int N);

int main(){

    int N;
    char x[21];
    scanf("%d",&N);
    int i,j;
    int n;
    for(i=0;i<N;i++){
        scanf("%d %s",&n,x);
        getchar();
        new_string(x,n);
    }

    return 0;
}
void new_string(char *x, int N){
    char *p;
    int i;
    for(p=x;*p;p++){
        for(i=0;i<N;i++){
            printf("%c",*p);
        }
    }
    printf("\n");
}