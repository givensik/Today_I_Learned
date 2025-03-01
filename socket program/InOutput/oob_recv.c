/*
    gcc oob_recv.c -o recv
    ./recv 9090
*/
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#define BUF_SIZE 30
void error_handling(char *message);
void urg_handler(int signo);

int acpt_sock;
int recv_sock;

int main(int argc, char **argv){
    struct sockaddr_in recv_adr, serv_adr;
    int str_len, state;
    socklen_t serv_adr_sz;
    struct sigaction act;
    
    char buf[BUF_SIZE];
    
    if(argc < 2){
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }
    
    act.sa_handler = urg_handler; 
    sigemptyset(&act.sa_mask); // sa_mask, sa_flag를 모두 0 으로 초기화 해주어야 함
    act.sa_flags = 0;

    acpt_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (acpt_sock == -1) {
        error_handling("socket() error");
    }
    // recv_adr 설정
    memset(&recv_adr, 0, sizeof(recv_adr));
    recv_adr.sin_family = AF_INET;
    recv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    recv_adr.sin_port = htons(atoi(argv[1]));

    // bind
    if(bind(acpt_sock, (struct sockaddr *)&recv_adr, sizeof(recv_adr)) == -1){
        error_handling("bind() error");
    }
    // socket listening
    listen(acpt_sock, 5);
   
    serv_adr_sz = sizeof(serv_adr);
    // recv_socket으로 읽기
    recv_sock = accept(acpt_sock, (struct sockaddr *)&serv_adr, &serv_adr_sz);

    fcntl(recv_sock, F_SETOWN, getpid()); // recv_sock이 가리키는 소켓에 의해 발생하는 SIGURG 시그널을 처리하는 프로세스를 getpid 함수가 반환하는 ID의 프로세스로 변경하겠다.
    // printf("pid = %d\n", getpid());
    state=sigaction(SIGURG, &act, 0); // MSB_OOB의 긴급 메시지 수신시 운영체제 -> SIGURG 사용이 됨 -> 프로세스가 등록한 시그널 핸들러가 호출됨

    while((str_len=recv(recv_sock, buf, sizeof(buf), 0)) != 0 ){
        if(str_len == -1){
            continue;
        }
        buf[str_len] = 0;
        puts(buf);
    }
    close(recv_sock);
    close(acpt_sock);

    return 0;
}


void error_handling(char *message){
    fputs(message, stderr);
    fputs("\n", stderr);
    perror(message);
    exit(1);
}

void urg_handler(int signo){
    int str_len;
    char buf[BUF_SIZE];
    str_len = recv(recv_sock, buf, sizeof(buf)-1, MSG_OOB);
    buf[str_len] = 0;
    printf("Urgent message: %s\n", buf);
}
