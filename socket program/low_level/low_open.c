#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>

void error_handling(char *message);

int main(){
    int fd;
    char buf[] = "Lets go\n";

    fd = open("data.txt", O_CREAT | O_WRONLY | O_TRUNC );
    if(fd == -1)
        error_handling("open() error");
    printf("file descriptor : %d\n", fd);

    if(write(fd, buf, sizeof(buf)) == -1)
        error_handling("write() error");
    close(fd);

    return 0;
}

void error_handling(char *message){
    fputs(message, stderr);
    fputs("\n", stderr);
    exit(1);
}