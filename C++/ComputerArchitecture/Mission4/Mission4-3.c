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
void instruction_fetch1();
void instruction_fetch2();

// 명령어 실행함수
void execution1();
void ADD1();
void Substract1();
void Store1();
void Load1();
void branch_always1();
void branch_if_01();
void branch_if_greater_than_01();
void INPUT1();
void OUTPUT1();

// 명령어 실행함수
void execution2();
void ADD2();
void Substract2();
void Store2();
void Load2();
void branch_always2();
void branch_if_02();
void branch_if_greater_than_02();
void INPUT2();
void OUTPUT2();

struct CPU{
    int MAR;
    int MBR;
    int IR;
}CPU1, CPU2;

int Memory[100] ={0};
int AC =0;
int PC=0;
int clock = 0;
int byte = 0;


int main(int argc, char* argv[]){
    FILE *code = fopen(argv[1],"r");
    char *cline; // 파일에서 한 줄씩 가져온 문자열이 들어가는 자리
    char line[100];
    int i=0;
    
    
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
    // 메모리에 저장
    while(!feof(code)){
        cline = fgets(line,100,code); //길이 100으로 한줄 가져옴
        Memory[i] = atoi(cline); //문자로 되어있는 cline을 정수로 바꿔서 메모리에 저장
        i++;
        byte++;
    }
    // printf("시작\n");
    while(1){
        instruction_fetch1();
        instruction_fetch2();
        printf("instruction_fetch : IR1 = %d, IR2 = %d\n",CPU1.IR,CPU2.IR);
        execution1();
        printf("CPU1 수행 : PC = %d, AC = %d\n",PC, AC);
        execution2();
        printf("CPU2 수행 : PC = %d, AC = %d\n",PC, AC);
    }
    

    fclose(code);
 
    return 0;
}
//instruction_fetch1 함수 PC에 있는 값을 CPU1의 IR에 가져옴
void instruction_fetch1(){
    CPU1.MAR = PC;
    CPU1.MBR = Memory[CPU1.MAR];
    PC = PC + 1;
    CPU1.IR = CPU1.MBR;
    clock++;
}

//instruction_fetch2 함수 PC에 있는 값을 CPU2의 IR에 가져옴
void instruction_fetch2(){
    CPU2.MAR = PC;
    CPU2.MBR = Memory[CPU2.MAR];
    PC = PC + 1;
    CPU2.IR = CPU2.MBR;
    clock++;
}

//execution1 함수 CPU1의 IR에 있는 명령어를 읽고 명령어를 수행한다.
void execution1(){
    int opcode = CPU1.IR/100;

    if(CPU1.IR == 901){
        INPUT1();
    }else if(CPU1.IR == 902){
        OUTPUT1();
    }

    switch (opcode)
    {
    case 0:
        printf("byte = %d\n",byte);
        printf("clock = %d\n",clock);
        exit(0);
        break;
    case 1: 
        ADD1();
        break;
    case 2:
        Substract1();
        break;
    case 3:
        Store1();
        break;
    case 5:
        Load1();
        break; 
    case 6:
        branch_always1();
        break;   
    case 7:
        branch_if_01();
        break;  
    case 8:
        branch_if_greater_than_01();
        break;
    default:
        break;
    }
}
//execution2 함수 CPU2의 IR에 있는 명령어를 읽고 명령어를 수행한다.
void execution2(){
    int opcode = CPU2.IR/100;

    if(CPU2.IR == 901){
        INPUT2();
    }else if(CPU2.IR == 902){
        OUTPUT2();
    }

    switch (opcode)
    {
    case 0:
        printf("byte = %d\n",byte);
        printf("clock = %d\n",clock);
        exit(0);
        break;
    case 1: 
        ADD2();
        break;
    case 2:
        Substract2();
        break;
    case 3:
        Store2();
        break;
    case 5:
        Load2();
        break; 
    case 6:
        branch_always2();
        break;   
    case 7:
        branch_if_02();
        break;  
    case 8:
        branch_if_greater_than_02();
        break;
    default:
        break;
    }
}

//ADD 함수, AC + IR의 address에 해당하는 값
void ADD1(){
    CPU1.MAR = CPU1.IR%100;
    CPU1.MBR = Memory[CPU1.MAR];
    AC = AC + CPU1.MBR;
}
//Substract 함수, AC - IR의 address에 해당하는 값
void Substract1(){
    CPU1.MAR = CPU1.IR%100;
    CPU1.MBR = Memory[CPU1.MAR];
    AC = AC - CPU1.MBR;
}
//Store 함수, AC에 있는 값을 Memeory에 저장
void Store1(){
    CPU1.MAR = CPU1.IR%100;
    CPU1.MBR = AC;
    Memory[CPU1.MAR] = CPU1.MBR;
}
//Load 함수
void Load1(){
    CPU1.MAR = CPU1.IR%100;
    CPU1.MBR = Memory[CPU1.MAR];
    AC = CPU1.MBR;
}
//branch_always
void branch_always1(){
    PC = CPU1.IR%100;
}
//branch_if_0
void branch_if_01(){
    if(AC == 0){
        PC = CPU1.IR%100;
    }
}
//branch if AC >= 0
void branch_if_greater_than_01(){
    if(AC >= 0){
        PC = CPU1.IR%100;
    }
}
//INPUT 함수
void INPUT1(){
    printf("input : ");
    scanf("%d",&AC);
    getchar();
}

//OUTPUT 함수
void OUTPUT1(){
    printf("%d\n",AC);
}


//ADD 함수, AC + IR의 address에 해당하는 값
void ADD2(){
    CPU2.MAR = CPU2.IR%100;
    CPU2.MBR = Memory[CPU2.MAR];
    AC = AC + CPU2.MBR;
}
//Substract 함수, AC - IR의 address에 해당하는 값
void Substract2(){
    CPU2.MAR = CPU2.IR%100;
    CPU2.MBR = Memory[CPU2.MAR];
    AC = AC - CPU2.MBR;
}
//Store 함수, AC에 있는 값을 Memeory에 저장
void Store2(){
    CPU2.MAR = CPU2.IR%100;
    CPU2.MBR = AC;
    Memory[CPU2.MAR] = CPU2.MBR;
}
//Load 함수
void Load2(){
    CPU2.MAR = CPU2.IR%100;
    CPU2.MBR = Memory[CPU2.MAR];
    AC = CPU2.MBR;
}
//branch_always
void branch_always2(){
    PC = CPU2.IR%100;
}
//branch_if_0
void branch_if_02(){
    if(AC == 0){
        PC = CPU2.IR%100;
    }
}
//branch if AC >= 0
void branch_if_greater_than_02(){
    if(AC >= 0){
        PC = CPU2.IR%100;
    }
}
//INPUT 함수
void INPUT2(){
    printf("input : ");
    scanf("%d",&AC);
    getchar();
}

//OUTPUT 함수
void OUTPUT2(){
    printf("%d\n",AC);
}