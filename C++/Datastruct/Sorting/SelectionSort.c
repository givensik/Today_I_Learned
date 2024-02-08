#include <stdio.h>

typedef int compfunc(int d1, int d2);

/*
	d1 > d2 -> return값 < 0
	d1 = d2 -> return값 = 0
	d1 < d2 -> return값 > 0
*/
int compare(int d1, int d2){
	return d2 - d1;
}

void SelectionSort(int arr[], int n, compfunc * cp){
	int i,j,tmp;
	
	int minIdx;
	for(i=0; i<n-1; i++){
		minIdx = i;
		// 최솟값 찾기
		for(j=i+1 ; j<n; j++){
			// arr[j]가 더 작을 때
			if(cp(arr[minIdx], arr[j]) <0 ){
				minIdx = j;
			}
		}
		tmp = arr[i];
		arr[i] = arr[minIdx];
		arr[minIdx] = tmp;
		// printf("i = %d일때, 최솟값 %d\n", i, arr[minIdx]);
	}

}

int main(){

	int arr[4] = { 3, 2, 4, 1};
	int i;

	SelectionSort(arr, 4, compare);
	for(i=0;i<4;i++){
		printf("%d ", arr[i]);
	}
	printf("\n");

	return 0;
}