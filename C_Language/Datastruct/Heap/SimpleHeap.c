#include <stdio.h>
#include <stdlib.h>

#include "SimpleHeap.h"

void HeapInit(Heap * ph){
	ph->numOfData = 0;
}

int HIsEmpty(Heap * ph){
	if(ph->numOfData == 0){
		return TRUE;
	}else{
		return FALSE;
	}
}

int GetParentIDX(int idx){
	return idx/2;
}

int GetLChildIDX(int idx){
	return idx*2;
}
int GetRChildIDX(int idx){
	return GetLChildIDX(idx)+1;
}

// 우선순위가 높은 자식 노드의 인덱스를 출력
// 자식노드가 없음 -> 0, 자식 노드가 하나 -> 
int GetHiPriChildIDX(Heap * ph, int idx){
	// 왼쪽 자식 노드의 인덱스가 힙의 마지막 노드의 인덱스보다 크면
	if(GetLChildIDX(idx) > ph->numOfData){
		return 0;
	}else if(GetLChildIDX(idx) == ph->numOfData){
		return 1;
	}else{// 자식 노드 둘 다 있으면
		// 오른쪽 자식 노드의 우선순위가 높을 때(pr값이 작아야 우선순위가 높다.)
		if(ph->heapArr[GetLChildIDX(idx)].pr > ph->heapArr[GetRChildIDX(idx)].pr ){
			return GetRChildIDX(idx);
		}else{
			return GetLChildIDX(idx);
		}
	}
}


void HInsert(Heap * ph, HData data, Priority pr){
	printf("HINSERT %d\n", data);
	int idx = ph->numOfData + 1;
	HeapElem nelem = {pr, data};
	// idx가 1이 아닐때 = 1이면 루트 노드임
	while(idx != 1){
		// 새로 추가하는 원소의 우선순위가 부모보다 높을 때
		if(pr < (ph->heapArr[GetParentIDX(idx)]).pr){
			// idx에 부모 노드의 값을 저장
			ph->heapArr[idx] = ph->heapArr[GetParentIDX(idx)];
			// 부모노드의 인덱스를 idx로 저장 
			idx = GetParentIDX(idx);
		}else{// 만약 부모 노드보다 우선순위가 낮다면 반복문 멈추기
			break;
		}
	}

	ph->heapArr[idx] = nelem;
	ph->numOfData += 1;		
}
HData HDelete(Heap * ph){
	printf("Delete \n");
	// 지울 데이터 값 -> 루트 노드의 데이터
	HData retData = (ph->heapArr[1]).data;

	// 루트 노드에 마지막 노드를 저장
	HeapElem lastElem = ph->heapArr[ph->numOfData]; 
	
	int parentIdx = 1;
	int childIdx;

	// 루트 노드부터 우선 순위가 높은 자식 노드랑 비교 교환? 을 하지 않고 우선 순위만 비교하면서 인덱스 값의 값을 찾아감
	while(1){
		childIdx = GetHiPriChildIDX(ph, parentIdx);
		printf("lastnode = %c -> pr : %d\n ChildIdx = %c -> pr : %d\n",lastElem.data, lastElem.pr,(ph->heapArr[childIdx]).data,(ph->heapArr[childIdx]).pr  );
		// 마지막 노드의 우선순위가 자식 노드보다 더 낮을 때 -> 멈추기
		if(lastElem.pr > (ph->heapArr[childIdx]).pr){
			break;
		}
		ph->heapArr[parentIdx] = ph->heapArr[childIdx];
		parentIdx = childIdx;
	}

	ph->heapArr[parentIdx] = lastElem;
	ph->numOfData -= 1;

	return retData;
}