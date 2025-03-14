#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <time.h>

#define PORT 9999
#define BUFFER_SIZE 1024

void log_connection(const char *ip, const char *data) {
    printf("로그를 기록 중\n");
    FILE *logfile = fopen("honeypot.log", "a");
    if (logfile) {
        time_t current_time = time(NULL);
        fprintf(logfile, "[%s] 연결됨: %s, 데이터: %s\n", ctime(&current_time), ip, data);
        fclose(logfile);
    }
    printf("로그를 기록 완료\n");
}

int main(int argc, char **argv){

    int serv_sockfd;

    // 소켓 생성
    if((serv_sockfd = socket(PF_INET, SOCK_STREAM, 0)) == 0){
        perror("socket error!");
        exit(EXIT_FAILURE);
    }


    int opt = 1;
    if (setsockopt(serv_sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("소켓 옵션 설정 실패");
        exit(EXIT_FAILURE);
    }

    // 소켓 주소 설정
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    // bind
    if(bind(serv_sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0){
        perror("bind error!");
        exit(EXIT_FAILURE);
    }


    // 연결 대기
    if(listen(serv_sockfd, 3)<0){
        perror("listen error!");
        exit(EXIT_FAILURE);
    }

    printf("허니팟이 %d 포트에서 실행 중입니다.\n", PORT);

    // 연결 -> 새로운 소켓 설정 -> 새로운 소켓이랑 그 길이가 필요함
    int new_sock; 
    int addr_len; 
    int pid;

    char* client_ip;
    while (1) {
        if ((new_sock = accept(serv_sockfd, (struct sockaddr *)&addr, (socklen_t*)&addr_len)) < 0) {
            perror("연결 수락 실패");
            exit(EXIT_FAILURE);
        }
        pid = fork();

        if(pid == 0){ // 새로운 연결 -> 자식 프로세스에서 처리
            client_ip= inet_ntoa(addr.sin_addr);
            printf("연결됨: %s:%d\n", inet_ntoa(addr.sin_addr), ntohs(addr.sin_port));
            break;
        }else{ // 부모프로세스는 계속 받기
            continue;
        }

    }
    char buffer[BUFFER_SIZE];

    int valread;
    while((valread= read(new_sock, buffer, BUFFER_SIZE)) != 0){

        buffer[valread] = '\0'; // 문자열 종료
        printf("받은 데이터: %s\n", buffer);
        log_connection(client_ip, buffer); // 로그 기록

        // 의심스러운 행동 탐지
        if (strstr(buffer, "malicious_command") != NULL) {
            send(new_sock, "위험한 명령어가 감지되었습니다!\n", 40, 0);
        } else {
            send(new_sock, "허니팟에 접속하셨습니다!\n", 30, 0);
        }  
    }

    close(new_sock);

    close(serv_sockfd);
    return 0;
}