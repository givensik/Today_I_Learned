#include <stdio.h>
#include <string.h>

int a,b,cnt;
char YN[100];
int main(){
    scanf("%d %d %d",&a,&b,&cnt);
    getchar();
    scanf("%s",YN);
    //printf("%s %d",YN, (int)strlen(YN));
    for(int i=0;i<(int)strlen(YN);i++){
        if(a == b){
            break;
        }
        if(YN[i] =='Y'){
            a = (a+b)/2 + 1;
        }else if(YN[i] =='N'){
            b = (a+b)/2;
        }
    }
    printf("%d\n",a);
    return 0;
}