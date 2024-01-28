#include <stdio.h>


int ISearch(int arr[], int first, int last, int target){
	int mid;

	if(arr[first] > target || arr[last] < target ){
		return -1;
	}

	mid = ((double)(target - arr[first])/(arr[last] - arr[first]) *(last - first)) + first;
	
	if(arr[mid] == target){
		return mid;
	}else if(arr[mid] > target){// target보다 mid값이 더 크다 -> 더 작은 값으로 가야함
		return ISearch(arr, first, mid-1, target);	
	}else{
		return ISearch(arr, mid+1, last, target);
	}


	return 0;
}


int main(){
	int arr[] = {1, 3, 5, 7 ,9};
	int idx;

	idx = ISearch(arr, 0, sizeof(arr)/sizeof(arr[0]) - 1, 7);

	if(idx == -1){
		printf("실패\n");
	}else{
		printf("target 찾기 성공 idx = %d\n", idx);
	}

	idx = ISearch(arr, 0, sizeof(arr)/sizeof(arr[0]) - 1, 10);

	if(idx == -1){
		printf("실패\n");
	}else{
		printf("target 찾기 성공 idx = %d\n", idx);
	}

	return 0;
}