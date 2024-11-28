/*
    gcc signal.c -o signal
    ./signal
*/
#include <stdio.h>
#include <unistd.h>
#include <signal.h>

void timeout(int sig){
    if(sig==SIGALRM) {
        puts("Time out!");
    }
    alarm(2);
}

void keycontrol(int sig){
    if(sig==SIGINT){
        puts("Ctrl+C pressed");
    }
}

int main(int argc, char **argv) {
    int i;
    signal(SIGALRM, timeout); // alarm -> 시간이 지나면 SIGALRM 시그널을 보냄 -> 그러면 timeout이 실행
    signal(SIGINT, keycontrol); // Ctrl+C -> SIGINT 시그널 => keycontrol 실행
    alarm(2);

    for(i=0; i<3; i++){
        puts("wait...");
        sleep(100);
    }

    return 0;
}
