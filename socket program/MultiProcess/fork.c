/*
    gcc fork.c -o fork
*/
#include <stdio.h>
#include <unistd.h>

int gval = 10;
int main(int argc, char **argv){
    pid_t pid;
    int lval = 20;
    gval++, lval+=5;

    pid = fork();
    // lval = 25, gval = 11로 프로세스 생성됨
    if(pid ==0){// if Child Process
        gval+=2, lval+=2;
    }else{
        gval-=2, lval-=2;
    }

    if(pid==0){
        printf("Child Process : [%d %d]\n", gval, lval);
    }else{
        printf("Parent Process : [%d %d]\n",gval, lval);
    }

    return 0;
}