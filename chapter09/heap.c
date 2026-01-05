/**
 * Chapter 09: 힙 (Heap)
 *
 * Max-Heap: 부모 노드가 항상 자식 노드보다 크거나 같은 완전 이진 트리
 *
 *          90
 *        /    \
 *      60      80
 *     /  \    /  \
 *   30   50  70   75
 *
 * 배열 표현 (0-based 인덱스):
 * [90, 60, 80, 30, 50, 70, 75]
 *
 * 인덱스 관계:
 * - 부모: (i - 1) / 2
 * - 왼쪽 자식: 2*i + 1
 * - 오른쪽 자식: 2*i + 2
 *
 * 시간 복잡도:
 * - 삽입 (insert_heap): O(log n) - 상향 이동 (up-heap)
 * - 삭제 (delete_heap): O(log n) - 하향 이동 (down-heap)
 * - 최댓값 찾기 (find_max): O(1)
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_HEAP_SIZE 100

// 힙 요소의 타입 (필요시 변경 가능)
typedef int element;

// 힙 구조체
typedef struct {
    element heap[MAX_HEAP_SIZE];
    int heap_size;  // 현재 요소 개수
} Heap;


// ==================== 유틸리티 함수 ====================

/**
 * 오류 메시지를 출력하고 프로그램을 종료한다
 */
void error(char *message) {
    fprintf(stderr, "%s\n", message);
    exit(1);
}


// ==================== 기본 연산 ====================

/**
 * 힙을 초기화한다
 */
void init_heap(Heap *h) {
    h->heap_size = 0;
}

/**
 * 힙이 공백상태인지 확인한다
 * @return 공백이면 1, 아니면 0
 */
int is_empty(Heap *h) {
    return h->heap_size == 0;
}

/**
 * 힙이 포화상태인지 확인한다
 * @return 포화상태이면 1, 아니면 0
 */
int is_full(Heap *h) {
    return h->heap_size == MAX_HEAP_SIZE;
}

/**
 * 힙의 내용을 출력한다
 */
void heap_print(Heap *h) {
    printf("[");
    for (int i = 0; i < h->heap_size; i++) {
        printf("%d", h->heap[i]);
        if (i < h->heap_size - 1) {
            printf(", ");
        }
    }
    printf("] (size=%d)\n", h->heap_size);
}


// ==================== 의사코드: 삽입 ====================

/*
ALGORITHM insert_heap(h, item)
  IF is_full(h) THEN
    error "힙이 포화상태입니다."
  END IF

  i ← h.heap_size           // 삽입할 위치
  h.heap_size ← h.heap_size + 1

  // 상향 이동 (up-heap): 부모보다 크면 교환
  WHILE (i > 0 AND item > h.heap[(i-1)/2]) DO
    h.heap[i] ← h.heap[(i-1)/2]  // 부모를 아래로 이동
    i ← (i - 1) / 2              // 부모 인덱스로 이동
  END WHILE

  h.heap[i] ← item            // 최종 위치에 삽입
*/

/**
 * 힙에 요소를 삽입한다 (상향 이동)
 * @param h 힙 포인터
 * @param item 삽입할 요소
 */
void insert_heap(Heap *h, element item) {
    if (is_full(h)) {
        error("힙이 포화상태입니다.");
    }

    int i = h->heap_size;  // 삽입할 위치
    h->heap_size++;

    // 상향 이동: 부모보다 크면 부모를 아래로 내림
    while (i > 0 && item > h->heap[(i - 1) / 2]) {
        h->heap[i] = h->heap[(i - 1) / 2];  // 부모를 아래로 이동
        i = (i - 1) / 2;                    // 부모 인덱스로 이동
    }

    h->heap[i] = item;  // 최종 위치에 삽입
}


// ==================== 의사코드: 삭제 ====================

/*
ALGORITHM delete_heap(h)
  IF is_empty(h) THEN
    error "힙이 공백상태입니다."
  END IF

  root ← h.heap[0]           // 최댓값 저장
  last ← h.heap[h.heap_size-1]  // 마지막 요소
  h.heap_size ← h.heap_size - 1

  parent ← 0

  // 하향 이동 (down-heap)
  WHILE (true) DO
    child ← 2 * parent + 1   // 왼쪽 자식

    // 오른쪽 자식이 존재하고 더 크면 오른쪽 선택
    IF (child + 1 < h.heap_size AND
        h.heap[child] < h.heap[child + 1]) THEN
      child ← child + 1
    END IF

    // 자식이 없거나 last가 더 크면 종료
    IF (child >= h.heap_size OR last >= h.heap[child]) THEN
      BREAK
    END IF

    h.heap[parent] ← h.heap[child]  // 자식을 위로 이동
    parent ← child
  END WHILE

  h.heap[parent] ← last
  RETURN root
*/

/**
 * 힙에서 최댓값을 삭제하고 반환한다 (하향 이동)
 * @param h 힙 포인터
 * @return 삭제된 최댓값
 */
element delete_heap(Heap *h) {
    if (is_empty(h)) {
        error("힙이 공백상태입니다.");
    }

    element root = h->heap[0];  // 최댓값 저장
    element last = h->heap[h->heap_size - 1];  // 마지막 요소
    h->heap_size--;

    int parent = 0;
    int child;

    // 하향 이동
    while (1) {
        child = 2 * parent + 1;  // 왼쪽 자식

        // 오른쪽 자식이 존재하고 더 크면 오른쪽 선택
        if (child + 1 < h->heap_size &&
            h->heap[child] < h->heap[child + 1]) {
            child++;
        }

        // 자식이 없거나 last가 더 크면 종료
        if (child >= h->heap_size || last >= h->heap[child]) {
            break;
        }

        h->heap[parent] = h->heap[child];  // 자식을 위로 이동
        parent = child;
    }

    h->heap[parent] = last;

    return root;
}


// ==================== 추가 연산 ====================

/**
 * 최댓값을 반환한다 (삭제 없음)
 * @param h 힙 포인터
 * @return 최댓값
 */
element find_max(Heap *h) {
    if (is_empty(h)) {
        error("힙이 공백상태입니다.");
    }
    return h->heap[0];
}


// ==================== 테스트 코드 ====================

int main(void) {
    printf("========== Max-Heap ==========\n\n");

    Heap heap;
    init_heap(&heap);

    // ========== 삽입 테스트 ==========
    printf("========== 삽입 테스트 ==========\n");
    int insert_data[] = {90, 60, 80, 30, 50, 70, 75};
    int n = sizeof(insert_data) / sizeof(insert_data[0]);

    for (int i = 0; i < n; i++) {
        insert_heap(&heap, insert_data[i]);
        printf("삽입: %d\t→ ", insert_data[i]);
        heap_print(&heap);
    }
    printf("\n");

    // ========== 최댓값 찾기 ==========
    printf("========== 최댓값 찾기 ==========\n");
    printf("최댓값: %d\n\n", find_max(&heap));

    // ========== 삭제 테스트 ==========
    printf("========== 삭제 테스트 ==========\n");
    while (!is_empty(&heap)) {
        element max = delete_heap(&heap);
        printf("삭제: %d\t→ ", max);
        heap_print(&heap);
    }

    return 0;
}
