/*
    gcc pipe1.c -o pipe1
*/
#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main(int argc, char **argv){
    int fds[2];
    char str[] = "Who are you?";
    char buf[BUF_SIZE];
    pid_t pid;

    pipe(fds); // 프로세스의 파이프 생성
    pid=fork(); 

    if(pid==0){ // 자식 프로세스일 때
        write(fds[1], str, sizeof(str)); // 파이프에 "Who are you?" 라는 값을 넣어둠
    }else{
        read(fds[0], buf, BUF_SIZE); // 파이프에 저장된 값을 읽어 오기
        puts(buf);
    }

    return 0;
}