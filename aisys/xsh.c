#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <signal.h>
#include <fcntl.h>

#define MAX_BUFFER 1024

// 빌트인 명령어 구조체
typedef struct builtin_cmd{
    char *name;
    void (*func)(int argc, char **argv);
} builtin_cmd;

typedef struct Command {
    char *name;
    char **args;
    int arg_count;
    int input_fd;  // 입력 리다이렉션 파일 디스크립터
    int output_fd; // 출력 리다이렉션 파일 디스크립터
} Command;

Command* parse_command(char *input);
int EvaluateCommand(Command *cmd);

// 빌트인 명령어
void builtin_exit();
void builtin_cd(Command *cmd);
void builtin_pwd();

// 빌트인 명령어 배열
builtin_cmd builtins[] = {
    {"exit", builtin_exit},
    {"cd", builtin_cd},
    {"pwd", builtin_pwd},
    {NULL, NULL} // 끝을 알리는 NULL
};


int main() {
    char buffer[MAX_BUFFER];
    Command *cmd;
    __pid_t pid;

    int cmd_index;
    char cwd[1024];
    char *tmp;

    // SIGNALS을 받아도 무시, 근데 자식 프로세스한테는 SIGNAL이 감
    signal(SIGTTIN, SIG_IGN); // background process가 터미널 읽으려 할 때 무시
    signal(SIGTTOU, SIG_IGN); // background process가 터미널 쓰려 할 때 무시
    signal(SIGINT, SIG_IGN); // Ctrl-C 무시
    signal(SIGTSTP, SIG_IGN); // Ctrl-Z 무시


    // 입력 받기
    while(1){
        // 프롬프트 출력 (PID + 현재 디렉터리 마지막 경로명)
        if(getcwd(cwd, sizeof(cwd)) != NULL){
            // printf("현재 디렉터리: %s\n", cwd);
            tmp = cwd;
            // 마지막 '/' 이후 문자열만 추출
            char *last_slash = strrchr(cwd, '/');
            if(last_slash != NULL){
                tmp = last_slash;
            }
        } else {
            perror("getcwd error");
            tmp = "xsh";
        }

        printf("\033[1;36mxsh[%d] : %s> \033[0m", getpid(), tmp);

        // 1. Read
        fflush(stdout);
        // fgets에서 EOF 처리
        if(fgets(buffer, sizeof(buffer), stdin) == NULL) {
            printf("\nEOF, exit\n");
            break; // 루프 종료
        }
        // fgets(buffer, sizeof(buffer), stdin);

        cmd = parse_command(buffer);
        // 명령어 파싱 실패 시 처리
        if(cmd == NULL){
            fprintf(stderr, "Failed to parse command\n");
            continue;
        }

        // 2. Evaluation command
        if((cmd_index = EvaluateCommand(cmd)) != -1){ // built-in 명령어인 경우
            // 실행
            if(cmd->input_fd != -1){
                dup2(cmd->input_fd, STDIN_FILENO);
                close(cmd->input_fd);
            }
            if(cmd->output_fd != -1){
                dup2(cmd->output_fd, STDOUT_FILENO);
                close(cmd->output_fd);
            }

            builtins[cmd_index].func(cmd->arg_count, cmd->args);
            free(cmd->args);
            free(cmd);
            continue;
        } else {
            // 3. Execute
            pid = fork();
            if(pid < 0){
                perror("Fork failed");
                continue;
            } else if(pid == 0){
                // 자식 프로세스 -> 터미널 제어권 획득
                if(setpgid(0, 0) < 0){
                    perror("setpgid failed");
                    exit(1);
                }
                if(cmd->input_fd != -1) {
                    dup2(cmd->input_fd, STDIN_FILENO);
                    close(cmd->input_fd);
                }
                if(cmd->output_fd != -1) {
                    dup2(cmd->output_fd, STDOUT_FILENO);
                    close(cmd->output_fd);
                }
                // 자식 프로세스
                if(execvp(cmd->name, cmd->args) < 0){
                    perror("command not found");
                    exit(1);
                }
            } else {
                if(setpgid(pid, pid) < 0){
                    perror("setpgid failed");
                    continue;
                }
                
                // 부모 프로세스
                waitpid(pid, NULL, 0); // 자식 프로세스가 끝날 때까지 대기
            }
        }
    }
    
    
    return 0;
}

// 명령어 파싱 함수
Command* parse_command(char *input) {
    Command *cmd = malloc(sizeof(Command));
    cmd->args = malloc(sizeof(char*) * MAX_BUFFER);
    cmd->arg_count = 0;

    char *token = strtok(input, " \n"); // 토큰화
    if(token == NULL){
        free(cmd->args);
        free(cmd);
        return NULL;
    }
    cmd->name = token; // 첫 토큰은 명령어 이름에 저장
    while(token != NULL){ // 나머지 토큰들을 인자에 저장
        cmd->args[cmd->arg_count++] = token;
        token = strtok(NULL, " \n");
    }

    return cmd;
}

// built_in 명령어인지 확인하는 함수
int EvaluateCommand(Command *cmd){
    builtin_cmd *builtin = builtins;
    int i=0, j;

    // 리다이렉션 처리
    for (i = 0; i < cmd->arg_count; i++) {
        // 출력 리다이렉션
        if (strcmp(cmd->args[i], ">") == 0) {
            if (i + 1 < cmd->arg_count) {
                cmd->output_fd = open(cmd->args[i + 1], O_CREAT | O_WRONLY | O_TRUNC, 0644);
                if (cmd->output_fd < 0) {
                    perror("Output redirection failed");
                    return -1;
                }
                // 리다이렉션 토큰과 파일 이름 제거
                for (j = i; j + 2 < cmd->arg_count; j++) {
                    cmd->args[j] = cmd->args[j + 2];
                }
                cmd->args[j] = NULL; // 마지막 요소를 NULL로 설정
                cmd->arg_count -= 2;
                i--; // 인덱스 조정
            } else {
                fprintf(stderr, "Output redirection: missing file name\n");
                return -1;
            }
        }
        // 입력 리다이렉션
        else if (strcmp(cmd->args[i], "<") == 0) {
            if (i + 1 < cmd->arg_count) {
                cmd->input_fd = open(cmd->args[i + 1], O_RDONLY);
                if (cmd->input_fd < 0) {
                    perror("Input redirection failed");
                    return -1;
                }
                // 리다이렉션 토큰과 파일 이름 제거
                for (j = i; j + 2 < cmd->arg_count; j++) {
                    cmd->args[j] = cmd->args[j + 2];
                }
                cmd->args[j] = NULL; // 마지막 요소를 NULL로 설정
                cmd->arg_count -= 2;
                i--; // 인덱스 조정
            } else {
                fprintf(stderr, "Input redirection: missing file name\n");
                return -1;
            }
        }
    }

    // built-in 명령어인지 확인 -> 있으면 해당 인덱스 반환
    while(builtin->name != NULL){
        // printf("built-in name: %s\n", builtin->name);
        if(strcmp(cmd->name, builtin->name) == 0){
            // printf("cmd name : %s, built-in name: %s\n", cmd->name, builtin->name);
            return i;
        }
        i++;
        builtin++;
    }

    return -1;
}

// 빌트인 명령어
// exit
void builtin_exit() {
    printf("exit 실행\n");
    exit(0);
}
// cd
void builtin_cd(Command *cmd) {
    printf("cd 실행\n");
    // 인자가 없으면 오류 메시지 출력
    if (cmd->arg_count < 2) {
        fprintf(stderr, "cd: missing argument\n");
        return;
    }
    // chdir 호출
    if (chdir(cmd->args[1]) != 0) {
        perror("cd");
    }
}

//pwd
void builtin_pwd(){
    printf("pwd 실행\n");
    char cwd[1024];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("%s\n", cwd);
    } else {
        perror("pwd");
    }
}