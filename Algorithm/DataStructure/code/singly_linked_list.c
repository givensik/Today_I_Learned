#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

//node 구조
typedef struct node{
    int elem; //데이터(일단은 자료형을 정수로 저장)
    struct node* next; //다음 노드 주소
}node;

//singlyLinkedList 구조
typedef struct SLList{
    struct node* header; //헤더 노드
}SLList;

//싱글리스트 초기화
void init(SLList* list){
    list->header = (node*)malloc(sizeof(node));
    list->header->next = NULL;

}

//리스트 ADT 구현, 일반 메쏘드
//size()
int size(SLList* list){
    printf("size\n");
    int s = 0;
    node* p = list->header->next;//헤더 다음 주소부터
    while(true){
        if(p->next == NULL){//다음 노드가 없으면 반복문 종료, header의 next가 0이라면 0으로 나올거임
            s++;
            
            break;
        }
        s++;
        p = p->next;
        printf("size = %d\n",s);
    }

    return s;
}

//isEmpty()
bool isEmpty(SLList* list){
    int i;
    if(list->header->next == NULL){
        return true;
    }else{
        return false;
    }
    return false;
}
//elements() -> interator elements() 인데 iterator가 무슨 의미인지 모르겠어서 패스

void print(SLList* list){
    node *p = list->header->next;
    while(true){
        if(p->next == NULL){
            printf(" %d\n",p->elem);
            break;
        }
        printf(" %d",p->elem);
        p = p->next;
    }
}

//접근 메쏘드 -> 원소 반환이라고 되어있는데, 그냥 노드의 주소를 반환하려함
node* get(SLList* list, int r){
    node *p = list->header;
    int i;
    for(i=0;i<r;i++){
        p = p->next;
    }
    return p;
}
//갱신 메쏘드
void set(SLList* list, int r, int e){
    
}

void add(SLList* list,int r, int e){
    
    node *new_node = (node*)malloc(sizeof(node));
    new_node ->elem = e;
    node *p = get(list,r-1); //r-1 순위의 node를 가져옴

    new_node->next = p->next;
    p->next = new_node;
    //print(list);
}




int main(){

    SLList *a = (SLList*)malloc(sizeof(SLList));
    
    init(a);//초기화
    //isEmpty확인
    add(a,1,1);
    add(a,2,2);
    print(a);
    printf("size = %d\n", size(a));
    
    if(isEmpty(a)){
        printf("empty!\n");
    }else{
        printf("not empty\n");
    }
    



    return 0;
}