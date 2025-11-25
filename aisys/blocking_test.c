// blockingTest.c
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>
#include <sys/wait.h>

const char* mode_str(int mode) {
    if (mode == 0)
        return "non-blocking";
    else
        return "blocking";
}

// mode: 0 = (       )  (non-blocking)
//       1 = (       )  (blocking)
int try_lock(int fd, int mode, const char *who) {
    struct flock fl;
    fl.l_type   = F_WRLCK;
    fl.l_whence = SEEK_SET;
    fl.l_start  = 0;
    fl.l_len    = 0;

    int ret;

    if (mode == 0) {
        // non-blocking 으로 잠금 시도
        ret = fcntl(fd, F_SETLK, &fl);
    } else {
        // blocking 으로 잠금 시도
        ret = fcntl(fd, F_SETLKW, &fl);
    }

    if (ret < 0)
        perror(who);
    else
        printf("%s: locked\n", who);

    return ret;
}

void run_test(int child_mode, int parent_mode) {
    printf("\n[Test child=%s, parent=%s]\n",
           mode_str(child_mode), mode_str(parent_mode));

    int fd = open("lockdemo.txt", O_RDWR | O_CREAT, 0666);
    if (fd < 0) { perror("open"); return; }

    write(fd, "X", 1);

    pid_t pid = fork();

    if (pid == 0) { // child
        printf("child(%s): trying lock...\n", mode_str(child_mode));
        try_lock(fd, child_mode, "child");
        sleep(10);
        close(fd);
        _exit(0);
    }

    sleep(1);
    printf("parent(%s): trying lock...\n", mode_str(parent_mode));
    try_lock(fd, parent_mode, "parent");

    close(fd);
    waitpid(pid, NULL, 0);
}

int main() {
    printf("=== fcntl() locking 4-case test ===\n");
    // 여러분이 blocking vs non-blocking 모드를 조합하여, 어떤 현상이 일어나는지 관찰해봅시다. 
    // 먼저 잡은 lock의 종류에 따라 이 후 lock 을 각각 모드에서 획득하려 할 때 어떻게 변하죠? 
    run_test(0, 0); // non-blocking vs non-blocking
    run_test(1, 0); // blocking vs non-blocking
    run_test(0, 1); // non-blocking vs blocking
    run_test(1, 1); // blocking vs blocking

    printf("\n=== done ===\n");
    return 0;
}
