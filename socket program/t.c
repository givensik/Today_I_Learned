// newproc-posix.c

#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

int main(){

	pid_t pid;

	pid = fork();

    if(pid < 0 ){ // 프로세스 생성에 실패한 경우 pid는 음수 값이 출력된다.
        perror("fork() error!");
        return -1;
    }

    if(pid == 0){ // pid가 0일 때, 자식 프로세스가 실행된다.
		printf("I am the child %d\n", pid); // 자식 프로세스인 경우
		execlp("/bin/ls","ls",NULL); // "/bin/ls"에 있는 ls 명령어를 실행하는 명령, 
		exit(1); // 프로세스를 종료하는 명령어
	}else{ // in Parent Process
		printf("I am the parent %d\n", pid); // 부모 프로세스의 경우 자식 프로세스 id를 가진다. 
		wait(NULL); // 자식프로세스 기다리기
		printf("Child Complete\n"); // 자식 프로세스가 종료되면 wait() 함수가 반환되면서 "Child Complete"가 출력된다.
	}

	return 0;
}