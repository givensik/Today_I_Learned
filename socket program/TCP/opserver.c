/*
    계산기 프로그램 서버
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <ctype.h>

#define BUF_SIZE 1024
int calculate(char *message);
void error_handling(char *message);

int main(int argc, char **argv){

    int serv_sock, clnt_sock;
    char message[BUF_SIZE];
    int str_len, i;
    char result_message[BUF_SIZE];
    int result;

    struct sockaddr_in serv_addr, clnt_addr;
    socklen_t clnt_sz;
    if(argc != 2){
        printf("Usage: %s <PORT>\n", argv[0]);
        exit(1);
    }

    // 서버 소켓 생성
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if(serv_sock == -1){
        error_handling("socket error");
    }

    // server socket 주소 설정
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(atoi(argv[1]));
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    // ip 설정(bind)
    if(bind(serv_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) == -1){
        error_handling("bind error");
    }

    // listen
    if(listen(serv_sock, 5) == -1){
        error_handling("listen error");
    }

    for(i=0; i<5; i++){
        clnt_sock = accept(serv_sock, (struct sockaddr *)&clnt_addr, &clnt_sz);
        if(clnt_sock == -1){
            error_handling("accept error");
        }else{
            printf("Connect client : %d\n", i+1);
        }

        while ((str_len =read(clnt_sock, message, BUF_SIZE)) != 0)
        {   
            result = calculate(message);
            
            sprintf(result_message, "%d", calculate(message));
            write(clnt_sock, result_message, str_len);
        }


    }
    
    return 0;
}


int calculate(char *message){
    int result = 0;
    int op1 =0 , op2 = 0;
    char current_op = '+';
    int i;
    for(i = 0 ; i< strlen(message) ; i++){
        // 정수이면
        if(isdigit(message[i])){
            op1 = op1 * 10 + (message[i] - '0');
            // printf("op1 : %d\n", op1);
        }

        // 연산자이거나 문자열의 끝인 경우
        if(!isdigit(message[i]) && !isspace(message[i]) || i == strlen(message)-1 ){
            // printf("operator\n");
            if (message[i] == '+') {
                current_op = '+';
            } else if (message[i] == '-') {
                current_op = '-';
            } else if (message[i] == '-') {
                current_op = '*';
            } else if (current_op == '/') {
                current_op = '/';
            }

            if (current_op == '+') {
                // printf("+\n");
                result += op1;
                // printf("result = %d\n", result);
            } else if (current_op == '-') {
                // printf("-\n");
                result -= op1;
            } else if (current_op == '*') {
                result *= op1; // 이전 결과에 곱하기
            } else if (current_op == '/') {
                result /= op1; // 이전 결과에 나누기
            }
            current_op = message[i];

            op1 = 0;
        }
        

    }
    

    return result;
}


void error_handling(char *message){
    fputs(message, stderr);
    fputs("\n", stderr);
    exit(1);
}