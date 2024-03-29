#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void insertion_sort(int *arr, int n){
    for (int i = 1; i < n; i++){
        int j = i;
        while (j > 0 && arr[j - 1] > arr[j]){
            int temp = arr[j];
            arr[j] = arr[j - 1];
            arr[j - 1] = temp;
            j--;
        }
    }
}

int main(){
    int n;

    scanf("%d", &n);

    int *arr = (int *)malloc(sizeof(int) * n);

    for (int i = 0; i < n; i++){
        scanf("%d", &arr[i]);
    }

    insertion_sort(arr, n);
    for (int i = 0; i < n; i++){
        printf(" %d", arr[i]);
    }

    free(arr);
    arr = NULL;

    return 0;
}