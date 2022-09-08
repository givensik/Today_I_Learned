#include <stdio.h>
#include <stdlib.h>


typedef struct DLnode {
    char elem;
    struct DLnode *prev;
    struct DLnode *next;

}node;

typedef struct list{
    node *header;
    int num;
    node *trailer;
}list;

//init -> 초기화
void init(list* l){
    node *head, *trail;

    //메모리 할당
    head = (node*)malloc(sizeof(node));
    trail = (node*)malloc(sizeof(node));

    //초기화
    head->next = trail;
    head->elem = 0;
    trail->prev = head;
    trail->elem = 0;

    l->header = head;
    l->trailer = trail;
    l->num = 0;
    
}
//리스트에서 조회 -> 주소가져오기
node* select(list *l, int r){   
    int i =0;
    node *s = l->header;

    for(i=0;i<r;i++){
        s = s->next;
    }
    return s;
}


//리스트에 원소 추가
void add(list *l, int r, char e){
    //printf("add함수 들어옴\n");
    if(r > l->num+1){
        printf("invalid position\n");
        return;
    }
    node *new_node;
    new_node = (node*)malloc(sizeof(node));

    new_node->elem = e;//원소 추가

    node *tmp;
    tmp = select(l,r);//r번째 원소 찾기
    
    tmp->prev->next = new_node;
    new_node->prev = tmp->prev;
    new_node->next = tmp;
    tmp->prev = new_node;




    //printf("new_node's element = %c\n",new_node->elem);
    // printf("add complete!\n");
    l->num ++;
    // printf("num = %d\n",l->num);

}

//리스트 에서 원소 가져오기
char get(list *l, int r){

    if(r > l->num){
        printf("invalid position\n");
        return 0;
    }
    node* s = select(l,r);
    return s->elem;
}
//리스트에서 원소 지우기
void delete(list *l, int r){

    if(r > l->num){
        printf("invalid position\n");
        return;
    }

    node *r_node = select(l,r);
    node *prev = r_node->prev;
    node *next = r_node->next;

    prev->next = next;
    next->prev = prev;
    l->num --;
}

void print(list* l){
    // printf("print\n");
    int i;
    node *tmp = l->header;
    for(i=1;i<=l->num;i++){
        tmp = tmp->next;
        printf("%c",tmp->elem);
    }
    printf("\n");
}


int main(){

    list* a;
    
    a = (list*)malloc(sizeof(list));
    init(a);

    int N;
    scanf("%d",&N);
    getchar();
    char OP = 0, value;
    int r;
    for (int i = 0; i < N; i++)
    {
        scanf("%c",&OP);
        getchar();
        if(OP == 'A'){
            // printf("ADD\n");
            scanf("%d %c",&r,&value);
            getchar();
            add(a,r,value);
        }else if(OP == 'D'){
            //printf("DELETE\n");
            scanf("%d",&r);
            getchar();
            delete(a,r);
        }else if(OP == 'G'){
            // printf("GET\n");
            scanf("%d",&r);
            getchar();
            if(get(a,r) == 0){
                continue;
            }else{
                printf("%c\n",get(a,r));
            }
            
        }else if(OP == 'P'){
            // printf("PRINT\n");
            print(a);
        }

    }
    






    

    return 0;
}