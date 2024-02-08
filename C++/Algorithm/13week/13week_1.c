#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 50
// 선언
typedef struct GraphType{
    int n;
    int adj_mat[MAX_VERTICES][MAX_VERTICES];
} GraphType;


// 그래프 초기화
void init(GraphType* g){
    int r,c;
    g->n=0;
    for(r=0;r<MAX_VERTICES;r++)
        for(c=0;c<MAX_VERTICES;c++)
           g->adj_mat[r][c]=0;
}


//정점 삽입
void insert_vertex(GraphType* g,int v){
    if (((g->n)+1)>MAX_VERTICES){
        fprintf(stderr,"overflow");
        return;
    }
    g->n++;
}

//간선 삽입
void insert_edge(GraphType* g,int start,int end){
    g->adj_mat[start][end]=1;
    g->adj_mat[end][start]=1;
}

// 인접 행렬 출력 함수
void print_adj_mat(GraphType* g){
    printf("print\n");
    for(int i = 0;i<g->n;i++){
        for(int j = 0 ; j<g->n;j++){
            printf("%d\n",g->adj_mat[i]);
        }
         
    }
}
int main()
{
    GraphType *g;
    g=(GraphType *)malloc(sizeof(GraphType));
    init(g);
    for(int i=0;i<4;i++){

    }
       
    
    free(g);
    return 0;
}
