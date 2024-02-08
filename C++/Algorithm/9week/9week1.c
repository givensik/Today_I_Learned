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
node* findKey(node* next, int key){
    int a,b;
    if(next->key == 0){
        return NULL;
    }
    if(next->key == key){
        return next;
    }else{
        if(findKey(next->lChild, key) != NULL){
            return findKey(next->lChild, key);
        }
        if(findKey(next->rChild, key) != NULL){
            return findKey(next->rChild, key);
        }
    }
    return NULL;
}

void print(node* next){
    if(next->key == 0){
        return ;
    }
    printf(" %d",next->key);
    print(next->lChild);
    print(next->rChild);
    
}

int isInternal(node *current){
    if(current->lChild->key != 0  || current->rChild->key != 0){
        return 1;
    }else{
        return 0;
    }
}

int isExternal(node *current){
    if(current->lChild->key == 0  || current->rChild->key == 0){
        return 1;
    }else{
        return 0;
    }
}

node* findM(node *current){
    printf("findM\n");
    node *next = current->rChild;
    //오른쪽 자식으로만 이동
    if(next->key==0){
        printf("error\n");
        return NULL;
    }
    while(1){
        if(next->lChild->key != 0){//다음 노드의 key가 0이 아니면 왼쪽으로 이동
            next = next->lChild;
            continue;
        }else if(next->lChild->key == 0){//노드의 왼쪽 자식의 키가 0이면 반복문 중단
            return next;
            break;
        }
    }
    return next;
}

void reduceExternal(node *x){

    free(x->lChild);
    free(x->rChild);
}

void Delete(node *a, int key){
    printf("Delete\n");
    node* d = findKey(a, key);
    node *tmp;
    printf("d = %d\n",d->key);

    if(d->key==0){//key에 해당하는 값이 없을때
        printf("X\n");
    }else if(d->key == key){//key값이 있을때
        //자식들 모두 외부노드인경우 -> key랑 그 밑의 가지들 지우기
        printf("key있을때\n");
        if(d->lChild->key ==0 && d->rChild->key == 0){
            printf("모두 외부 노드");
            printf("%d\n",d->key);
            d->key = 0;
            reduceExternal(d);
        }else if(isInternal(d->lChild) && isInternal(d->rChild)){//모두 내부노드인 경우
            // 중위순위 후계자 찾기
            tmp = findM(d);
            if(d->rChild->key != 0){
                d->parent->lChild = d->rChild; //옮겨질애의 오른쪽 자식을 옮겨질애 자리로
                d->key = tmp ->key;
                tmp ->key =0;
                free(tmp->lChild);
            }else{
                d->key = tmp ->key;
            }
        }else{
            printf("1234");
        }
        
        // printf("%d\n",tmp->key);
    }
}


int main(){
    char instrcution;
    int key;
    node* tmp;
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
            Delete(a->root,key);
            
        }else if(instrcution == 's'){
            //출력
            scanf("%d",&key);
            getchar();
            tmp = findKey(a->root, key);
            if(tmp){
                printf("%d\n",tmp->key);
            }else{
                printf("X\n");
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