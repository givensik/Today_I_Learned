#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

#define BUF_SIZE 30
void error_handling(char *message);

int main(int argc, char* argv[]){
    
    int sock;
    char msg1[] = "Hi!";
    char msg2[] = "I'm another UDP host";
    char msg3[] = "Nice to meet you";

    struct sockaddr_in your_adr;

    if(argc != 3){
        printf("Usage: : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0); // UDP는 diagram이기 때문에 SOCK_DGRAM을 사용
    if(sock == -1) 
        error_handling("UDP socket creation failed");
    
    memset(&your_adr, 0, sizeof(your_adr));
    your_adr.sin_family = AF_INET;
    your_adr.sin_addr.s_addr = inet_addr(argv[1]);
    your_adr.sin_port = htons(atoi(argv[2]));

    printf("msg1\n");
    sendto(sock, msg1, sizeof(msg1), 0, (struct sockaddr *)&your_adr, sizeof(your_adr));
    printf("msg2\n");
    sendto(sock, msg2, sizeof(msg1), 0, (struct sockaddr *)&your_adr, sizeof(your_adr));
    printf("msg3\n");
    sendto(sock, msg3, sizeof(msg1), 0, (struct sockaddr *)&your_adr, sizeof(your_adr));
    
    close(sock);
    
    return 0;
}

void error_handling(char *message){
    fputs(message, stderr);
    fputs("\n", stderr);
    exit(1);
}