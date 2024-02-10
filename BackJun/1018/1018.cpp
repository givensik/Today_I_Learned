#include <stdio.h>
#include <iostream>
#include <math.h>
#include <string.h>

using namespace std;
/*
	C++을 사용하고 있고 cin/cout을 사용하고자 한다면, cin.tie(NULL)과 sync_with_stdio(false)를 둘 다 적용해 주고, endl 대신 개행문자(\n)를 쓰자. 
*/

int m = 64;

int chessBoard(string* board, int x, int y);

int main(){
	int N, M;
	cin >> N >> M;
	
	string *Board = new string[N];

	for(int i =0 ;i< N; i++){
		cin >> Board[i];
	}

	// cout << "N :" << N << " M :" << M << "\n";
	for(int i = 0 ; i<= N - 8 ; i++){
		for(int j = 0 ; j<= M - 8 ; j++){
			m = min(chessBoard(Board, i, j), m);
		}
	}
	cout << m << "\n";
	return 0;
}

int chessBoard(string* board,int x, int y){
	// printf("x : %d, y: %d 일때\n",x,y);
	int cnt1 = 0;
	int cnt2 = 0;
	
	// 처음이 흰색으로 가정
	// printf("흰색이 처음이다?\n");
	for(int i = 0; i < 8 ; i++){
		for(int j = 0; j< 8 ; j++){
			// i+j가 짝수이면 흰색이 아니면 바꿔야함
			if((i+j) % 2 == 0 && board[x+i][y+j] == 'B'){
				// printf("%d, %d 일때, B->W\n",i,j);
				cnt1++;
			}else if((i+j) % 2 == 1 && board[x+i][y+j] == 'W'){ // 홀수일 떄 검정색이 아니라면
				// printf("%d, %d 일때, W->B\n",i,j);
				cnt1++;
			}
		}
	}
	//처음이 검정색이라면
	// printf("검은색이 처음이다?\n"); // 짝수일때 검은색, 홀수일때 흰색
	for(int i = 0; i < 8 ; i++){
		for(int j = 0; j< 8 ; j++){
			// i+j가 짝수이면 검정색이 아니면 바꾸기
			// printf("x+i : %d, y+j : %d\n", x+i, y+j);
			if((i+j) % 2 == 0){
				if(board[x+i][y+j] == 'W'){
					// printf("%d, %d 일때, %c W->B",i,j, board[x+i][y+j]);
					cnt2++;
				}
			}else if((i+j) % 2 == 1){ // 홀수일 떄 흰색이 아니라면
				if(board[x+i][y+j] == 'B'){
					// printf("%d, %d 일때, %c B->W",i,j,board[x+i][y+j]);
					cnt2++;
				}
			}
			// printf("\n");
		}
	}


	if(cnt1 < cnt2){
		return cnt1;
	}else{
		return cnt2;
	}
}

/*

10 13
BBBBBBBBWBWBW
BBBBBBBBBWBWB
BBBBBBBBWBWBW
BBBBBBBBBWBWB
BBBBBBBBWBWBW
BBBBBBBBBWBWB
BBBBBBBBWBWBW
BBBBBBBBBWBWB
WWWWWWWWWWBWB
WWWWWWWWWWBWB

BBBBWBWB
BBBBBWBW
BBBBWBWB
BBBBBWBW
BBBBWBWB
BBBBWBWB
BBBBWBWB
BBBBBWBW

8 8
WBWBWBWB
BWBWBWBW
WBWBWBWB
BWBBBWBW
WBWBWBWB
BWBWBWBW
WBWBWBWB
BWBWBWBW

*/