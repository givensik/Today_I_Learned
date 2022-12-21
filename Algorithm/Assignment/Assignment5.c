#include <stdio.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <limits.h>

#define MAX 30

int A[30]; // 항공 요금 배열
int H[30]; // 숙박 요금 배열
int m[30]; // 최소 비용 배열 

// function min
// parameter a, b
// return a와 b 중 작은 값
int min(int a, int b) {
	if (a > b) 
		return b; 
	else 
		return a; 
}

// function rAirtelDC
// parameter n, s(출발점), d(도착점)
// return 최소비용 minCost 반환
int rAirtelDC(int n, int s, int d){
	int minCost = INT_MAX; 
	int cost = 0; 
    int k;
    // 도착점(d) === 출발점(s)
	if(d == s){ 
		return 0;
    } 

	for(k = s; k < d; k++){
        
		if(k == s){ // k == s이면, 숙박요금 더하기 x 
            cost = rAirtelDC(n, s, k) + A[d - k];
        }else{ // k!=s일때, 숙박요금 더하기(H) 
            cost = rAirtelDC(n, s, k) + H[k] + A[d - k];
        }  
		minCost = min(minCost, cost); // min을 통해서 minCost, cost 중 최솟값 구하기
	}
	return minCost; 
}
// function airtelDP
// parameter n, s(출발점), d(도착점)
// return 도착점까지의 최소 비용 m[d] 반환
int airtelDP(int n, int s, int d){
	int cost = 0; 
    int i,j,k;
    // 0부터 출발점 s까지의 최소비용을 0으로 초기화
	for(i = 0; i <= s; i++){
        m[i] = 0; 
    }
	
	for(j = s + 1; j <= d; j++){
		m[j] = INT_MAX; 
		for(k = s; k < j; k++){
			if(k == s){ // k == s이면, 숙박요금 더하기 x 
                cost = m[k] + A[j - k];  
            }else{ // k!=s일때, 숙박요금 더하기(H)
                cost = m[k] + H[k] + A[j - k]; 
            } 
			m[j] = min(m[j], cost); // min을 통해 m[j]와 cost 중 최솟값 구하기 
		}
	}
	return m[d]; 
}

// function airtelDC
// parameter n, s(출발점), d(도착점)
// return 여행의 최소 비용 구하고 반환
int airtelDC(int n, int s, int d){
	return rAirtelDC(n, s, d); 
}

int main() {
	int n, min_1, min_2;
	double t; // cputtime 변수
    int i; // 반복 변수
	clock_t ticksPerSec = CLOCKS_PER_SEC;
	clock_t start, end, diff;

	A[0] = 0; // 항공 요금 0
	A[1] = 1; // 항공 요금 1

	for(i = 2; i < MAX; i++){
		A[i] = A[i - 1] + A[i - 1] % 5 + 3; 
    }

	H[0] = 0; // 숙박 요금 0
	H[1] = 5; // 숙박 요금 5

	for (int i = 2; i < MAX; i++){
        H[i] = (H[i - 1] + i)% 9 + 1;
    } 
		 
	
	printf("n\ts\td\tminocst\tversion\tcputime\n");

	n = 6;

	int s[4] = {0, 2, 2, 1}; 
	int d[4] = {4, 5, 4, 28}; 

	for (i = 0; i < 3; i++) { 
		start = clock();
		min_1 = airtelDC(n, s[i], d[i]); // airtelDC 호출
		end = clock();
		diff = end - start;
		t = ((double)diff / (double)ticksPerSec);

		printf("%d\t%d\t%d\t%2d\tDC\t%.8f\n", n, s[i], d[i], min_1, t);
        // 시간 측정 시작
		start = clock();
		min_2 = airtelDP(n, s[i], d[i]); 
		// 시간 측정 종료
        end = clock();
		diff = end - start; // 측정 시간 구하기
		t = ((double)diff / (double)ticksPerSec);

		printf("%d\t%d\t%d\t%2d\tDP\t%.8f\n", n, s[i], d[i], min_2, t);
	}
	n = MAX; 
    // 시간 측정 시작
	start = clock();
	min_1 = airtelDC(n, s[3], d[3]); 
    // 시간 측정 종료
	end = clock();
	diff = end - start;
	t = ((double)diff / (double)ticksPerSec);

	printf("%d\t%d\t%d\t%d\tDC\t%.8f\n", n, s[3], d[3], min_1, t);
	
	start = clock();
	min_2 = airtelDP(n, s[3], d[3]); 
	end = clock();
	diff = end - start;
	t = ((double)diff / (double)ticksPerSec);

	printf("%d\t%d\t%d\t%d\tDP\t%.8f\n", n, s[3], d[3], min_2, t);

	return 0;
}
