#include <stdio.h>

void upheap(int *heap, int n){
    while(n>1){
        if(heap[n/2]<heap[n]){//부모보다 자식이 클 때
            heap[0] = heap[n/2];
            heap[n/2] = heap[n];
            heap[n] = heap[0];
            n/=2;
        }else{
            break;
        }
        
    }
}

void input_heap(int* heap, int n){
    int input;
    scanf("%d",&input);
    getchar();
    if(n==1){//처음 입력할 때
        heap[n]=input;
    }else{
        // printf("n=%d and upheap 들어감\n",n);
        heap[n]=input;
        upheap(heap,n);
    }
}



void print(int* heap, int n){
    int i;
    for(i=1;i<n;i++){
        printf(" %d",heap[i]);
    }
    printf("\n");
}

void down_heap(int* heap, int n){
    int i=1;
    while(n<i){
        if(heap[2*i] > heap[i] || heap[2*i+1] > heap[i]){//i번쨰 수보다 크면?
            if(heap[2*i] > heap[2*i+1]){
                heap[0] = heap[2*i];
                heap[2*i] = heap[i];
                heap[i] = heap[0];
            }else{
                heap[0] = heap[2*i+1];
                heap[2*i + 1] = heap[i];
                heap[i] = heap[0];
            }
        }else{
            break;
        }
    }
    

}

int delete(int* heap, int n){
    int key = heap[1];
    heap[1] = heap[n-1];//n-1이 마지막 노드
    heap[n-1] = 0; 
    n -= 2;
    down_heap(heap,n);
    return key;
}



int main(){

    char instruct;
    int heap[100]={0,};
    int n=1;
    while(1){

        scanf("%c",&instruct);
        getchar();
        switch(instruct){
            case 'i' :
                // printf("input\n");
                input_heap(heap,n);
                n++;
                break;
            case 'd' :
                // printf("delete\n");
                printf("%d\n",delete(heap,n));
                n--;
                break;
            case 'p' :
                // printf("print\n");
                print(heap,n);
                break;
            case 'q' :
                // printf("quit\n");
                return 0;
        }
    }

    return 0;
}