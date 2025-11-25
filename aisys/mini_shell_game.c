#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>

#define MAXLINE 1024
#define MAXARGS 64

// --- ANSI 색상 코드 ---
#define COLOR_RESET   "\033[0m"
#define COLOR_RED     "\033[1;31m"
#define COLOR_GREEN   "\033[1;32m"
#define COLOR_YELLOW  "\033[1;33m"
#define COLOR_BLUE    "\033[1;34m"
#define COLOR_MAGENTA "\033[1;35m"
#define COLOR_CYAN    "\033[1;36m"
#define COLOR_WHITE   "\033[1;37m"
#define COLOR_GRAY    "\033[0;90m"


// --- 게임 상태 (Boss HP) ---
static double hp_boss = 1000.0;

// --- HP 보호용 뮤텍스(전역) hp_mutex ---
pthread_mutex_t hp_mutex = PTHREAD_MUTEX_INITIALIZER;

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

// --- 공격 스레드에 쓰일 구조체 ---
typedef struct {
    double damage;
    int thread_id;
} attack_arg_t;

// --- Attack 스레드 함수 ---
// 구현 조건
// 각 스레드는 damage 만큼 hp_boss를 깎음
// 0.01 단위로 나누어 여러번 깎음 (동시성 테스트용)
// 한번에 0.01씩 깎고 1ms 대기
// 뮤텍스로 hp_boss 보호

void* attack_thread(void* arg) {
    // 위 설명에 따라 코드를 작성하세요. 
    // your code here

    return (void *)NULL;
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
            // attack 명령어 처리
            if (strcmp(argv[0], "attack") == 0) {
                printf(COLOR_CYAN "🎮 [BOSS BATTLE] Current Boss HP: %.2f\n" COLOR_RESET, hp_boss);
                
                // 공격 인자 개수 세기
                int num_attacks = 0;
                for (int i = 1; argv[i] != NULL && i < MAXARGS; i++) {
                    num_attacks++;
                }
                
                if (num_attacks == 0) {
                    printf(COLOR_RED "Usage: attack <damage1> <damage2> ...\n" COLOR_RESET);
                    printf(COLOR_MAGENTA "AI-shell> " COLOR_RESET);
                    fflush(stdout);
                    continue;
                }
                
                // 스레드를 생성할 수 있도록 스레드 배열 준비
                // your code here
                
                // 각 공격 인자마다 스레드 생성
                for (int i = 1; argv[i] != NULL && i < MAXARGS; i++) {
                    int damage = atoi(argv[i]);
                    // 공격이 음수이면 무시
                    if (damage <= 0) {
                        printf(COLOR_RED "Invalid damage: %s\n" COLOR_RESET, argv[i]);
                        continue;
                    }
                    
                    // 아까 작성한 attack_arg_t 구조체에 인자 채우기
                    // your code here
                    
                    // 스레드 생성
                    // your code here
                }
                
                // 모든 스레드 종료 대기
                // your code here
                
                // 보스는 부활하는 걸로 해봅시다. hp가 0 이하가 되면 1000으로 리셋
                printf(COLOR_CYAN "🎮 [RESULT] Boss HP: %.2f" COLOR_RESET, hp_boss);
                if (hp_boss <= 0) {
                    printf(COLOR_GREEN " 💀 BOSS DEFEATED!\n" COLOR_RESET);
                    hp_boss = 1000.0;  // 리셋
                    printf(COLOR_YELLOW "🔄 Boss respawned with 1000 HP!\n" COLOR_RESET);
                } else {
                    printf("\n");
                }
                
                printf(COLOR_MAGENTA "AI-shell> " COLOR_RESET);
                fflush(stdout);
                continue;
            }
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
