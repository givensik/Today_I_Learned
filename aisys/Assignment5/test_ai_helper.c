// test_ai_helper.c
// ai_helper_chat_repl_shm_posix 동작 확인용 간단한 테스트 프로그램

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>

#define SHM_NAME "/ai_shm"
#define SEM_TO_AI "/sem_to_ai"
#define SEM_TO_PARENT "/sem_to_parent"

typedef struct {
    char prompt[4096];
    char response[8192];
} ShmBuf;

int main(void) {
    printf("=== AI Helper Test ===\n");
    
    // 기존 리소스 정리
    shm_unlink(SHM_NAME);
    sem_unlink(SEM_TO_AI);
    sem_unlink(SEM_TO_PARENT);
    
    // 공유 메모리 생성
    int shm_fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    if (shm_fd < 0) {
        perror("shm_open");
        return 1;
    }
    
    if (ftruncate(shm_fd, sizeof(ShmBuf)) < 0) {
        perror("ftruncate");
        close(shm_fd);
        return 1;
    }
    
    ShmBuf *shm = mmap(NULL, sizeof(ShmBuf), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (shm == MAP_FAILED) {
        perror("mmap");
        close(shm_fd);
        return 1;
    }
    
    // 세마포어 생성
    sem_t *sem_to_child = sem_open(SEM_TO_AI, O_CREAT, 0666, 0);
    sem_t *sem_to_parent = sem_open(SEM_TO_PARENT, O_CREAT, 0666, 0);
    
    if (sem_to_child == SEM_FAILED || sem_to_parent == SEM_FAILED) {
        perror("sem_open");
        return 1;
    }
    
    printf("✓ Shared memory and semaphores created\n");
    
    // AI helper 프로세스 시작
    pid_t pid = fork();
    
    if (pid == 0) {
        // 자식: ai_helper 실행
        execl("./ai_helper_chat_repl_shm_posix", "ai_helper_chat_repl_shm_posix", (char *)NULL);
        perror("execl");
        _exit(127);
    } else if (pid < 0) {
        perror("fork");
        return 1;
    }
    
    printf("✓ AI helper process started (PID: %d)\n", pid);
    printf("Waiting for AI helper to initialize...\n");
    sleep(2);
    
    // 테스트 질문 보내기
    printf("\n--- Test 1: Sending 'hello' ---\n");
    strncpy(shm->prompt, "hello", sizeof(shm->prompt)-1);
    shm->prompt[sizeof(shm->prompt)-1] = '\0';
    
    printf("Signaling AI helper...\n");
    sem_post(sem_to_child);
    
    printf("Waiting for response (timeout: 30s)...\n");
    
    // 타임아웃 있는 대기 (macOS 폴링 방식)
    int timeout_sec = 30;
    int got_response = 0;
    
    for (int i = 0; i < timeout_sec * 10; i++) {
        if (sem_trywait(sem_to_parent) == 0) {
            got_response = 1;
            break;
        }
        usleep(100000);  // 0.1초
        
        if (i % 10 == 0 && i > 0) {
            printf(".");
            fflush(stdout);
        }
    }
    
    if (got_response) {
        printf("\n✓ Response received:\n");
        printf("---\n%s\n---\n", shm->response);
    } else {
        printf("\n✗ TIMEOUT - No response from AI helper\n");
        printf("Checking if process is still running...\n");
        
        int status;
        pid_t result = waitpid(pid, &status, WNOHANG);
        
        if (result == 0) {
            printf("AI helper is still running but not responding\n");
            printf("Check:\n");
            printf("  1. Is ollama running? (ps aux | grep ollama)\n");
            printf("  2. Is gemma3:1b installed? (ollama list)\n");
            printf("  3. Check ai_helper logs/errors\n");
        } else if (result > 0) {
            printf("AI helper process exited with status: %d\n", WEXITSTATUS(status));
        }
    }
    
    // 종료 신호 보내기
    printf("\nSending exit signal...\n");
    strncpy(shm->prompt, "exit", sizeof(shm->prompt));
    sem_post(sem_to_child);
    
    waitpid(pid, NULL, 0);
    
    // 정리
    munmap(shm, sizeof(ShmBuf));
    close(shm_fd);
    sem_close(sem_to_child);
    sem_close(sem_to_parent);
    sem_unlink(SEM_TO_AI);
    sem_unlink(SEM_TO_PARENT);
    shm_unlink(SHM_NAME);
    
    printf("✓ Test completed\n");
    return 0;
}
