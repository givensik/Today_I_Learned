/*
1. socket() : 소켓 생성
2. bind() : 소켓 주소할당
3. listen() : 연결요청 대기상태
4. accept() : 연결허용
5. read() / write() : 데이터 송수신
6. close() : 연결종료
*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>

int main(int argc, char **argv){

    if(argc != 2){
        printf("Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    int sock;
    // bind(int sockfd, struct sockaddr *myaddr, socklent_t addrlen);
    
    sock = socket(PF_INET, SOCK_STREAM, 0); // TCP socket 생성
    
    struct sockaddr_in serv_addr; // <arpa/inet.h>
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY; // 서버니까 INADDR_ANY
    serv_addr.sin_port = htons(atoi(argv[1])); // atoi -> <stdlib.h>

    // bind()
    bind(sock, (struct sockaddr *)&serv_addr, sizeof(sock));

    listen(sock, 5); // listen()
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    //accept() 결과로 연결이 된 클라이언트의 소켓을 반환
    int clnt_sock = accept(sock, (struct sockaddr *)&clnt_addr, clnt_addr_size);
    
    while(1){

        break;
    }



    return 0;
}