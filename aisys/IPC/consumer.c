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
    int fd = shm_open(SHM_NAME, O_RDWR, 0666);
    char *ptr = mmap(0, 1024, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    sem_t *sem = sem_open(SEM_NAME, 0);
    while(1){
        sem_wait(sem); // producer가 쓸 때까지 대기
        if(strcmp(ptr, "exit\n") == 0){
            break;
        }
        printf("Received: %s", ptr);
    }
    munmap(ptr, 1024);
    close(fd);

    return 0;
}