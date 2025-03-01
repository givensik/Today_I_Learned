/*
    gcc echo_selectserv.c -o selserv
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>

#define BUF_SIZE 100
void error_handling(char *buf);

int main(int argc, char **argv){
    int serv_sock, clnt_sock;
    struct sockaddr_in serv_adr, clnt_adr;
    struct timeval timeout;
    fd_set reads, cpy_reads;

    socklen_t adr_sz;
    int fd_max, str_len, fd_num, i;
    char buf[BUF_SIZE];

    if(argc != 2){
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

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

    FD_ZERO(&reads);
    FD_SET(serv_sock, &reads); // serv_sock을 관심 대상에 추가
    fd_max = serv_sock; // select에 fd의 수를 넣기 위해서 사용

    while(1){
        cpy_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;

        if((fd_num = select(fd_max+1, &cpy_reads, 0, 0, &timeout)) == -1){
            break;
        }
        if(fd_num == 0){
            continue;
        }

        for(i=0; i<fd_max+1; i++){ // 0 ~ fd_max 까지 파일 디스크립터에서
            if(FD_ISSET(i, &cpy_reads)){ // cpy_reads에 들어있는 파일디스크립터가 있으면
                if(i==serv_sock){ // serv_sock 디스크립터가 바뀐거면 요청이 온거임
                    adr_sz = sizeof(clnt_adr);
                    clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
                    FD_SET(clnt_sock, &reads); // select에서 추적할 디스크립터로 clnt_sock을 추가
                    if(fd_max < clnt_sock){
                        fd_max = clnt_sock;
                    }
                    printf("connected client %d\n", clnt_sock);
                }else{ // serv_sock은 아니지만 cpy_reads에 있는 fds -> clnt_sock이므로 클라이언트 소켓임
                    str_len = read(i, buf, BUF_SIZE);
                    if(str_len == 0){ // 아무런 값이 안오면 연결 해제
                        FD_CLR(i, &reads);
                        close(i);
                        printf("closed client %d\n", i);
                    }else{ // 값이 왔다면 그대로 Echo 해주기
                        write(i, buf, str_len); // echo
                    }
                }
            }
        }

    }

    close(serv_sock);

    return 0;
}

void error_handling(char *message){
    fputs(message, stderr);
    fputs("\n", stderr);
    exit(1);
}