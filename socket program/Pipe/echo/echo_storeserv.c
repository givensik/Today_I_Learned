/*
    gcc echo_storeserv.c -o serv
    ./serv 9180
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 100
void error_handling(char *message);
void read_childproc(int sig);

int main(int argc, char **argv){
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    int fds[2];

    pid_t pid;
    struct sigaction act;
    socklen_t adr_sz;
    int str_len, state;
    char buf[BUF_SIZE];
    if(argc!= 2){
        printf("Usage: %s [port]\n", argv[0]);
        exit(1);
    } 

    // 그 뭐냐 이거 무슨 설정이었는데
    act.sa_handler =read_childproc;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    state=sigaction(SIGCHLD, &act, 0);
    
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1){
        error_handling("bind() error");
    }
    if(listen(serv_sock, 5) == -1){
        error_handling("listen() error");
    }

    pipe(fds);
    pid = fork();

    // 프로세스를 생성해서 파이프fds로 보낸 값을 받아서 파일에 저장하는 코드
    if(pid == 0){
        FILE * fp = fopen("echomsg.txt", "wt");
        char msgbuf[BUF_SIZE];
        int i, len;
        if(fp == NULL){
            error_handling("file error");
        }
        // 수정된 부분: 무한 루프를 추가하여 파이프에서 데이터를 계속 읽음
        while((len = read(fds[0], msgbuf, BUF_SIZE)) > 0){
            fwrite(msgbuf, 1, len, fp);
            fflush(fp); // 이걸 해줘야 되는데 개빡치네;;;ss
            // puts(msgbuf);
        }
        // for(i=0; i<10; i++){
        //     len = read(fds[0], msgbuf, BUF_SIZE);
        //     fwrite((void *)msgbuf, 1, len, fp);
        // }
        fclose(fp);
        return 0;
    }
    
    while(1){
        adr_sz = sizeof(clnt_adr);
        clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
        if(clnt_sock == -1){
            continue;
        }else{
            puts("new Client connected.. ");
        }
        
        pid = fork();
        if(pid==0){
            close(serv_sock);
            while((str_len=read(clnt_sock, buf, BUF_SIZE))!=0){
                write(clnt_sock, buf, str_len);
                write(fds[1], buf, str_len); // 파이프에 저장하는 코드
            }

            close(clnt_sock);
            puts("client disconnected");
            return 0;
        }else{
            close(clnt_sock);
        }
    }
    close(serv_sock);
    return 0;
}



void read_childproc(int sig){
    pid_t pid;
    int status;
    pid = waitpid(-1, &status, WNOHANG);
    printf("removed proc id %d\n", pid);
}


void error_handling(char *message){
    fputs(message, stderr);
    fputs("\n", stderr);
    exit(1);
}
