#include <stdio.h>
#include <unistd.h>
#define BUF_SIZE 30

int main(int argc, char *argv[]){
    int fds[2];
    char str1[]="Who are you?";
    char str2[]="Thank you for your message.";
    char buf[BUF_SIZE];
    pid_t pid;

    pipe(fds); // 파이프 생성
    pid = fork(); // 프로세스 생성
    if(pid == 0){ // 자식 프로세스
        write(fds[1], str1, sizeof(str1)); // 파이프로 Who are you? 이거 메시지 보내기
        // sleep(2); // 2초 기다리기 이 코드가 없을 경우, 자신이 보낸 값 Who are you를 출력함
        read(fds[0], buf, BUF_SIZE); // 파이프에서 내용을 읽어서 buf에 저장하기
        printf("Child proc output : %s\n", buf);  // buf 출력
    }else{
        read(fds[0], buf, BUF_SIZE); // 부모 프로세스가 파이프를 통해 보낸 값을 가져와서 buf에 저장
        printf("Parent proc output : %s\n", buf); // 받은 값 출력
        write(fds[1], str2, sizeof(str2)); // str2를 파이프를 통해서 출력
        sleep(3);
    }
    
    return 0;
}