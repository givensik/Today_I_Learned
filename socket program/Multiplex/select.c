/*
    gcc select.c -o select
*/
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>
#define BUF_SIZE 30

int main(int argc, char *argv[]){

    fd_set reads, temps;
    int result, str_len;
    char buf[BUF_SIZE];
    struct timeval timeout;

    FD_ZERO(&reads);
    FD_SET(0, &reads); // 파일디스크립터 0 = standard input임

    /*
    timeouts.tv_sec = 5;
    timeouts.tv_usec = 5000;
    */

    while(1){
        temps = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        
        result = select(1, &temps, 0, 0, &timeout); // readset(수신된 데이터 여부)를 확인하려하는거임
        if(result == -1){ // -1 이면 오류난 거
            puts("select() error! ");
            break;
        }else if(result == 0){ // 0 이면 타임아웃
            puts("Time-out! ");
            
        }else{
            if(FD_ISSET(0, &temps)){
                str_len = read(0, buf, BUF_SIZE);
                buf[str_len] = 0;
                printf("message from console : %s", buf);
            }
        }
    }


    return 0;
}