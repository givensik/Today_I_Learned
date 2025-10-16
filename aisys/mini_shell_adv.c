#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <termios.h>
#include <errno.h>
#include <signal.h>

#define MAXLINE 1024
#define MAXARGS 64

// 문자열을 공백 기준으로 나누어 argv 배열 생성
void parse_line(char *line, char **argv) {
    int i = 0;
    char *token = strtok(line, " \t\n");
    while (token != NULL && i < MAXARGS - 1) {
        argv[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    argv[i] = NULL;
}

int main(void) {
    char line[MAXLINE];
    char *argv[MAXARGS];
    pid_t pid;
    int status;

    // 해당 시그널을 무시
    signal(SIGTTIN, SIG_IGN);
    signal(SIGTTOU, SIG_IGN);
    signal(SIGTSTP, SIG_IGN);

    while (1) {
        printf("mini-shell> ");
        fflush(stdout);

        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("\n");
            break; // EOF (Ctrl+D)
        }
        if (line[0] == '\n') continue;

        parse_line(line, argv);
        if (argv[0] == NULL) continue;

        // 내장 명령어 처리
        if (strcmp(argv[0], "exit") == 0) break;
        if (strcmp(argv[0], "cd") == 0) {
            if (argv[1] == NULL) {
                fprintf(stderr, "cd: missing argument\n");
            } else if (chdir(argv[1]) != 0) {
                perror("cd");
            }
            continue;
        }

        // fork → exec → wait
        pid = fork();
        if (pid < 0) {
            perror("fork");
            continue;
        } else if (pid == 0) {
            // --- [자식: 새로운 그룹 leader] ---
            setpgid(0, 0);   // 자식도 자기 그룹 leader
            // foreground 지정은 부모가 해줌
            execvp(argv[0], argv);
            perror("execvp");
            exit(127);
        } else {
            // --- [부모: 자식 그룹 제어] ---
            setpgid(pid, pid);                   // 자식을 그룹 leader로 확정
            tcsetpgrp(STDIN_FILENO, pid);        // 자식을 foreground로

            if (waitpid(pid, &status, WUNTRACED) < 0) {
                perror("waitpid");
            }

            // --- [부모: 다시 셸로 제어 회수] ---
            tcsetpgrp(STDIN_FILENO, getpgrp());
        }
    }

    return 0;
}
