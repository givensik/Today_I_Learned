#include <stdio.h>
#include <stdlib.h>

typedef struct node {
    struct node* left;
    struct node* right;
    int number;
}node;

typedef struct tree{
    struct node* root;
}tree;

void init(tree *tr){
    tr->root = (node*)malloc(sizeof(node));
    tr->root->left = NULL;
    tr->root->number = -1;
    tr->root->right =NULL;
}

void add_tree(tree *tr, int a){

    if(tr->root->number == -1){//첫번째일때
        tr->root->number = a;
    }
}

node* find_node(){
    
}

int main(){

    tree *a = (tree*)malloc(sizeof(tree));
    
    init(a);
    int x,y,z;
    while(1){
        scanf("%d %d %d",&x,&y,&z);

    }

    


    
    return 0;
}