/*
    gcc pipe3.c -o pipe3
*/
#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main(int argc, char **argv){
    int fds1[2], fds2[2];
    char str1[] = "Who are you?";
    char str2[] = "Thank you for your message";
    char buf[BUF_SIZE];
    pid_t pid;

    pipe(fds1), pipe(fds2);
    pid = fork();
    // fds1 자식 프로세스 -> 부모 프로세스
    // fds2 부모 프로세스 -> 자식 프로세스
    if(pid == 0){ // 자식 프로세스일 때
        write(fds1[1], str1, sizeof(str1)); // 자식 프로세스가 보내는 파이프 = fds1[1]
        read(fds2[0], buf, BUF_SIZE); 
        printf("Child proc output : %s\n", buf);
    }else{
        read(fds1[0], buf, BUF_SIZE); // 자식 프로세스가 보낸 값을 받아오는 파이프 = fds1[0]
        printf("Parent Proc output: %s\n", buf);
        write(fds2[1], str2, sizeof(str2));
        sleep(3);
    }

    return 0;
}