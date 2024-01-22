#include <stdio.h>
#include <string.h>
#include <time.h>
#include <Windows.h>
#include <stdlib.h>
#pragma warning(disable: 4996)
int MAX = 30;
int airtelDC(int n, int s, int d);
int rAirtelDC(int n, int s, int d);
int airtelDP(int n, int s, int d);
int H[30]; // 숙박요금 H배열 선언
int A[30]; // 항공요금 A배열 선언
int m[30]; // 출발점부터 도착점까지의 최소비용을 저장할 m배열 선언
int main() {
	int n, min1, min2;
	LARGE_INTEGER ticksPerSec;
	LARGE_INTEGER start, end, diff;
	double t; // cputime 저장할 t 선언
	A[0] = 0; // 0지점 항공 요금 0 저장
	A[1] = 1; // 1지점 항공 요금 1 저장
	for (int i = 2; i < MAX; i++) {
		A[i] = A[i - 1] + A[i - 1] % 5 + 3; // 나머지 항공요금 A배열 초기화
	}
	H[0] = 0; // 0지점 숙박 요금 0 저장
	H[1] = 5; // 1지점 숙박 요금 5 저장
	for (int i = 2; i < MAX; i++) {
		H[i] = (H[i - 1] + i)% 9 + 1; // 나머지 숙박요금 H배열 초기화
	}
	printf(" n  s  d  minocst  version    cputime\n");
	n = 6; // n이 6일 때
	int s[4] = { 0, 2, 2, 1 }; // 4가지 경우의 출발점 s 초기화
	int d[4] = { 4, 5, 4, 28 }; // 4가지 경우의 도착점 d 초기화
	for (int i = 0; i < 3; i++) { // for (s, d) in {(0, 4), (2, 5), (2, 4)}
		QueryPerformanceFrequency(&ticksPerSec);
		QueryPerformanceCounter(&start); // start = airtelDC() 함수 시작시간 저장
		min1 = airtelDC(n, s[i], d[i]); // 출발점 s와 도착점 d에 따른 최소 비용을 min1에 저장
		QueryPerformanceCounter(&end); // end = StopClock() 함수 종료시간 저장
		diff.QuadPart = end.QuadPart - start.QuadPart; // 시작시간와 종료시간의 차이 계산
		t = ((double)diff.QuadPart / (double)ticksPerSec.QuadPart); // t ← CPUtime()
		printf(" %d  %d  %d    %2d       DC      %.8f\n", n, s[i], d[i], min1, t);
		// n,  출발점 s, 도착점 d, 최소비용, 버전, cputime 출력
		QueryPerformanceFrequency(&ticksPerSec);
		QueryPerformanceCounter(&start); // start = airtelDP() 함수 시작시간 저장
		min2 = airtelDP(n, s[i], d[i]); // 출발점 s와 도착점 d에 따른 최소 비용을 min2에 저장
		QueryPerformanceCounter(&end); // end = StopClock() 함수 종료시간 저장
		diff.QuadPart = end.QuadPart - start.QuadPart; // 시작시간와 종료시간의 차이 계산
		t = ((double)diff.QuadPart / (double)ticksPerSec.QuadPart); // t ← CPUtime()
		printf(" %d  %d  %d    %2d       DP      %.8f\n", n, s[i], d[i], min2, t);
		// n,  출발점 s, 도착점 d, 최소비용, 버전, cputime 출력	
	}
	n = MAX; // n = 30, s = 1, d = 28 일때
	QueryPerformanceFrequency(&ticksPerSec);
	QueryPerformanceCounter(&start); // start = airtelDC() 함수 시작시간 저장
	min1 = airtelDC(n, s[3], d[3]); // 출발점 s와 도착점 d에 따른 최소 비용을 min1에 저장
	QueryPerformanceCounter(&end); // end = StopClock() 함수 종료시간 저장
	diff.QuadPart = end.QuadPart - start.QuadPart; // 시작시간와 종료시간의 차이 계산
	t = ((double)diff.QuadPart / (double)ticksPerSec.QuadPart); // t ← CPUtime()
	printf(" %d %d  %d   %d       DC      %.8f\n", n, s[3], d[3], min1, t);
	// n,  출발점 s, 도착점 d, 최소비용, 버전, cputime 출력	
	QueryPerformanceFrequency(&ticksPerSec);
	QueryPerformanceCounter(&start); // start = airtelDC() 함수 시작시간 저장
	min2 = airtelDP(n, s[3], d[3]); // 출발점 s와 도착점 d에 따른 최소 비용을 min2에 저장
	QueryPerformanceCounter(&end); // end = StopClock() 함수 종료시간 저장
	diff.QuadPart = end.QuadPart - start.QuadPart; // 시작시간와 종료시간의 차이 계산
	t = ((double)diff.QuadPart / (double)ticksPerSec.QuadPart); // t ← CPUtime()
	printf(" %d %d  %d   %d       DP      %.8f\n", n, s[3], d[3], min2, t);
	// n, 출발점 s, 도착점 d, 최소비용, 버전, cputime 출력	
}
int airtelDC(int n, int s, int d) {
	return rAirtelDC(n, s, d); // 여행의 최소비용 구하고 반환
}
int rAirtelDC(int n, int s, int d) {

	if (d == s) return 0; // 도착점이 출발점과 같다면 0 반환 후 종료
	int mincost = INT_MAX; // mincost에 최대한 큰 값 저장
	int cost = 0; // cost 0으로 초기화
	for (int k = s; k < d; k++) {
		if (k == s) { cost = rAirtelDC(n, s, k) + A[d - k]; }
		// 만약 k가 s면 숙박요금은 0이므로 H를 더하지 않음
		else { cost = rAirtelDC(n, s, k) + H[k] + A[d - k]; }
		// k가 s가 아니면 정상적으로 숙박요금을 더함
		mincost = min(mincost, cost); 
		// 기존의 mincost와 새로 구한 cost중 더 작은값을 mincost로 초기화
		
	}
	return mincost; // 최소비용 mincost 반환
}
int airtelDP(int n, int s, int d) {
	for (int i = 0; i <= s; i++) {
		m[i] = 0; // 0부터 출발점 s까지의 최소비용을 0으로 초기화
	}
	int cost = 0; // cost 0으로 초기화
	for (int j = s + 1; j <= d; j++) {
		m[j] = INT_MAX; // 현재 최소비용 m[j]에 최대한 큰 값 저장
		for (int k = s; k < j; k++) {
			if (k == s) { cost = m[k] + A[j - k]; } 
			// 만약 k가 s면 숙박요금은 0이므로 H를 더하지 않음
			else { cost = m[k] + H[k] + A[j - k]; }
			// k가 s가 아니면 정상적으로 숙박요금을 더함
			m[j] = min(m[j], cost);
			// m[j]와 새로 구한 cost중 더 작은 값을 최소비용 m[j]로 초기화
		}
	}
	return m[d]; // 도착점 d까지의 최소비용 m[d] 반환
}