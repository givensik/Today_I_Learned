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

// 명령어 인출 함수
void instruction_fetch();

// 명령어 실행함수
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


int AC=0;
int MAR=0;
int MBR=0;
int IR=0;
int Memory[100] ={0};
int PC=0;

int clock = 0;
int byte = 0;

int main(int argc, char* argv[]){
    FILE *code = fopen(argv[1],"r");
    char *cline; // 파일에서 한 줄씩 가져온 문자열이 들어가는 자리
    char line[100];
    int i=0;
    
    
    //메모리에 저장
    // 입력한 인자 수가 없을 때
    if(argc == 1){
        printf("input filename!\n");
        return 0;
    }
    // 파일을 열지 못했을때
    if(code == NULL){
        printf("Couldn't open file\n");
        return 0;
    }

    while(!feof(code)){
        cline = fgets(line,100,code); //길이 100으로 한줄 가져옴
        Memory[i] = atoi(cline); //문자로 되어있는 cline을 정수로 바꿔서 메모리에 저장
        i++;
        byte++;
    }
    while(1){
        instruction_fetch();
        execution();
    }
    

    fclose(code);
 
    return 0;
}
//instruction_fetch함수 PC에 있는 값을 IR에 가져옴
void instruction_fetch(){
    MAR = PC;
    MBR = Memory[MAR];
    PC = PC + 1;
    IR = MBR;
    clock++;
}

void execution(){
    int opcode = IR/100;

    if(IR == 901){
        INPUT();
    }else if(IR == 902){
        OUTPUT();
    }

    switch (opcode)
    {
    case 0:
        printf("byte = %d\n",byte);
        printf("clock = %d\n",clock);
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
    case 5:
        Load();
        break; 
    case 6:
        branch_always();
        break;   
    case 7:
        branch_if_0();
        break;  
    case 8:
        branch_if_greater_than_0();
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
    printf("input : ");
    scanf("%d",&AC);
}

//OUTPUT 함수
void OUTPUT(){
    printf("%d\n",AC);
}