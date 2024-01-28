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


/*
	삽입 정렬은 왼쪽부터 정렬된 부분을 만들어가는 정렬임
	5 3 2 1 4
	
	3 5 | 2 1 4
	2 3 5 | 1 4
	1 2 3 5 | 4
	1 2 3 4 5 | 

	이런식으로 왼쪽 영역이 정렬된 부분이 된다.
	이거는 정렬이 안된 첫번째 원소를 바로 왼쪽의 정렬된 원소랑 비교하면서 자리를 교체하면 되는데 교체하는 것보단 바꿔야하면 정렬된 애를 오른쪽으로 밀고,
	교체하지 않아도 되면 그 자리에 그 원소를 넣는 방식으로 하면 됨.

*/
void insertSort(int arr[], int n, compfunc * cp){
	int i,j,tmp, cIdx;

	// i -> 정렬된 영역의 마지막 위치
	for(i=1;i<n;i++){
		tmp = arr[i]; // 정렬이 안된 첫번째 원소
		for(j=i-1 ; j>=0; j--){
			// 왼쪽에 있는애가 더 크면? 교체 -> 오름 차순
			if(cp(arr[j], tmp) < 0){
				arr[j+1] = arr[j];	
			}else{
				break;
			}
		}
		arr[j+1] = tmp;
	}
}

int main(){
	int arr[4] = { 3, 2, 4, 1};
	int i;

	insertSort(arr, 4, compare);
	for(i=0;i<4;i++){
		printf("%d ", arr[i]);
	}
	printf("\n");

	return 0;
}
