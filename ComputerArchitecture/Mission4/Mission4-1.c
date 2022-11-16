#include <stdio.h>
#include <stdlib.h>
// SJU-CPU
/*
000 end
1xx add
2xx substract
3xx store
5xx load
6xx branch always
7xx branch if 0
8xx branch if >= 0
901 input
902 output
*/
void execution();
void ADD();
void Substract();
void Store();
void Load();
void branch_always();
void branch_if_0();
void branch_if_greater_than_0();
void INPUT();
void OUTPUT();
// 명령어 실행함수

int AC=0;
int MAR=0;
int MBR=0;
int IR=0;
int Memory[100];
int PC=0;


int main(){
    Memory[99]=1;
    int tmp;
    
    while(1){
        printf("000 end\n1xx add\n2xx substract\n3xx store\n5xx load\n6xx  branch always\n7xx branch if 0\n8xx branch if >= 0\n901 input\n902 output\ninput instruction :");
        scanf("%d",&tmp); // 이거랑 PC +1 한거랑 같음
        IR = tmp;
        execution();
        printf("AC = %d\n",AC);
        printf("IR = %d\n",IR);
        printf("PC = %d\n",PC);
        printf("Memory\n");
    }
    
    return 0;
}

void execution(){
    int opcode = IR/100;

    switch (opcode)
    {
    case 0:
        exit(0);
        break;
    case 1: 
        ADD();
        break;
    case 2:
        Substract();
        break;
    case 3:
        Store();
        break;
    case 4:
        Load();
        break;    
    default:

        break;
    }
}
//ADD 함수, AC + IR의 address에 해당하는 값
void ADD(){
    MAR = IR%100;
    MBR = Memory[MAR];
    AC = AC + MBR;
}
//Substract 함수, AC - IR의 address에 해당하는 값
void Substract(){
    MAR = IR%100;
    MBR = Memory[MAR];
    AC = AC - MBR;
}
//Store 함수, AC에 있는 값을 Memeory에 저장
void Store(){
    MAR = IR%100;
    MBR = AC;
    Memory[MAR] = MBR;
}
//Load 함수
void Load(){
    MAR = IR%100;
    MBR = Memory[MAR];
    AC = MBR;
}
//branch_always
void branch_always(){
    PC = IR%100;
}
//branch_if_0
void branch_if_0(){
    if(AC == 0){
        PC = IR%100;
    }
}
//branch if AC >= 0
void branch_if_greater_than_0(){
    if(AC >= 0){
        PC = IR%100;
    }
}
//INPUT 함수
void INPUT(){
    scanf("%d",&AC);
}

//OUTPUT 함수
void OUTPUT(){
    printf("%d\n",AC);
}