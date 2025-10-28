#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>

#define MAXLINE 1024
#define MAXARGS 64

static volatile int ai_mode=0;

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

// SIG-QUIT 핸들러
void handle_sigquit(int signo){
    ai_mode = !ai_mode;
    if(ai_mode){
        write(STDOUT_FILENO, "\n[AI MODE ON]\n", 15);
        write(STDOUT_FILENO, "ai-shell> ", 10);
    } else {
        write(STDOUT_FILENO, "\n[AI MODE OFF]\n", 16);
        write(STDOUT_FILENO, "mini-shell> ", 12);
    }
}

int main(void) {
    char line[MAXLINE];
    char *argv[MAXARGS];
    pid_t pid;
    int status;
    // SIGNALS을 받아도 무시, 근데 자식 프로세스한테는 SIGNAL이 감
    signal(SIGTTIN, SIG_IGN); // background process가 터미널 읽으려 할 때 무시
    signal(SIGTTOU, SIG_IGN); // background process가 터미널 쓰려 할 때 무시
    signal(SIGINT, SIG_IGN); // Ctrl-C 무시
    signal(SIGTSTP, SIG_IGN); // Ctrl-Z 무시
    signal(SIGQUIT, handle_sigquit); // Ctrl-\ 핸들러 등록

    while (1) {
        if(ai_mode){
            printf("ai-shell> ");
        } else {
            printf("mini-shell> ");
        }
        fflush(stdout);

        // 입력 없으면 EOF 처리
        if (fgets(line, sizeof(line), stdin) == NULL) {
            printf("\n");
            break;
        }
        
        // 공백/엔터만 입력 시 무시
        if (line[0] == '\n') continue;

        // 명령 파싱
        parse_line(line, argv);
        if (argv[0] == NULL) continue;

        // 내장 명령어 처리
        if (strcmp(argv[0], "exit") == 0) {
            break;
        }

        if(ai_mode){
            printf("[AI] AI 모드에서는 명령을 실행할 수 없습니다. (exit으로 종료)\n");
            continue;
        }

        if (strcmp(argv[0], "cd") == 0) {
            if (argv[1] == NULL) {
                fprintf(stderr, "cd: missing argument\n");
            } else {
                if (chdir(argv[1]) != 0) {
                    perror("cd");
                }
            }
            continue;
        }

        // fork → exec → wait
        pid = fork();
        if (pid < 0) {
            perror("fork");
            continue;
        } else if (pid == 0) {
            /*
             * Child: put itself into a new process group (pgid == pid)
             * restore default signal handlers for job-control signals
             * then exec the program
             */
            if (setpgid(0, 0) < 0) {
                /* non-fatal; continue */
            }

            /* restore default handlers so child reacts to signals like Ctrl-C */
            // 위에서 SIG_IGN 했던거 다시 원래대로 돌리는거
            signal(SIGINT, SIG_DFL);
            signal(SIGTSTP, SIG_DFL);
            signal(SIGQUIT, SIG_DFL);

            /* 자식 프로세스: 명령 실행 */
            execvp(argv[0], argv);
            /* execvp가 반환하면 실패한 것 */
            perror("execvp");
            exit(127);
        } else {
            /*
             * Parent: ensure child's pgid is set, give terminal foreground to child
             * then wait for child to exit or stop. After that restore terminal to shell.
             */
            /* try to set child's pgid in case child hasn't done so yet */
            if (setpgid(pid, pid) < 0) {
                /* ignore possible race errors */
            }
            
            /* Prevent the parent from being stopped by TTOU when changing terminal owner */
            void (*old_sig)(int) = signal(SIGTTOU, SIG_IGN);
            // STDIN_FILENO = stdin 제어권 = 터미널 제어권 -> 자식 프로세스에게 넘김
            if (tcsetpgrp(STDIN_FILENO, pid) < 0) {
                perror("tcsetpgrp");
            }
            /* restore SIGTTOU handler */
            signal(SIGTTOU, old_sig);

            /* wait for child (also return when child is stopped) */
            if (waitpid(pid, &status, WUNTRACED) < 0) {
                perror("waitpid");
            }

            /* restore terminal to shell's process group */
            old_sig = signal(SIGTTOU, SIG_IGN);
            if (tcsetpgrp(STDIN_FILENO, getpgrp()) < 0) {
                perror("tcsetpgrp");
            }
            signal(SIGTTOU, old_sig);
        }
    }

    return 0;
}
