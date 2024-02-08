#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Graph{
    int **matrix;
    int vertexCount;
}Graph;

void initGraph(Graph *g, int max); // 그래프 초기화 함수



int main(){

    Graph *g;
    g =(Graph*)malloc(sizeof(Graph));
    initGraph(g, 6);

    char instruction;
    int node_number;
    while(1){
        scanf("%c",instruction);
        if(instruction =='a'){
            scanf("%d",&node_number);
            getchar();
        }else if(instruction == 'm'){

        }else if(instruction == 'q'){
            break;
        }
    }

    return 0;
}

//그래프 초기화 함수
void initGraph(Graph *g, int max){
    g->vertexCount = max;
    g->matrix = (int**)malloc(sizeof(int*)*(max+1));
    int i=0,j=0;
    for(i=0;i<max+1;i++){
        g->matrix[i] = (int *)malloc(sizeof(int)*(max+1));
        memset(g->matrix[i], 0, sizeof(int)*(max+1));
    }
}
