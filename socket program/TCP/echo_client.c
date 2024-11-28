#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 1024

void error_handling(char *message);

int main(int argc, char **argv){

    int sock;
    char message[BUF_SIZE];
    int str_len;
    struct sockaddr_in serv_adr;

    if(argc != 3){
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        error_handling("socket() error");
    }

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr =inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if(connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1){
        error_handling("socket error");
    }else{
        puts("Connect........");
    }

    while(1){

        fputs("Input message(Q to quit):", stdout);
        fgets(message, BUF_SIZE, stdin);

        if(!strcmp(message, "q\n") || !strcmp(message, "Q\n")){
            break;
        }

        write(sock, message, strlen(message)); // 메시지 보내기
        str_len = read(sock, message, strlen(message)); // 메시지 받기
        message[str_len] = 0;
        printf("Message from server: %s\n", message);

        /*
            문제점 : 보내는 메시지의 길이가 긴 경우, 서버에서는 이를 몇 번으로 나누어서 보낼 수 있음, 근데 여기 client의 경우에는 read를 한 번만 진행하므로 , 서버에서 보낸 데이터가 다 도착하지 않았는데도 read 함수를 호출 할 수 있다.
            write 한 번 사용하고 read 한 번으로 다 받으려함
            해결책 : 20바이트를 받아야하면 20바이트를 수신할 때 까지 기다리면 됨 
        */
    }
    close(sock);

    return 0;
}

void error_handling(char *message){
    fputs(message, stderr);
    fputs("\n", stderr);
    exit(1);
}