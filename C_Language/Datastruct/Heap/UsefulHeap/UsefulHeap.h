#ifndef __USEFUL_HEAP_H__
#define __USEFUL_HEAP_H__


#define TRUE 1
#define FALSE 0

#define HEAP_LEN 100

typedef char HData;
typedef int PriorityComp(HData d1, HData d2); // 우선순위 비교 함수 포인터
/*
	PritorityComp 함수 가이드라인
	인자 1의 우선순위가 높다면 0보다 큰 값 반환
	인자 2의 우선순위가 높다면 0보다 작은 값 반환
	우선순위가 동일하다면 0을 반환
*/
typedef struct _heap
{
	PriorityComp * comp;
	int numOfData;
	HData heapArr[HEAP_LEN];
} Heap;


void HeapInit(Heap * ph, PriorityComp * pc);
int HIsEmpty(Heap * ph);

void HInsert(Heap * ph, HData data);
HData HDelete(Heap * ph);


#endif