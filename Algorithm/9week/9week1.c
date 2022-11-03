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

void makeExternal(node* a){
    a->lChild = (node*)malloc(sizeof(node));
    a->lChild->parent = a;
    a->lChild->key = 0;
    a->rChild = (node*)malloc(sizeof(node));
    a->rChild->key = 0;
}

void insertTree(tree* a, int key){
    node *next = a->root;
    //첫 원소
    if(a->root->key == 0){
        printf("root insert\n");
        a->root->key = key;
    }else{
        while(1){
        if(next->key <= key){//오른쪽으로
            printf("right ");
            if(next->rChild->key == 0){//비어있으면
                printf("insert\n");
                next->rChild->key = key;
                makeExternal(next->rChild);
                break;
            }else{
                next = next->rChild;
                continue;
            }
        }else if(key < next->key){// 왼쪽으로
            printf("left ");
            if(next->lChild->key == 0){//비어있으면
                printf("insert\n");
                next->lChild->key = key;
                makeExternal(next->lChild);
                break;
            }else{
                next = next->lChild;
                continue;
            }
        }
    }
    }
    
}
// 성공
int findKey(node* next, int key){
    int a,b;
    if(next->key == 0){
        return 0;
    }
    if(next->key == key){
        return key;
    }else{
        if(findKey(next->lChild, key)){
            return findKey(next->lChild, key);
        }
        if(findKey(next->rChild, key)){
            return findKey(next->rChild, key);
        }
    }
    return 0;
}

void print(node* next){
    if(next->key == 0){
        return ;
    }
    printf(" %d",next->key);
    print(next->lChild);
    print(next->rChild);
    
}

int main(){

    char instrcution;
    int key;
    int tmp;
    tree* a = (tree*)malloc(sizeof(tree));
    //트리 root 생성
    a->root = (node*)malloc(sizeof(node));
    makeExternal(a->root);
    while(1){
        scanf("%c",&instrcution);
        getchar();
        if(instrcution == 'q'){
            break;
        }else if(instrcution == 'i'){
            //키를 트리에 삽입
            scanf("%d",&key);
            getchar();
            insertTree(a,key);
        }else if(instrcution == 'd'){
            //삭제
            scanf("%d",&key);
            getchar();
            
        }else if(instrcution == 's'){
            //출력
            scanf("%d",&key);
            getchar();
            tmp = findKey(a->root, key);
            if(tmp == 0){
                printf("X\n");
            }else{
                printf("%d\n",tmp);
            }
        }else if(instrcution == 'p'){
            //전위 순회로 인쇄
            printf("print\n");
            print(a->root);
            printf("\n");
        }

        

    }

    free(a);

    return 0;
}