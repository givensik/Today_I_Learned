#include <stdio.h>
#include <stdlib.h>

typedef struct _Graph {
	int **matrix; //그래프 인접행렬
	int vertexCount; //정점 개수
}Graph;

int *label = NULL;	// label을 전역변수로 지정 (힙으로 사용하지 않고 그냥 순서를 저장하는 배열)

Graph* graph_create(int max_vertex);	// 그래프 생성 함수
void graph_add_edge(Graph *graph, int from, int target,int weight);	// 경로 저장

int main(){
    int n, m, s;	// 입력 변수 선언
	int i, st, en;	// 그래프 입력 값

    scanf("%d %d %d",&n,&m,&s);

	// 라벨 배열 생성
    label = (int*)malloc(sizeof(int)*n);

	// graph 생성
	Graph *graph = graph_create(n);

	// 간선의 정보 저장
	for(i = 0; i < m; i++){
		scanf("%d %d",&st,&en);
		graph_add_edge(graph,st,en,1);
	}

	// 너비 우선 탐색 함수 (BFS)
	BFS(graph,s,n);

	// 라벨에 저장한 순회 순서 출력
	for (i = 0;i<n;i++){
		printf("%d\n",label[i]);
	}
	return 0;
}

// 그래프 생성
Graph* graph_create(int max_vertex) {
	int i = 0;
	Graph* graph = (Graph*)malloc(sizeof(Graph));	// 그래프 동적할당
	graph->vertexCount = max_vertex; //최대 정점 개수 설정
	graph->matrix = (int **)malloc(sizeof(int *)*(max_vertex+1));//인접행렬 메모리 할당

	for (i = 0; i <= max_vertex; i++) {
		graph->matrix[i] = (int *)malloc(sizeof(int)*(max_vertex+1));//i행 메모리 할당
		memset(graph->matrix[i], 0, sizeof(int)*(max_vertex+1)); //메모리 0으로 초기화
	}
	// 생성한 그래프 반환
	return graph;
}

// 간선 생성
void graph_add_edge(Graph *graph, int from, int target,int weight) {
	graph->matrix[from][target] = weight; //간선 weight 설정
	graph->matrix[target][from] = weight; //간선 weight 설정
}

// 너비 우선 탐색 함수
void BFS(Graph *graph, int start, int n) {
	int i, cnt = 0, j, flag, k = 0;

	// 그래프 순회의 시작을 라벨에 저장하고 라벨에 저장한 순회 순서의 개수를 증가
	label[cnt++] = start;

	// k가 그래프 순회 입력 개수보다 작을 때 반복문 실행
	while(k<n){
		// label에 있는 값을 시작으로 정하고 아래 반복문 실행
		start = label[k++];
		for (i = 1; i <= n; i++) {
			// 그래프에 방문한 적이 있으면 저장
			if(graph->matrix[start][i]){
				flag = 0;
				// 그 방문한 값이 이미 라벨에 존재하는 경우 반복문 탈출
				for(j = 0; j < cnt; j++) {
					if (i == label[j]) flag = 1; 
				}
				if (flag == 1) continue;
				// 아닌 경우 라벨에 새로운 값 저장
				else {
					label[cnt++] = i;
				}
			}
			// 그래프에 방문한 적이 있으면 저장
			else if (graph->matrix[i][start]){
				flag = 0;
				// 그 방문한 값이 이미 라벨에 존재하는 경우 반복문 탈출
				for(j = 0; j < cnt; j++) {
					if (i == label[j]) flag = 1; 
				}
				if (flag == 1) continue;
				// 아닌 경우 라벨에 새로운 값 저장
				else {
					label[cnt++] = i;
				}
			}
			else {
				continue;
			}
		}
	}

	// 결국 라벨에 모든 순회 순서를 저장하고 함수 종료
}