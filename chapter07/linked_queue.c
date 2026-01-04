#include <stdio.h>
#include <stdlib.h>

/* ============================================================
 * 노드 및 큐 구조체 정의
 * ============================================================ */
typedef int element;

typedef struct QueueNode {
    element data;
    struct QueueNode *prev;
    struct QueueNode *next;
} QueueNode;

typedef struct {
    QueueNode *front;
    QueueNode *rear;
} QueueType;

/* ============================================================
 * 에러 처리 함수
 * ============================================================ */
void error(const char *message) {
    fprintf(stderr, "[ERROR] %s\n", message);
    exit(1);
}

/* ============================================================
 * 기본 연산
 * ============================================================ */

/* 큐 초기화
 * Time Complexity: O(1)
 */
void init_queue(QueueType *q) {
    q->front = NULL;
    q->rear = NULL;
}

/* 큐가 비었는지 확인
 * Time Complexity: O(1)
 */
int is_empty(QueueType *q) {
    return q->front == NULL;
}

/* 큐가 가득 찼는지 확인
 * Time Complexity: O(1)
 * - 동적 할당이므로 항상 0 반환 (가득 찰 일 없음)
 */
int is_full(QueueType *q) {
    return 0;
}

/* ============================================================
 * 삽입 연산
 * ============================================================ */

/* 큐에 원소 삽입 (enqueue)
 * Time Complexity: O(1)
 * - rear에 노드 삽입
 */
void enqueue(QueueType *q, element item) {
    QueueNode *new_node = (QueueNode *)malloc(sizeof(QueueNode));
    if (new_node == NULL) {
        error("Memory allocation failed");
    }
    new_node->data = item;
    new_node->next = NULL;
    new_node->prev = NULL;

    if (is_empty(q)) {
        /* 빈 큐: front와 rear가 새 노드를 가리킴 */
        q->front = new_node;
        q->rear = new_node;
    } else {
        /* 기존 rear 뒤에 새 노드 연결 */
        q->rear->next = new_node;
        new_node->prev = q->rear;
        q->rear = new_node;
    }
}

/* ============================================================
 * 삭제 연산
 * ============================================================ */

/* 큐에서 원소 삭제 및 반환 (dequeue)
 * Time Complexity: O(1)
 * - front 노드 삭제 후 데이터 반환
 */
element dequeue(QueueType *q) {
    QueueNode *removed;
    element data;

    if (is_empty(q)) {
        error("Queue is empty");
    }

    removed = q->front;
    data = removed->data;

    q->front = removed->next;  /* front가 다음 노드를 가리킴 */

    if (q->front == NULL) {
        /* 마지막 노드였을 경우 */
        q->rear = NULL;
    } else {
        q->front->prev = NULL;  /* 새 front의 prev를 NULL로 */
    }

    free(removed);
    return data;
}

/* ============================================================
 * 조회 연산
 * ============================================================ */

/* 큐의 front 원소 반환 (삭제 없음)
 * Time Complexity: O(1)
 */
element peek(QueueType *q) {
    if (is_empty(q)) {
        error("Queue is empty");
    }
    return q->front->data;
}

/* ============================================================
 * 유틸리티 연산
 * ============================================================ */

/* 큐의 모든 원소 삭제
 * Time Complexity: O(n) where n = 큐 크기
 */
void clear(QueueType *q) {
    QueueNode *current = q->front;
    QueueNode *next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    q->front = NULL;
    q->rear = NULL;
}

/* ============================================================
 * 시간 복잡도 요약
 * ============================================================
 * Operation    | Time Complexity | Notes
 * -------------|-----------------|------------------------
 * init_queue   | O(1)            |
 * is_empty     | O(1)            |
 * is_full      | O(1)            | 항상 0 반환 (동적 할당)
 * enqueue      | O(1)            | rear에 삽입
 * dequeue      | O(1)            | front 삭제 후 반환
 * peek         | O(1)            | front 조회 (미삭제)
 * clear        | O(n)            | 전체 삭제
 *
 * * 연결 리스트 기반 큐의 장점:
 *   - 크기 제한 없음 (동적 할당)
 *   - enqueue, dequeue 모두 O(1)
 *   - is_full 불필요
 *   - front/rear 유지로 효율적
 *
 * * 단점:
 *   - malloc/free 오버헤드
 *   - 캐시 비친화적
 *   - 포인터 추가 메모리
 * ============================================================ */

/* ============================================================
 * 메인 함수
 * ============================================================ */
int main(void) {
    QueueType q;

    printf("=== 연결 리스트 기반 큐 (Linked Queue) ===\n\n");

    init_queue(&q);
    printf("초기화: is_empty = %d, is_full = %d\n\n", is_empty(&q), is_full(&q));

    // enqueue 테스트
    printf(">> enqueue(10), enqueue(20), enqueue(30)\n");
    enqueue(&q, 10);
    enqueue(&q, 20);
    enqueue(&q, 30);
    printf("peek: %d\n", peek(&q));
    printf("is_empty: %d\n\n", is_empty(&q));

    // dequeue 테스트
    printf(">> dequeue() = %d\n", dequeue(&q));
    printf(">> dequeue() = %d\n", dequeue(&q));
    printf("peek: %d\n\n", peek(&q));

    // enqueue 추가
    printf(">> enqueue(40), enqueue(50)\n");
    enqueue(&q, 40);
    enqueue(&q, 50);
    printf("peek: %d\n\n", peek(&q));

    // 전체 dequeue
    printf(">> 전체 dequeue:\n");
    while (!is_empty(&q)) {
        printf("dequeue: %d\n", dequeue(&q));
    }
    printf("\n");

    printf("is_empty: %d\n", is_empty(&q));

    return 0;
}
