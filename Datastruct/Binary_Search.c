//이진 탐색 알고리즘
#include <stdio.h>
int Bsearch(int *x, int first, int last, int target);
int main(){

    int x[] = {1,3,5,7,8,9,10};
    int target = 3;
    // printf("%d\n",(int)(sizeof(x)/sizeof(int)));
    printf("Bsearch = %d\n",Bsearch(x, 0, (int)(sizeof(x)/sizeof(int)), target));
    target = 6;
    printf("Bsearch = %d\n",Bsearch(x, 0, (int)(sizeof(x)/sizeof(int)), target));
    return 0;
}
int Bsearch(int *x, int first, int last, int target){
    int mid =0;
    while(first<=last){
        mid = (first+last)/2;
        //printf("first = %d, last = %d, mid = %d\n",first, last, x[mid]);
        if(x[mid] == target){
            return mid;
        }else if(x[mid] < target){
            first = mid + 1;
        }else if(x[mid] > target){
            last = mid - 1;
        }
    }

    return -1;
}