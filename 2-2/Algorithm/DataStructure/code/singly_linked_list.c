#include <stdio.h>
#include <stdbool.h>

//node 구조
typedef struct node{
    int elem; //데이터
    struct node* next; //다음 노드 주소
}node;

//singlyLinkedList 구조
typedef struct SLList{
    struct node* header; //헤더 노드
}SLList;

void init(SLList* list){
    list->header->next = 0;
}

//리스트 구현 : size(), isEmmpty(), 

//조회


bool isEmpty(SLList* list){
    int i;
    if(list->header->next == 0){
        return false;
    }else{
        return true;
    }
    return false;
}


int main(){

    SLList *a = (SLList*)
    return 0;
}