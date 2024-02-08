#include <stdio.h>

typedef int compfunc(int d1, int d2);

void BubbleSort(int arr[], int n, compfunc * cp);


/*
	d1 > d2 -> return값 < 0
	d1 = d2 -> return값 = 0
	d1 < d2 -> return값 > 0
*/
int compare(int d1, int d2){
	return d2 - d1;
}


int main(){
	int arr[4] = { 3, 2, 4, 1};
	int i;

	BubbleSort(arr, 4, compare);
	for(i=0;i<4;i++){
		printf("%d ", arr[i]);
	}
	printf("\n");

	return 0;
}




void BubbleSort(int arr[], int n, compfunc * cp){
	int i,j, tmp;

	for(i=0;i<n;i++){
		for(j=i ; j<n; j++){
			// 왼쪽에 있는애가 더 크면? 교체 -> 오름 차순
			if(cp(arr[i], arr[j]) < 0){
				tmp = arr[i];
				arr[i] = arr[j];
				arr[j] = tmp;				
			}
		}
	}
}