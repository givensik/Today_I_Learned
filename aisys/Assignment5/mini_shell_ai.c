// mini_shell_ai.c
// 
// AI 기능이 있는 미니 셸
// - Ctrl+T: AI 모드 토글
// - Ctrl+\: AI 응답 중단
// - Ctrl+D: 종료
//
// 플랫폼별 컴파일:
// macOS:  gcc -o mini_shell_ai mini_shell_ai.c
// Linux:  gcc -o mini_shell_ai mini_shell_ai.c -lrt -pthread
//
// Linux 사용 시: 
// - 211번째 줄 근처 Linux 주석 해제 (sem_timedwait 사용)
// - macOS 폴링 코드 주석 처리
//
// AI helper 컴파일:
// gcc -o ai_helper_chat_repl_shm_posix ai_helper_chat_repl_shm_posix.c \
//     -lcurl -lcjson -I/opt/homebrew/include -L/opt/homebrew/lib

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <errno.h>
#include <signal.h>
#include <termios.h>
#include <semaphore.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
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

// --- 전역 상태 ---
static volatile int ai_mode = 0;
static volatile int ai_thinking = 0;


// semaphore 및 공유 메모리 이름
#define SHM_NAME "/ai_shm"
#define SEM_TO_AI "/sem_to_ai"         // 부모 → AI
#define SEM_TO_PARENT "/sem_to_parent" // AI → 부모

// 공유 메모리 구조체
typedef struct {
    char prompt[4096];
    char response[8192];
} ShmBuf;


// 문자열 파싱
void parse_line(char *line, char **argv) {
    int i = 0;
    char *token = strtok(line, " \t\n");
    while (token != NULL && i < MAXARGS - 1) {
        argv[i++] = token;
        token = strtok(NULL, " \t\n");
    }
    argv[i] = NULL;
}

// --- SIGQUIT : AI 모드 토글 ---
void handle_sigquit(int signo) {
    (void)signo; // signo 사용 안함. 안쓰면 경고가 뜸.
    const char msg_on[]  = "\n" COLOR_CYAN "╔════════════════════╗\n"
                           "║   AI MODE ON 🤖    ║\n"
                           "╚════════════════════╝" COLOR_RESET "\n"
                           COLOR_MAGENTA "AI-shell> " COLOR_RESET;
    const char msg_off[] = "\n" COLOR_YELLOW "╔════════════════════╗\n"
                           "║  AI MODE OFF 💤    ║\n"
                           "╚════════════════════╝" COLOR_RESET "\n"
                           COLOR_GREEN "mini-shell> " COLOR_RESET;
    ai_mode = !ai_mode;
    write(STDOUT_FILENO, ai_mode ? msg_on : msg_off,
          ai_mode ? sizeof(msg_on) - 1 : sizeof(msg_off) - 1);
}

// --- SIGINT (Ctrl+\) : AI reasoning 중단 ---
void handle_sigint(int signo) {
    (void)signo; // signo 사용 안함. 안쓰면 경고가 뜸. 
    const char msg[] = "\n" COLOR_RED "⚠️  AI REASONING INTERRUPTED ⚠️" COLOR_RESET "\n";                        
    if (ai_mode && ai_thinking) {
        ai_thinking = 0;
        
        write(STDOUT_FILENO, msg, sizeof(msg) - 1);
    }
}

// --- 터미널 모드 제어 ---
void setup_terminal(struct termios *orig) {
    struct termios new_term;
    tcgetattr(STDIN_FILENO, orig);
    new_term = *orig;
    // ICANON은 유지 (라인 편집 가능), ISIG는 켜서 Ctrl+C 등을 시그널로 변환
    new_term.c_lflag |= ISIG;  // 시그널 활성화
    // Ctrl+T를 VQUIT에 매핑 (SIGQUIT 발생) - AI 모드 토글
    new_term.c_cc[VQUIT] = 20;  // Ctrl+T
    // Ctrl+\를 VINTR에 매핑 (SIGINT 발생) - AI thinking 중단
    new_term.c_cc[VINTR] = 28;  // Ctrl+\ (ASCII 28)
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &new_term);
}
void restore_terminal(struct termios *orig) {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, orig);
}

int main(void) {
    char line[MAXLINE];
    char *argv[MAXARGS];
    pid_t pid;
    pid_t ai_helper_pid;  // AI helper 전용 PID
    int status;
    struct termios orig_termios;

    // shm_open 전에 기존 리소스 제거
    shm_unlink(SHM_NAME);
    sem_unlink(SEM_TO_AI);
    sem_unlink(SEM_TO_PARENT);

    // 공유 메모리 생성
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    // 공유 메모리 열기 실패시 오류 처리
    if (shm_fd < 0) {
        perror("shm_open");
        return 1;
    }

    // 공유 메모리 크기 설정 ftruncate 사용, 에러처리도 같이 해주세요. 
    if (ftruncate(shm_fd, sizeof(ShmBuf)) < 0) {
        perror("ftruncate");
        close(shm_fd);
        return 1;
    }

    // 공유 메모리 매핑, MAP_SHARED, PROT_READ | PROT_WRITE 사용
    ShmBuf *shm = mmap(NULL, sizeof(ShmBuf),
                       PROT_READ | PROT_WRITE,
                       MAP_SHARED, shm_fd, 0);
    // 매핑 실패시 오류 처리
    if (shm == MAP_FAILED) {
        perror("mmap");
        close(shm_fd);
        return 1;
    }

    // 세마포어 생성 (부모가 O_CREAT)
    sem_t *sem_to_child = sem_open(SEM_TO_AI, O_CREAT, 0666, 0);
    // 세마포어 생성 실패시 오류 처리
    if (sem_to_child == SEM_FAILED) {
        perror("sem_open to_ai");
        return 1;
    }

    // 세마포어 생성 (부모가 O_CREAT)
    sem_t *sem_to_parent = sem_open(SEM_TO_PARENT, O_CREAT, 0666, 0);
    if (sem_to_parent == SEM_FAILED) {
        perror("sem_open to_parent");
        return 1;
    }

    // --- 시그널 핸들러 등록 ---
    signal(SIGTTIN, SIG_IGN); // background 프로세스가 터미널에서 읽으려고 하면, 무시
    signal(SIGTTOU, SIG_IGN); // background 프로세스가 터미널에 쓰려고 하면, 무시
    signal(SIGTSTP, SIG_IGN); // Ctrl+Z 무시 (쉘은 멈추면 안됨)
    signal(SIGQUIT, handle_sigquit);  // Ctrl+T
    signal(SIGINT,  handle_sigint);   // Ctrl+backslash

    // 자식 프로세스 생성 (AI 프로세스 실행)
    // 주의: fork 전에 터미널 설정하면 안됨! 자식에 영향
    ai_helper_pid = fork();

    if (ai_helper_pid == 0) {
        // [추가] 자식 프로세스는 터미널 시그널을 기본값으로 복원
        signal(SIGTTIN, SIG_DFL);
        signal(SIGTTOU, SIG_DFL);
        signal(SIGTSTP, SIG_DFL);
        signal(SIGQUIT, SIG_DFL);
        signal(SIGINT, SIG_DFL); // 쉘이 설정한 핸들러 대신 기본 동작 수행
        // 자식: ai_helper_chat_repl_shm_posix 실행
        execl("./ai_helper_chat_repl_shm_posix", "ai_helper_chat_repl_shm_posix", (char *)NULL);
        perror("execl ai_helper_chat_repl_shm_posix");
        fflush(stderr);
        _exit(127);
    } else if (ai_helper_pid < 0) {
        perror("fork");
        return 1;
    }
    
    // 자식이 충분히 초기화될 시간 제공 (ollama 모델 로딩 시간)
    printf(COLOR_CYAN "Initializing AI helper (PID: %d)..." COLOR_RESET "\n", ai_helper_pid);
    sleep(2);  // 2초 대기
    
    // 자식 프로세스가 죽었는지 확인
    int child_status;
    pid_t check = waitpid(ai_helper_pid, &child_status, WNOHANG);
    if (check > 0) {
        fprintf(stderr, COLOR_RED "Error: AI helper process exited prematurely (status: %d)\n" COLOR_RESET, WEXITSTATUS(child_status));
        fprintf(stderr, "Check if ./ai_helper_chat_repl_shm_posix exists and is executable\n");
        return 1;
    }
    
    printf(COLOR_GREEN "✓ AI helper ready\n" COLOR_RESET);
    
    // === AI helper 연결 테스트 ===
    printf(COLOR_CYAN "Testing AI helper connection..." COLOR_RESET "\n");
    
    strncpy(shm->prompt, "hi", sizeof(shm->prompt)-1);
    shm->prompt[sizeof(shm->prompt)-1] = '\0';
    
    sem_post(sem_to_child);
    
    // 10초 타임아웃으로 응답 대기
    int test_timeout = 10;
    int test_ok = 0;
    
    for (int i = 0; i < test_timeout * 10; i++) {
        if (sem_trywait(sem_to_parent) == 0) {
            test_ok = 1;
            break;
        }
        usleep(100000);
        if (i % 10 == 0 && i > 0) {
            printf(".");
            fflush(stdout);
        }
    }
    
    if (test_ok) {
        printf("\n" COLOR_GREEN "✓ AI helper test passed!" COLOR_RESET "\n");
        printf(COLOR_GRAY "  Response preview: %.60s...\n" COLOR_RESET, shm->response);
    } else {
        printf("\n" COLOR_RED "✗ AI helper test FAILED (timeout)\n" COLOR_RESET);
        printf(COLOR_YELLOW "Possible issues:\n" COLOR_RESET);
        printf("  1. Ollama not running\n");
        printf("  2. Model 'gemma3:1b' not installed\n");
        printf("  3. AI helper crashed\n");
        
        // 프로세스 상태 확인
        pid_t test_check = waitpid(ai_helper_pid, &child_status, WNOHANG);
        if (test_check > 0) {
            printf(COLOR_RED "  → AI helper process exited! (status: %d)\n" COLOR_RESET, WEXITSTATUS(child_status));
        } else {
            printf(COLOR_YELLOW "  → AI helper still running but not responding\n" COLOR_RESET);
        }
        
        printf("\nContinuing anyway (AI mode will not work)...\n");
    }
    
    // 부모 터미널 설정 (fork 후에 해야 자식에 영향 안 줌)
    setup_terminal(&orig_termios);
    
    // 부모 : mini-shell 역할
    printf(COLOR_CYAN "╔═══════════════════════════════════════════════════════════╗\n");
    printf("║         🚀 Mini Shell with AI Mode 🤖                     ║\n");
    printf("╠═══════════════════════════════════════════════════════════╣\n");
    printf("║  " COLOR_YELLOW "Ctrl+T" COLOR_CYAN "  : Toggle AI Mode                                 ║\n");
    printf("║  " COLOR_YELLOW "Ctrl+\\" COLOR_CYAN "  : Stop AI Thinking                               ║\n");
    printf("║  " COLOR_YELLOW "Ctrl+D" COLOR_CYAN "  : Exit Shell                                     ║\n");
    printf("╚═══════════════════════════════════════════════════════════╝" COLOR_RESET "\n");
    printf(COLOR_GREEN "mini-shell> " COLOR_RESET);
    fflush(stdout);

    
    while (1) {
        // fgets로 라인 단위 입력 (편집 가능!)
        if (fgets(line, MAXLINE, stdin) == NULL) {
            printf("\n");
            break;  // EOF (Ctrl+D)
        }

        // 빈 줄 처리
        if (line[0] == '\n') {
            printf(ai_mode ? COLOR_MAGENTA "AI-shell> " COLOR_RESET : COLOR_GREEN "mini-shell> " COLOR_RESET);
            fflush(stdout);
            continue;
        }
        char line_copy[MAXLINE];
        strcpy(line_copy, line);

        parse_line(line, argv);
        if (argv[0] == NULL) {
            printf(ai_mode ? COLOR_MAGENTA "AI-shell> " COLOR_RESET : COLOR_GREEN "mini-shell> " COLOR_RESET);
            fflush(stdout);
            continue;
        }

        if (strcmp(argv[0], "exit") == 0)
            break;

        // --- AI 모드 ---
        if (ai_mode) {
            ai_thinking = 1;
            printf(COLOR_CYAN "🤖 [AI] " COLOR_YELLOW "Thinking deeply about '%s'..." COLOR_RESET "\n", argv[0]);
            fflush(stdout);

            // 공유 메모리에 질문 쓰기
            printf("질문 : %s", line_copy); // 디버그용 출력
            strncpy(shm->prompt, argv[0], sizeof(shm->prompt)-1);
            shm->prompt[sizeof(shm->prompt)-1] = '\0';

            // AI에게 알림(세마포어 이용)
            sem_post(sem_to_child);
            
            // AI 응답 대기 (30초 타임아웃)
            // macOS용: sem_timedwait 미지원으로 sem_trywait + 폴링 사용
            // Linux용: 아래 주석 해제하고 위 폴링 코드 주석처리
            /*
            // === Linux 타임아웃 구현 (sem_timedwait 사용) ===
            struct timespec timeout;
            clock_gettime(CLOCK_REALTIME, &timeout);
            timeout.tv_sec += 30;  // 30초 타임아웃
            
            int wait_result = sem_timedwait(sem_to_parent, &timeout);
            
            if (wait_result == -1) {
                if (errno == ETIMEDOUT) {
                    printf("\n" COLOR_RED "⏱️  [AI] Response timeout (30s)" COLOR_RESET "\n");
                } else {
                    perror("sem_timedwait");
                }
            } else {
                printf("\n" COLOR_GREEN "[AI] %s" COLOR_RESET "\n", shm->response);
            }
            */
            
            // === macOS 타임아웃 구현 (폴링 방식) ===
            int timeout_sec = 30;
            int wait_result = -1;
            
            for (int i = 0; i < timeout_sec * 10; i++) {
                if (sem_trywait(sem_to_parent) == 0) {
                    wait_result = 0;  // 성공
                    break;
                }
                usleep(100000);  // 0.1초 대기
                
                // 진행 표시 (매 초마다 점 출력)
                if (i % 10 == 0 && i > 0) {
                    printf(COLOR_CYAN "." COLOR_RESET);
                    fflush(stdout);
                }
            }
            
            if (wait_result == -1) {
                printf("\n" COLOR_RED "⏱️  [AI] Response timeout (30s)" COLOR_RESET "\n");
            } else {
                // 응답 출력
                printf("\n" COLOR_GREEN "[AI] %s" COLOR_RESET "\n", shm->response);
            }

            ai_thinking = 0;
            printf(COLOR_MAGENTA "AI-shell> " COLOR_RESET);
            fflush(stdout);
            continue;
        }

        // --- 내부 명령어: cd ---
        if (strcmp(argv[0], "cd") == 0) {
            if (argv[1] == NULL)
                fprintf(stderr, COLOR_RED "cd: missing argument" COLOR_RESET "\n");
            else if (chdir(argv[1]) != 0)
                perror("cd");
            printf(COLOR_GREEN "mini-shell> " COLOR_RESET);
            fflush(stdout);
            continue;
        }

        // --- 외부 명령 실행 ---
        pid = fork();
        if (pid < 0) {
            perror("fork");
        } else if (pid == 0) {
            // 자식: 원래 터미널 모드로 복원
            restore_terminal(&orig_termios);
            setpgid(0, 0);
            execvp(argv[0], argv);
            perror("execvp");
            exit(127);
        } else {
            // 부모: 원래 터미널 모드로 복원
            restore_terminal(&orig_termios);
            setpgid(pid, pid);
            tcsetpgrp(STDIN_FILENO, pid);

            if (waitpid(pid, &status, WUNTRACED) < 0)
                perror("waitpid");

            // 복귀 후 다시 설정
            tcsetpgrp(STDIN_FILENO, getpgrp());
            setup_terminal(&orig_termios);
        }

        printf(COLOR_GREEN "mini-shell> " COLOR_RESET);
        fflush(stdout);
    }

    restore_terminal(&orig_termios);
    printf(COLOR_CYAN "\n👋 Goodbye!\n" COLOR_RESET);


    // 종료 처리
    // AI에게도 종료 신호 보내고 싶으면 필요시 여기서 사용:
    strncpy(shm->prompt, "exit", sizeof(shm->prompt));
    // AI에게 알림(세마포어 이용)
    sem_post(sem_to_child); 

    // AI helper 프로세스 대기
    waitpid(ai_helper_pid, NULL, 0);    // 공유 메모리 및 세마포어 정리
    munmap(shm, sizeof(ShmBuf));
    close(shm_fd);

    // 세마포어 닫기
    sem_close(sem_to_child);
    sem_close(sem_to_parent);

    // 세마포어 및 공유 메모리 제거(sem_unlink, shm_unlink)
    sem_unlink(SEM_TO_AI);
    sem_unlink(SEM_TO_PARENT);
    shm_unlink(SHM_NAME);

    return 0;
}
