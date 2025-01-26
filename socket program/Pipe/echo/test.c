#include <stdio.h>
#define BUF_SIZE 30
int main(){
    FILE * fp = fopen("echomsg.txt", "wt");
    char msgbuf[BUF_SIZE] = "test";
    int i, len;

    for(i=0; i<BUF_SIZE; i++){
        fwrite((void *)msgbuf, 1, len, fp);
    }
    fclose(fp);
    
    return 0;
}