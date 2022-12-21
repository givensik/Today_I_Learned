#include<stdio.h>
#include<stdlib.h>

typedef int ElementType;

// struct tagVertex
typedef struct tagVertex {
	ElementType data; // 데이터 
	struct tagVertex *next; // 다음 정점
	struct tagEdge *aList; // 인접 정점 리스트
	int isVisited; // 방문여부
}Vertex;

typedef struct tagEdge {
	struct tagEdge *next; // 다음 간선
	Vertex *from; // 정점 시작
	Vertex *target; // 정점 끝
}Edge;

typedef struct tagGraph {
	Vertex *vertices; // 정점 목록

}Graph;


// function CreateGraph()
// return 생성한 그래프 반환
// 그래프 생성 함수
Graph* CreateGraph() {
    // 그래프 동적할당
	Graph* graph = (Graph*)malloc(sizeof(Graph));
	// 초기화 후 반환
    graph->vertices = NULL;
	return graph;
}

// function ininVertex()
// return 
// 정점 생성 함수
void initVertex(Vertex *vertex, int vertexSize) {
	int i = 0;
    // 초기화
	for(i = 1; i <= vertexSize; i++){
		vertex[i].data = i;
		vertex[i].next = NULL;
		vertex[i].aList = NULL;
		vertex[i].isVisited = 0;
	}

}

// function CreateEdge
// return 생성한 간선 반환
// 간선 생성 함수
Edge* CreateEdge(Vertex* from, Vertex* target) {
    // 동적할당 후 값 넣어주고 반환
	Edge *edge = (Edge*)malloc(sizeof(Edge));
	edge->from = from;
	edge->target = target;
	edge->next = NULL;

	return edge;
}

// function AddVertex
// return 
// 정점 추가 함수
void AddVertex(Graph* graph, Vertex* vertex) {
	Vertex* vertexList = graph->vertices;

    // 리스트에 값이 없으면 그래프의 정점에 정점 추가
	if(vertexList == NULL){
		graph->vertices = vertex;
	}else{
        // list가 끝날 때까지 리스트에 정점 추가
		while(vertexList->next != NULL){
            // 중복되면 함수 종료
			if (vertexList->data == vertex->data||vertexList->next->data==vertex->data) {
				return;
			}
			vertexList = vertexList->next;
		}
		vertexList->next = vertex;
	}


}

// function AddEdge
// return 
// 간선 추가 함수
void AddEdge(Vertex* vertex, Edge* edge) {
    // 인접리스트가 없다면 간선을 추가
	if(vertex->aList == NULL){
		vertex->aList = edge;
    }else{
        // 정점의 인접 리스트를 따로 저장
		Edge *aList = vertex->aList;

		while(aList->next != NULL){
            // 인접리스트의 정점 값이 더 클때, 맨 앞에 넣기
			if(aList->target->data > edge->target->data){
				edge->next = aList;
				vertex->aList = edge;
				return;
			}
            // 인접리스트의 다음 정점 값이 클때
			else if(aList->next->target->data > edge->target->data){
				edge->next = aList->next;
				aList->next = edge;
				return;
			}
			aList = aList->next;
		}
        // 인접리스트의 정점이 간선의 값보다 클때
		if(aList->target->data > edge->target->data){
			edge->next = aList;
			vertex->aList = edge;
			return;
		}
        // 인접리스트의 다음으로 간선 추가
		aList->next = edge;
	}
}


// function DFS
// return 
// 깊이 우선 탐색 (DFS)
void DFS(Vertex *start) {
    // 방문 흔적 남기기
	start->isVisited = 1;

	Edge *edge = NULL;

    // 방문 값 출력하기
	printf("%d\n", start->data);

	edge = start->aList;

    // 재귀로 불러서 하나씩 출력
	while(edge != NULL){
        // 연결되어 있는 것 중에 방문하지 않을때 재귀
		if(edge->target != NULL && edge->target->isVisited == 0){
			DFS(edge->target);
		}
		edge = edge->next;
	}
}

int main() {
    
	Graph *graph = CreateGraph(); // 그래프 생성

	int num, vertex1, vertex2;;
	char ch; 

	int i = 0, n, m, s;
	Vertex *vertex;

    // 정점 개수, 간선 개수, 순회 시작 번호 입력 받기
	scanf("%d %d %d", &n, &m, &s);
	vertex = (Vertex*)malloc(sizeof(Vertex)*(n+1));

	initVertex(vertex, n);

    // 두 정점의 간선 연결
	for (i = 0; i < m; i++) {
		scanf("%d %d", &vertex1, &vertex2);
        // 정점 추가
		AddVertex(graph, (vertex+vertex1));
		AddVertex(graph, (vertex+vertex2));
        // 간선 추가
		AddEdge((vertex+vertex1), CreateEdge(vertex+vertex1, vertex+vertex2)); 
		AddEdge((vertex+vertex2), CreateEdge(vertex+vertex2, vertex+vertex1));
	}

    // DFS 호출
	DFS((vertex + s));

	return 0;
}








