#include <stdio.h>

void Swap(int arr[], int idx1, int idx2);

int Partition(int arr[], int left, int right);

void QuickSort(int arr[], int left, int right);


void QuickSort(int arr[], int left, int right){
	
	if(left <= right){
		int pivot = Partition(arr, left, right);
		QuickSort(arr, left, pivot-1);
		QuickSort(arr, pivot+1, right);
	}
}

void Swap(int arr[], int idx1, int idx2){
	int tmp;
	tmp = arr[idx1];
	arr[idx1] = arr[idx2];
	arr[idx2] = tmp;
}

int Partition(int arr[], int left, int right){
	int pivot;
	int low, high;
	
	low = left+1;
	high = right;
	pivot = arr[left];

	// low랑 high가 교차되지 않았을 때
	while(low <= high){
		while(arr[low] < pivot){
			low++;
		}
		while(arr[high] > pivot){
			high--;
		}
		if(low<=high){
			Swap(arr, low, high);
		}
	}

	// pivot이랑 high랑 바꾸기
	Swap(arr, high, left);

	return high;
}



int main(){

	int arr[] = {3, 2, 4, 1, 7, 6, 5};
	int len = sizeof(arr) / sizeof(arr[0]);
	int i;

	QuickSort(arr, 0, len - 1);

	for(i = 0; i < len; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");

	return 0;
}