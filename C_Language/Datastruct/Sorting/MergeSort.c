#include <stdio.h>
#include <stdlib.h>

void MergeTwoArea(int arr[], int left, int mid, int right){
	// 각 영역의 첫번째 원소
	int fIdx = left;
	int rIdx = mid+1;
	// 합쳐진 배열을 임시로 넣어 둘 곳
	int * sortArr = (int*)malloc(sizeof(int)*(right+1));
	int sIdx = left;
	int i;

	// 비교해서 넣기
	while (fIdx <= mid && rIdx <= right)
	{	
		// 왼쪽이 더 작을 때
		if(arr[fIdx] <= arr[rIdx]){
			sortArr[sIdx] = arr[fIdx++];
		}else{
			sortArr[sIdx] = arr[rIdx++];
		}
		sIdx++;
	}
	// 여기서 fIdx나 rIdx 중에 하나가 먼저 mid, right에 도달하면 다른 애는 끝까지 못 넣음
	// if(fIdx <= mid){
	// 	for(i = fIdx; i<=mid; i++,sIdx++){
	// 		sortArr[sIdx] = arr[i];
	// 	}
	// }else{
	// 	for(i = rIdx; i<=right; i++,sIdx++){
	// 		sortArr[sIdx] = arr[i];
	// 	}
	// }
	if(fIdx > mid) {
		for(i = rIdx; i <= right; i++, sIdx++)
			sortArr[sIdx] = arr[i];
	}
	else {
		for(i = fIdx; i <= mid; i++, sIdx++)
			sortArr[sIdx] = arr[i];
	}

	// arr에 정렬된 값 넣기
	for(i = left; i<=right ; i++){
		arr[i] = sortArr[i];
	}

	free(sortArr);
	
	
}

void MergeSort(int arr[], int left, int right){
	int mid;

	// left < right -> 분할 가능한 상태
	if(left < right){
		mid = (left + right)/2;
		// 중간 값으로 나눠서 병합 정렬 실행
		MergeSort(arr, left, mid);
		MergeSort(arr, mid+1, right); // mid + 1을 해야 안겹침..	
		MergeTwoArea(arr,left, mid , right);
	}
	
}

int main(){

	int arr[] = {3, 2, 4, 1, 7, 6, 5};
	int len = sizeof(arr) / sizeof(arr[0]);
	int i;

	MergeSort(arr, 0, len - 1);

	for(i = 0; i < len; i++) {
		printf("%d ", arr[i]);
	}
	printf("\n");

	return 0;
}