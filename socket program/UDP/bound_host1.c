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
    char message[BUF_SIZE];
    struct sockaddr_in my_adr, your_adr;
    int str_len, i;
    socklen_t adr_sz;

    
    if(argc != 2){
        printf("Usage: : %s <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_DGRAM, 0); // UDP는 diagram이기 때문에 SOCK_DGRAM을 사용
    if(sock == -1) 
        error_handling("UDP socket creation failed");
    
    memset(&my_adr, 0, sizeof(my_adr));
    my_adr.sin_family = AF_INET;
    my_adr.sin_addr.s_addr = htonl(INADDR_ANY);
    my_adr.sin_port = htons(atoi(argv[1]));

    if(bind(sock, (struct sockaddr *)&my_adr, sizeof(my_adr)) == -1)
        error_handling("bind() error");
    
    for(i=0; i<3; i++){
        printf("get data... ");
        sleep(5);
        adr_sz = sizeof(your_adr);
        str_len = recvfrom(sock, message, BUF_SIZE, 0, (struct sockaddr *)&your_adr, &adr_sz);

        printf("Message %d : %s \n", i+1, message);
    }

    return 0;
}

void error_handling(char *message){
    fputs(message, stderr);
    fputs("\n", stderr);
    exit(1);
}