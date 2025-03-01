/*
    gcc peek_recv.c -o recv
    ./recv 9190
*/
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char **argv){
    int acpt_sock, recv_sock;
    struct sockaddr_in acpt_adr, recv_adr;
    int str_len, state;
    socklen_t recv_adr_sz;
    char buf[BUF_SIZE];

    if(argc != 2) {
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
    memset(&acpt_adr, 0, sizeof(acpt_adr));
    acpt_adr.sin_family = AF_INET;
    acpt_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    acpt_adr.sin_port = htons(atoi(argv[1]));

    if(bind(acpt_sock, (struct sockaddr *)&acpt_adr, sizeof(acpt_adr)) == -1) {
        error_handling("bind() error");
    }

    listen(acpt_sock, 5);

    recv_adr_sz = sizeof(recv_adr);
    recv_sock = accept(acpt_sock, (struct sockaddr *)&recv_adr, &recv_adr_sz);

    while(1){
        str_len = recv(recv_sock, buf, sizeof(buf) -1, MSG_PEEK|MSG_DONTWAIT); 
        // MSG_PEEK 옵션을 주고 recv 함수를 호출하면 입력버퍼에 존재하는 데이터가 읽혀지더라도 입력버퍼에서 데이터가 지워지지 않음
        // MSG_DONTWAIT 옵션을 주면 데이터가 존재하지 않아도 블로킹 상태에 두지 않게 함
        if(str_len > 0){
            break;
        }
    }

    
    buf[str_len] =0;
    printf("Buffering %d bytes : %s \n", str_len, buf);

    str_len = recv(recv_sock, buf, sizeof(buf)-1, 0); // 이때는 MSG_PEEK|MSG_DONTWAIT 옵션이 설정되지 않았으므로 입력버퍼가 읽히는 순간 지워진다.
    buf[str_len] = 0;
    printf("Read again : %s \n", buf);
    close(acpt_sock);
    close(recv_sock);
    
    return 0;
}

void error_handling(char *message){
    fputs(message, stderr);
    fputs("\n", stderr);
    perror(message);
    exit(1);
}
