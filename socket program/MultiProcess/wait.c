/*
    gcc wait.c -o wait
    ./wait
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char **argv) {
    int status;
    pid_t pid = fork();

    if(pid==0){ // 자식 프로세스
        return 3;
    }else{
        printf("Child PID : %d\n", pid); // 부모 프로세스의 pid값은 자식 프로세스 pid임
        pid = fork();
        if(pid==0){ 
            exit(7);
        }else{
            printf("Child PID : %d\n", pid);
            wait(&status); // 자식 프로세스가 종료되면 status에 값을 전달함
            if(WIFEXITED(status)){
                printf("Child send one : %d\n", WEXITSTATUS(status)); 
            }

            wait(&status);
            if(WIFEXITED(status)){
                printf("Child send two : %d\n", WEXITSTATUS(status));
            }
            sleep(30); 
        }
    }

    return 0;
}