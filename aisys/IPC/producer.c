#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <string.h>

#define SHM_NAME "/myshm"
#define SEM_NAME "/mysem"

int main(){
    int fd = shm_open(SHM_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(fd, 1024); // 세마포어 생성 및 열기

    char *ptr = mmap(0, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    sem_t *sem = sem_open(SEM_NAME, 0);
    char buf[256];

    while(1){
        printf("producer> ");
        fflush(stdout);
        if (!fgets(buf, sizeof(buf), stdin)) {
            break;
        }

        if(strncmp(buf, "exit\n", 5) == 0){ // exit 입력 시 종료
            break;
        }
        strcpy(ptr, buf);
        sem_post(sem); // consumer에게 알림
    }
    munmap(ptr, 1024);
    close(fd);

    return 0;
}