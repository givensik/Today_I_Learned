#include <stdio.h>
#include <ctype.h>
#include <string.h>

int calculate(char *message);


int main(){

    char *message = "1+3";
    char str[10];
    printf("%d\n", calculate(message));

    sprintf(str, "%d", calculate(message));

    printf("%s\n", str);
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