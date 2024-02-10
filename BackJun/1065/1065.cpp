#include <stdio.h>

const int M = 1000;

bool isHan(int N);

int main(){
	int N;
	int cnt = 0;

	scanf("%d",&N);

	for(int i=1; i<N+1; i++){
		if(isHan(i)){
			printf("%d\n", i);
			cnt++;
		}
	}
	printf("%d\n",cnt);
	return 0;
}

bool isHan(int N){
	// 100보다 작으면 무조건 등차수열임
	if(N < 100){
		// int sec = N / 10;
		// int thr = N % 10;
		// if(sec == thr){
		// 	return false;
		// }
		return true;
	}else{
		int fir = N / 100;
		int sec = (N % 100) / 10;
		int thr = N % 10;
		
		if(fir - sec == sec - thr ){
			return true;
		}else{
			return false;
		}

	}

}