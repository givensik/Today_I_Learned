#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

// 간단한 미니 셸: ai_helper_chat 프로그램과 파이프로 통신

int main(void) {
    // 파이프를 위한 파일 디스크립터 배열
    // your code here
    // your code here
    int fd1[2]; // 부모 -> 자식
    int fd2[2]; // 자식 -> 부모

    // 파이프 생성: 부모 -> 자식, 자식 -> 부모
    // 둘 중 하나라도(hint : or 연산) 실패하면 종료
    if (pipe(fd1) == -1 || pipe(fd2) == -1) {
        perror("pipe");
        return 1;
    }

    // 자식 프로세스 생성
    pid_t pid = fork();

    if (pid == 0) {
        // 자식 프로세스: ai_helper_chat 실행

        // 표준 입력을 파이프로 리다이렉트
        dup2(fd1[0], STDIN_FILENO);
        
        // 표준 출력을 파이프로 리다이렉트
        dup2(fd2[1], STDOUT_FILENO);

        // 사용하지 않는 파이프 끝 닫기
        close(fd1[0]);
        close(fd1[1]);
        close(fd2[0]);
        close(fd2[1]);
        
        // ai_helper_chat 실행
        execl("./test_ai_helper", "test_ai_helper", (char *)NULL);
        perror("execl");
        _exit(127);
    }

    // 부모인 경우 자식에게는 쓰기 파이프 열고, 자식으로부터는 읽기 파이프 열기
    // 나머지는 닫음. 
    close(fd1[0]);
    close(fd2[1]);

    char buf[1024];
    while (1) {
        printf("mini-shell> ");
        fflush(stdout);

        // 사용자 입력 읽기 fgets() 사용, stdin에서 읽기
        if (!fgets(buf, sizeof(buf), stdin))  
            break;
        if (strncmp(buf, "exit", 4) == 0) 
            break;

        // 자식에게 가는 파이프에 입력 전송
        write(fd1[1], buf, strlen(buf));

        // 자식한테서 오는 파이프에 한 줄 응답 읽기, 오기 전까지는 blocking이 됨
        int n = read(fd2[0], buf, sizeof(buf) - 1);
        if (n > 0) {
            buf[n] = '\0';
            printf("[AI] %s", buf);
        } else {
            break;
        }
    }

    // 파이프 둘다 닫기 
    close(fd1[1]);
    close(fd2[0]);

    // 자식 프로세스 대기
    waitpid(pid, NULL, 0);
    return 0;
}
