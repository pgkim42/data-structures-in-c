#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 5

// 큐 요소의 타입 (필요시 변경 가능)
typedef int element;

// 선형 큐 구조체
typedef struct {
    int front;  // 삭제 위치 (전단)
    int rear;   // 삽입 위치 (후단)
    element data[MAX_QUEUE_SIZE];
} QueueType;

/* ========== 유틸리티 함수 ========== */

// 오류 메시지를 출력하고 프로그램을 종료한다
void error(char *message) {
    fprintf(stderr, "%s\n", message);
    exit(1);
}

/* ========== 기본 연산 함수 ========== */

// 큐를 초기화한다
void init_queue(QueueType *q) {
    q->front = -1;
    q->rear = -1;
}

// 큐의 현재 상태를 시각화한다
void queue_print(QueueType *q) {
    printf("[");
    for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
        if (i <= q->front || i > q->rear) {
            printf("   ");
        } else {
            printf("%3d", q->data[i]);
        }
        if (i < MAX_QUEUE_SIZE - 1) printf("|");
    }
    printf("] ");

    // front, rear 위치 표시
    for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
        if (i == q->front && i == q->rear) {
            printf("  F=R");
        } else if (i == q->front) {
            printf("   F");
        } else if (i == q->rear) {
            printf("   R");
        } else {
            printf("    ");
        }
    }
    printf(" (front=%d, rear=%d)\n", q->front, q->rear);
}

// 큐가 포화상태인지 확인한다
int is_full(QueueType *q) {
    return q->rear == MAX_QUEUE_SIZE - 1;
}

// 큐가 공백상태인지 확인한다
int is_empty(QueueType *q) {
    return q->front == q->rear;
}

// 큐의 후단에 원소를 삽입한다 (enqueue)
void enqueue(QueueType *q, int item) {
    if (is_full(q)) {
        error("큐가 포화상태입니다.");
    }
    q->data[++(q->rear)] = item;
}

// 큐의 전단 원소를 삭제하고 반환한다 (dequeue)
int dequeue(QueueType *q) {
    if (is_empty(q)) {
        error("큐가 공백상태입니다.");
    }
    int item = q->data[++(q->front)];
    return item;
}

/* ========== 메인 함수 ========== */

int main(void) {
    int item = 0;
    QueueType q;

    init_queue(&q);

    // 삽입 연산
    enqueue(&q, 10); queue_print(&q);
    enqueue(&q, 20); queue_print(&q);
    enqueue(&q, 30); queue_print(&q);

    // 삭제 연산
    item = dequeue(&q); queue_print(&q);
    item = dequeue(&q); queue_print(&q);
    item = dequeue(&q); queue_print(&q);

    return 0;
}
