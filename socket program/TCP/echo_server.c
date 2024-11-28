#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ctype.h>


#define BUF_SIZE 1024

void error_handling(char *message);
int main(int argc, char **argv){
    
    int serv_sock, clnt_sock; // 서버, 클라이언트 소켓 변수
    char message[BUF_SIZE]; // 클라이언트에서 받아올 메시지를 담을 변수
    int str_len, i;
    int result;
    struct sockaddr_in serv_adr, clnt_adr; // address 넣을 구조체
    socklen_t clnt_adr_sz;
    
    if(argc != 2){
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1){
        error_handling("socket error");
    }

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr =htonl(INADDR_ANY);
    serv_adr.sin_port = htons(atoi(argv[1]));

    if(bind(serv_sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1){
        error_handling("bind error");
    }

    if(listen(serv_sock, 5) == -1){
        error_handling("listen error");
    }

    clnt_adr_sz = sizeof(clnt_adr);

    // 5번 반복하면서 연결요청 받고 message 돌려보내기
    for(i=0 ; i<5; i++){
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_adr, &clnt_adr_sz);
        if(clnt_sock == -1){
            error_handling("accept error");
        }else{
            printf("Connect client : %d\n", i+1);
        }
        
        while ((str_len =read(clnt_sock, message, BUF_SIZE)) != 0)
        {
            result = calculate(message);
        }

        close(clnt_sock);
    }

    return 0;
}


void error_handling(char *message){
    fputs(message, stderr);
    fputs("\n", stderr);
    exit(1);
}