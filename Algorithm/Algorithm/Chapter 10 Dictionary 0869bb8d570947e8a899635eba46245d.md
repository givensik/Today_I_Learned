# Chapter 10 Dictionary

---

# 사전 ADT

### 사전

- 수많은 (단어, 설명) 쌍의 모음으로 구성

### 주요 작업

- 탐색
- 삽입
- 삭제

### 종류

- 무순 사전 ADT (unordered dictionary ADT)
- 순서사전 ADT (ordered dictionary ADT)

# 사전 ADT 메쏘드

### 일반 메쏘드

- integer size() : 사전의 항목 수를 반환
- boolean isEmpty() : 사전이 비어 있는지 여부를 반환

### 접근 메쏘드

- element findElement(k) : (키, 원소) 항목들의 모음인 사전에 키 k를 가진 항목이 존재하면 해당 원소르 ㄹ반환, 그렇지 않으면 특별 원소 NoSuchKey를 반환

### 갱신 메쏘드

- insertItem(k,e) : 사전에 (k,e) 항목을 삽입
- element removeElement(k) : 사전에 키 k를 가진 항목이 존재하면 해당 항목을 삭제하고 원소를 반환, 그렇지 않으면 특별 원소 NoSuchKey를 반환

# 사전 ADT 구현

### 사전 구현의 가장 중요한 목적

- 탐색

### 종류

- 유일 키 사전
    
    한 개의 키 → 하나의 데이터 항목만 존재
    
- 중복 키 사전
한 개의 키 → 여러 개의 데이터 항목이 존재

| 구현 형태 | 구현 종류 | 예 | 주요 탐색 기법 |
| --- | --- | --- | --- |
| 리스트 | 무순 사전 ADT | 기록 파일 | 선형탐색 |
| 리스트 | 순서 사전 ADT | 일람표 | 이진탐색 |
| 트리 | 탐색트리 | 이진탐색트리, AVL트리, 스플레이 트리 | 트리탐색 |
| 해시테이블 |  |  | 해싱 |

## 무순 사전 ADT

- 무순리스트 → 이중연결리스트, 원형배열 등을 이용하여 상세 구현

### 무순리스트로 사전 구현 성능

- 삽입(insertItem) : 새로운 항목을 기존 리스트의 맨 앞 또는 맨 뒤에 삽입 → O(1)
- 탐색(findElement) 및 삭제(removeElement) : 최악의 경우 리스트 전체를 순회 → O(n)
- 삽입은 빠르지만 탐색과 삭제가 느림 → 소규모 상관 x, 규모가 큰 기록파일에 유리
    - 예시 : 서버의 접속자가 많아 로그인 기록 삽입이 빈번하지만 로그인 기록을 탐색하거나 삭제하는 일은 드물기 떄문에 무순리스트가 유리

### 선형탐색(linear search)

- findElement
    
    ```
    Alg findElement(k)
    	input list L, key k
    	output element with key k
    
    1. L.initialize(i)
    2. while (L.isValid(i))
    			if (L.key(i) = k)
    					return L.element(i)
    			else 
    					L.advance(i)
    3. return NoSuchKey
    ```
    
    - 수행 시간 : O(n)
    - 기억공간 소요량 : O(1)

## 순서사전 ADT

### 순서리스트로 사전 구현 성능

- 탐색(findElement) : 이진 탐색을 이용하면 O(log n) 시간에 수행
- 삽입(insertItem) : 새로운 항목을 삽입하기 위한 공간 확보를 위해 최악의 경우 n개의 기존 항목들을 이동 → O(n) 시간을 소요
- 삭제(removeElement) : 항목이 삭제된 공간을 메꾸기 위해 최악의 경우 n개의 기존 항목들을 이동 → O(n)
- 탐색이 빠르지만 삽입과 삭제가 느림 → 삽입과 삭제가 빈번하지 않은 경우 사용

- findElement(k)
    
    ```
    Alg findElement(k)
    	input list L, key k
    	output element with key k
    
    1. L.initialize(i)
    2. while (L.isValid(i))
    			if (L.key(i) = k)
    					return L.element(i)
    			elseif (L.key(i) > k)
    					return NoSuchKey
    			else
    					L.advance(i)
    3. return NoSuchKey
    ```
    
    - 무순 리스트 → 키가 나타날때까지 마지막 데이터 항목까지 탐색
    - 순서 리스트 → 키 순서로 저장되어 있으므로 키가 나타나야할 지점을 지나친 순간 탐색 실패로 함. 근데 점근적으로는 O(n)

### 이진 탐색(binary search)

- 키로 정렬되고 배열에 기초한 리스트로 구현된 사전에 대한 탐색 작업을 수행

- findElement(k), rFindElement(k,l,r)
    
    ```
    Alg findElement(k)
    	input sorted array A[0..n-1], key k
    	output element with key k
    	
    1. return rFindElement(k,0,n-1)
    
    Alg rFindElement(k,l,r) {recursive}
    1. if (l > r)
    				return NoSuchKey
    2. mid <- (l+r)/2
    3. if (k = key(A[mid]))
    				return element(A[mid])
    		elseif (k < key(A[mid]))
    				return rFindElement(k,l,mid-1)
    		elseif (k > key(A[mid]))
    ```
    

- 예시

7을 찾는다고 가정

| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 0 | 1 | 3 | 4 | 5 | 7 | 8 | 9 | 11 | 14 | 16 | 18 | 19 |
| l |  |  |  |  |  | mid |  |  |  |  |  | r |

7과 mid에 해당하는 값을 비교했을 때 7보다 크므로 왼쪽으로 범위를 수정( r = mid -1 )

| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 0 | 1 | 3 | 4 | 5 | 7 | 8 | 9 | 11 | 14 | 16 | 18 | 19 |
| l |  | mid |  |  | r |  |  |  |  |  |  |  |

이번에도 7과 mid에 해당하는 값을 비교하면 7이 더 크므로 오른쪽으로 범위를 수정 (l = mid +1)

| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 0 | 1 | 3 | 4 | 5 | 7 | 8 | 9 | 11 | 14 | 16 | 18 | 19 |
|  |  |  | l | mid | r |  |  |  |  |  |  |  |

이번에도 같은 방법을 수행 → l = mid +1

| 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 | 10 | 11 | 12 |
| --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- | --- |
| 0 | 1 | 3 | 4 | 5 | 7 | 8 | 9 | 11 | 14 | 16 | 18 | 19 |
|  |  |  |  |  | l = mid =  r |  |  |  |  |  |  |  |

- 성능
    
    순서리스트
    
    - 배열로 구현되었을 때 최악의 경우 → O(log n)
    - 연결리스트로 구현된 경우 → 이진 탐색으로 얻는 이득이 없음( 가운데 위치로 접근하는데만 O(n) 시간 소요됨)