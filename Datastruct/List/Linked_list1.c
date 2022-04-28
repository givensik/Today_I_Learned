#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    int data;
    struct node *next;
}Node;

//일단 입력받는거만 그냥 해보는거

int main(){
    
    
    //list 초기화
    Node *head=NULL;
    Node *tail=NULL;
    Node *cur=NULL;

    int readD;
    //입력
    Node * NewNode=NULL;
    while(1){
        printf("입력 : ");
        scanf("%d",&readD);
        if(readD < 1){
            break;
        }
        NewNode = (Node*)malloc(sizeof(Node));//새로운 노드 생성
        NewNode->data = readD;
        NewNode->next = NULL;
        if(head == NULL){//첫번째 노드일 때
            head = NewNode;
        }else{//그 이후
            tail->next = NewNode;
        }
        tail = NewNode;//가장 마지막에 들어온거니까
    }

    //출력
    if(head == NULL){//없는거
        printf("없음\n");
    }else{
        cur = head;
        while(cur->next != NULL){//next가 있다면 반복
            printf("%d ",cur->data);
            cur = cur->next;
        }
        printf("%d\n",cur->data);
    }

    return 0;
}