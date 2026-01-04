#include <stdio.h>
#include <stdlib.h>

#define MAX_QUEUE_SIZE 5  // 실제 사용 가능한 공간은 4

typedef int element;

// 덱 구조체
typedef struct {
    int front;  // 전단 (삭제 위치)
    int rear;   // 후단 (삽입 위치)
    element data[MAX_QUEUE_SIZE];
} DequeType;

/* ========== 유틸리티 함수 ========== */

void error(char *message) {
    fprintf(stderr, "%s\n", message);
    exit(1);
}

/* ========== 기본 연산 함수 ========== */

// 덱을 초기화한다
void init_deque(DequeType *dq) {
    dq->front = 0;
    dq->rear = 0;
}

// 덱의 현재 상태를 시각화한다
void deque_print(DequeType *dq) {
    printf("[");
    for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
        int is_valid = 0;
        if (dq->front < dq->rear) {
            if (i > dq->front && i <= dq->rear) {
                is_valid = 1;
            }
        } else if (dq->front > dq->rear) {
            if (i > dq->front || i <= dq->rear) {
                is_valid = 1;
            }
        }

        if (is_valid) {
            printf("%3d", dq->data[i]);
        } else {
            printf("   ");
        }
        if (i < MAX_QUEUE_SIZE - 1) printf("|");
    }
    printf("] ");

    // front, rear 위치 표시
    for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
        if (i == dq->front && i == dq->rear) {
            printf("  F=R");
        } else if (i == dq->front) {
            printf("   F");
        } else if (i == dq->rear) {
            printf("   R");
        } else {
            printf("    ");
        }
    }
    printf(" (front=%d, rear=%d)\n", dq->front, dq->rear);
}

// 덱이 포화상태인지 확인한다
int is_full(DequeType *dq) {
    return (dq->rear + 1) % MAX_QUEUE_SIZE == dq->front;
}

// 덱이 공백상태인지 확인한다
int is_empty(DequeType *dq) {
    return dq->front == dq->rear;
}

// 전단에 원소를 삽입한다 (add_front)
void add_front(DequeType *dq, int item) {
    if (is_full(dq)) {
        error("덱이 포화상태입니다.");
    }
    dq->data[dq->front] = item;
    dq->front = (dq->front - 1 + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
}

// 후단에 원소를 삽입한다 (add_rear)
void add_rear(DequeType *dq, int item) {
    if (is_full(dq)) {
        error("덱이 포화상태입니다.");
    }
    dq->rear = (dq->rear + 1) % MAX_QUEUE_SIZE;
    dq->data[dq->rear] = item;
}

// 전단 원소를 삭제하고 반환한다 (delete_front)
int delete_front(DequeType *dq) {
    if (is_empty(dq)) {
        error("덱이 공백상태입니다.");
    }
    dq->front = (dq->front + 1) % MAX_QUEUE_SIZE;
    return dq->data[dq->front];
}

// 후단 원소를 삭제하고 반환한다 (delete_rear)
int delete_rear(DequeType *dq) {
    if (is_empty(dq)) {
        error("덱이 공백상태입니다.");
    }
    int item = dq->data[dq->rear];
    dq->rear = (dq->rear - 1 + MAX_QUEUE_SIZE) % MAX_QUEUE_SIZE;
    return item;
}

// 전단 원소를 확인한다 (get_front)
int get_front(DequeType *dq) {
    if (is_empty(dq)) {
        error("덱이 공백상태입니다.");
    }
    return dq->data[(dq->front + 1) % MAX_QUEUE_SIZE];
}

// 후단 원소를 확인한다 (get_rear)
int get_rear(DequeType *dq) {
    if (is_empty(dq)) {
        error("덱이 공백상태입니다.");
    }
    return dq->data[dq->rear];
}

/* ========== 메인 함수 ========== */

int main(void) {
    DequeType dq;

    init_deque(&dq);

    printf("==== 후단 삽입 (add_rear) ====\n");
    add_rear(&dq, 10); deque_print(&dq);
    add_rear(&dq, 20); deque_print(&dq);
    add_rear(&dq, 30); deque_print(&dq);

    printf("\n==== 전단 삽입 (add_front) ====\n");
    add_front(&dq, 5); deque_print(&dq);
    add_front(&dq, 1); deque_print(&dq);  // 여기서 포화상태

    printf("\n==== 전단 삭제 (delete_front) ====\n");
    printf("delete_front: %d\n", delete_front(&dq)); deque_print(&dq);
    printf("delete_front: %d\n", delete_front(&dq)); deque_print(&dq);

    printf("\n==== 후단 삭제 (delete_rear) ====\n");
    printf("delete_rear: %d\n", delete_rear(&dq)); deque_print(&dq);
    printf("delete_rear: %d\n", delete_rear(&dq)); deque_print(&dq);

    printf("\n==== get_front / get_rear ====\n");
    printf("get_front: %d\n", get_front(&dq));
    printf("get_rear: %d\n", get_rear(&dq));
    deque_print(&dq);

    return 0;
}
