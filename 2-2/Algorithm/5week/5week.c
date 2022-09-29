#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    int elem;
    struct node *next;
}node;

typedef struct SLList{
    struct node *head;
    int num;
}SLList;

//싱글리스트 초기화
void init(SLList* list){
    list->head = (node*)malloc(sizeof(node));
    list->head->next = NULL;

}

struct node* get(SLList* list, int r){
    int i;
    node *p = list->head;
    for(i=0;i<r;i++){
        p = p->next;        
    }
    return p;

}

void add(SLList* list, int r, int e){
    node *p = get(list,r);
    node *new_node = (node*)malloc(sizeof(node));
    new_node ->elem = e;
    new_node ->next = NULL;
    p->next = new_node;
    
}

void print(SLList* list){
    node *p = list->head;
    while(p->next != NULL){
        p = p->next;
        printf(" %d",p->elem);
        
    }
    printf("\n");
}

//합병 정렬 구현

void partition(SLList* L1, SLList* L2, SLList* L){
    if(L->num > 2){
        
    }
}



int main(){
    
    SLList *a = (SLList*)malloc(sizeof(SLList));
    init(a);
    
    int N;
    int new;
    int i;
    scanf("%d",&N);
    a->num =N;
    for(i = 0; i < N; i++){
        scanf("%d",&new);
        add(a,i,new);//i가 0일때 
    }  

    




    print(a);

    //메모리 해제
    node *p = a->head,*q;
    for(i=0;i<N;i++){
        q = p;
        p = p->next;
        free(q);
    }
    free(a);
    
    return 0;
}