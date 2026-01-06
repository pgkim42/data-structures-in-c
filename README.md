# 자료구조 학습

**C언어로 쉽게 풀어쓴 자료구조** (천인국, 공용해, 하상호 공저 / 생능출판사) 책으로 공부하며 작성하는 예제 코드입니다.

- 📖 [교보문고 책 보기](https://product.kyobobook.co.kr/detail/S000001076349)
- 📖 [Yes24 책 보기](https://www.yes24.com/product/goods/69750539)

## 구조

```
.
├── chapter02/  # 순환 (Recursion)
├── chapter03/  # 배열, 구조체, 포인터
├── chapter04/  # 스택
├── chapter05/  # 큐 (Queue)
├── chapter06/  # 연결 리스트 I
├── chapter07/  # 연결 리스트 II
├── chapter08/  # 트리 (Tree)
└── CMakeLists.txt
```

## 목차

| 장  | 주제           | 상태 |
|----|--------------|----|
| 01 | 자료구조와 알고리즘   | ✅  |
| 02 | 순환           | ✅  |
| 03 | 배열, 구조체, 포인터 | ✅  |
| 04 | 스택           | ✅  |
| 05 | 큐            | ✅  |
| 06 | 연결 리스트 I     | ✅  |
| 07 | 연결 리스트 II    | ✅  |
| 08 | 트리           | ✅  |
| 09 | 우선순위 큐       | ✅  |
| 10 | 그래프          | -  |


## Chapter 02: 순환 (Recursion)

- **main.c**: 팩토리얼, 거듭제곱, 피보나치, 하노이의 탑
  - 재귀 vs 반복 비교
  - 시간 복잡도: O(2^n) → O(log n) 최적화

## Chapter 03: 배열, 구조체, 포인터

- **structure.c**: 학생 구조체 정의 및 출력
- **swap.c**: 포인터와 call by reference
- **array1.c**: 배열과 포인터 관계
- **malloc.c / malloc2.c**: 동적 메모리 할당
- **polynomial1.c / polynomial2.c**: 다항식 덧셈 (배열 vs 희소)
- **matrix1.c / matrix2.c**: 행렬 전치 (일반 vs 희소)
- **quiz_01.c / quiz_02.c**: 구조체, 다항식 연습 문제

## Chapter 04: 스택 (Stack)

### 기본 구현
- **stack_integer.c**: 전역 변수로 구현한 정수 스택
- **stack_struct_array.c**: 구조체(학생 정보)를 저장하는 스택
- **stack_array.c**: 캡슐화된 배열 스택
- **stack_dynamic_array.c**: 동적 배열 스택 (자동 용량 확장)

### 응용 문제
- **bracket_checker.c**: 괄호 짝 검사 `()`, `[]`, `{}`
- **postfix_eval.c**: 후위 표기 수식 계산
- **infix_to_postfix.c**: 중위 표기 → 후위 표기 변환
- **maze.c**: 스택을 이용한 미로찾기 (백트래킹)

## Chapter 05: 큐 (Queue)

### 문서
- **ADT_Queue.md**: 큐 추상 데이터 타입 명세
- **ADT_Deque.md**: 덱 추상 데이터 타입 명세

### 기본 구현
- **linear_queue.c**: 선형 큐 (배열 기반, front 락 발생)
- **circular_queue.c**: 원형 큐 (순환으로 공간 효율 개선)

### 응용 문제
- **circular_queue_sim.c**: 난수 기반 큐 시뮬레이션 (20% 삽입, 10% 삭제)
- **deque.c**: 덱 (Double-Ended Queue) - 양쪽 끝에서 삽입/삭제 가능
- **bank_simulation.c**: 은행 서비스 시뮬레이션 (다중 서버 큐, 창구 2개)

## Chapter 06: 연결 리스트 I

### 배열 기반 연결 리스트
- **linked_list_array.c**: 포인터 대신 배열 인덱스로 노드 연결
  - 가용 리스트(Free List)로 메모리 관리: alloc_node, free_node
  - 연산: init, is_empty, is_full, get_entry, insert, insert_last, delete, get_length, clear, replace
  - 시간 복잡도: 삽입/삭제 O(n), 헤드 삽입/삭제 O(1)
  - 장점: 메모리 할당 O(1), 단편화 없음, 캐시 친화적

### 단일 연결 리스트 (Singly Linked List)
- **singly_linked_list.c**: 동적 메모리 할당 기반 단일 연결 리스트
  - ListNode 구조체 (data, link), ListType 구조체 (head, length)
  - 연산: init, is_empty, get_length, insert_first, insert, delete_first, delete, get_entry, print_list, clear
  - 탐색: search_list - 특정 값 선형 탐색 O(n)
  - 병합: concat_list - 두 리스트 연결 O(n)
  - 변환: reverse - 리스트 역순 (in-place, O(n))

### 다항식 연결 리스트
- **polynomial_linked_list.c**: 연결 리스트로 다항식 표현
  - PolyNode 구조체 (coef, expon, link)
  - 차수 내림차순 자동 정렬 유지
  - 연산: add_term, remove_term, get_term, get_degree, simplify
  - 다항식 연산:
    - add_poly: 두 다항식 덧셈 O(n+m), merge 방식
    - mult_poly: 두 다항식 곱셈 O(n×m), 이중 루프
    - eval_poly: x값 대입 계산 O(n)
  - 출력: 3x^6 + 7x^3 - 2x^2 + 5 형태

## Chapter 07: 연결 리스트 II

### 원형 연결 리스트 (Circular Linked List)
- **circular_linked_list.c**: 마지막 노드가 첫 노드를 가리키는 원형 구조
  - tail 포인터만 유지 (tail->link가 head)
  - 연산: init, is_empty, get_length, insert_first, insert_last, insert, delete_first, delete_last, delete, get_entry, replace, search, print_list, clear
  - 시간 복잡도: insert_first O(1), insert_last O(1), delete_first O(1), delete_last O(n)
  - 경계 조건: 빈 리스트 (tail==NULL), 단일 노드 (tail->link==tail)
  - 응용: CPU 스케줄링 (Round Robin)

### 이중 연결 리스트 (Doubly Linked List)
- **doubly_linked_list.c**: 양방향 링크를 가진 연결 리스트
  - DListNode 구조체 (prev, next)
  - 연산: init, is_empty, get_length, insert_first, insert_last, insert, delete_first, delete_last, delete, get_entry, replace, search, print_list, print_reverse, reverse, clear
  - 시간 복잡도: insert_first/last O(1), delete_first/last O(1), reverse O(1)
  - 장점: 양방향 순회, 역방향 출력, delete_last O(1), reverse O(1) (head, tail만 swap)

### 연결 리스트 기반 스택 (Linked Stack)
- **linked_stack.c**: 단일 연결 리스트로 구현한 스택
  - StackNode 구조체 (data, link), StackType (top)
  - 연산: init_stack, is_empty, is_full(항상 0), push, pop, peek, clear
  - 시간 복잡도: 모든 연산 O(1)
  - 장점: 크기 제한 없음, 동적 할당

### 연결 리스트 기반 큐 (Linked Queue)
- **linked_queue.c**: 이중 연결 리스트로 구현한 큐
  - QueueNode 구조체 (prev, next), QueueType (front, rear)
  - 연산: init_queue, is_empty, is_full(항상 0), enqueue, dequeue, peek, clear
  - 시간 복잡도: 모든 연산 O(1)
  - 장점: enqueue/dequeue 모두 O(1), 크기 제한 없음

## Chapter 08: 트리 (Tree)

### 이진 트리 (Binary Tree)
- **binary_tree.c**: 포인터 기반 이진 트리
  - TreeNode 구조체 (data, left, right)
  - 기본 연산: create_node, is_empty, set_left/right, get_left/right
  - 순회 (Traversal): preorder, inorder, postorder
  - 반복적 순회: preorder_iterative (스택 사용)
  - 레벨 순회: level_order (큐 사용, BFS)
  - 유틸리티: get_node_count, get_height, get_leaf_count
  - 고급 연산: copy_tree (트리 복사), is_equal (동일성 검사)
  - 시간 복잡도: 순회 O(n), 탐색 O(n)

### 수식 트리 (Expression Tree)
- **expression_tree.c**: 산술 표현식을 트리로 표현
  - ExprNode 구조체 (op, is_operator, left, right)
  - make_expression_tree: 후위 표기식으로부터 트리 생성
  - evaluate: 수식 트리 계산 (후위 순회)
  - 표기법 출력: print_infix, print_prefix, print_postfix
  - 예: (3+4)*(5-2) = 21, 후위 표기식 "34+52-*"

### 이진 탐색 트리 (Binary Search Tree)
- **binary_search_tree.c**: BST 속성 (왼쪽 < 루트 < 오른쪽)
  - BSTNode 구조체 (key, left, right)
  - 연산: bst_insert (삽입), bst_search (탐색), bst_delete (삭제)
  - 삭제 3가지 케이스: 리프, 자식 1개, 자식 2개 (후계자 대체)
  - 보조 연산: bst_find_min, bst_find_max
  - 중위 순회 시 오름차순 정렬 출력
  - 시간 복잡도: 평균 O(log n), 최악 O(n)

## Chapter 09: 우선순위 큐 (Priority Queue)

### 문서
- **ADT_Priority_Queue.md**: 우선순위 큐 추상 데이터 타입 명세

### 기본 구현
- **heap.c**: Max-Heap 기반 우선순위 큐
  - 완전 이진 트리를 배열로 구현 (0-based 인덱싱)
  - 연산: insert_heap O(log n), delete_heap O(log n), find_max O(1)
  - 인덱스 관계: 부모 (i-1)/2, 왼쪽 자식 2i+1, 오른쪽 자식 2i+2

### 응용 알고리즘
- **heap_sort.c**: 힙 정렬 O(n log n)
  - 제자리 정렬(in-place), 추가 메모리 불필요
  - 최대 힙构建 후 루트를 반복적으로 추출

- **lpt_scheduling.c**: LPT (Longest Processing Time) 스케줄링
  - Min-Heap으로 가장 한가한 머신 선택 O(n log m)
  - 작업 시간이 긴 순서대로 할당하여 makespan 최소화

- **huffman.c**: 허프만 코딩 (무손실 압축)
  - Min-Heap으로 빈도가 가장 낮은 두 기호 선택 O(n + k log k)
  - 접두사 코드(prefix code) 생성, 최적 압축

## 빌드

```bash
mkdir build && cd build
cmake ..
make
```
