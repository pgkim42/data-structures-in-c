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
└── CMakeLists.txt
```

## 목차

| 장  | 주제           | 상태 |
|----|--------------|------|
| 01 | 자료구조와 알고리즘   | ✅   |
| 02 | 순환           | ✅   |
| 03 | 배열, 구조체, 포인터 | ✅   |
| 04 | 스택           | ✅   |
| 05 | 큐            | ✅   |
| 06 | 연결 리스트 I     | ✅   |
  

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

## 빌드

```bash
mkdir build && cd build
cmake ..
make
```
