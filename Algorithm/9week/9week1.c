#include <stdio.h>
#include <stdlib.h>

typedef struct node{
    struct node *lChild,*rChild; 
    struct node *parent;
    int key;
}node;

typedef struct tree{
    struct node* root;
}tree;

int isEmpty(struct tree* a){

    if(a->root == NULL){
        return 1;
    }
    return 0;
}

void makeExternal(node* a){
    a->lChild = (node*)malloc(sizeof(node));
    a->lChild->parent = a;
    a->lChild->key = 0;
    a->rChild = (node*)malloc(sizeof(node));
    a->rChild->key = 0;
}

void inserTree(tree* a, int key){
    node *next = a->root;

    while(1){
        //오른쪽으로
        if(next->key <= key){
            if(next->rChild->key == 0){
                next->rChild->key = key;
                makeExternal(next->rChild);
                break;
            }else{
                next = next->rChild;
            }
        }else{
            if(next->lChild->key == 0){
                next->lChild->key = key;
                makeExternal(next->lChild);
                break;
            }else{
                next = next ->lChild;
            }
        }
    }
}

int main(){

    char instrcution;
    int key;
    tree* a = (tree*)malloc(sizeof(tree));

    while(1){
        scanf("%c",&instrcution);
        if(instrcution == 'q'){
            break;
        }else if(instrcution == 'i'){
            //키를 트리에 삽입
            scanf("%d",&key);
            
            
            if(isEmpty(a)){
                node *new_node = (node*)malloc(sizeof(node));
                new_node->key = key;
                a->root = new_node;
                makeExternal(new_node);
            }else{
                insertTree(a,key);
            }
        }else if(instrcution == 'd'){
            //삭제
        }else if(instrcution == 's'){
            //출력
        }else if(instrcution == 'p'){
            //전위 순회로 인쇄
        }

        

    }

    free(a);

    return 0;
}