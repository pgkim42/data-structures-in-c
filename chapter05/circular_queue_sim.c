#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_QUEUE_SIZE 5
#define SIMULATION_COUNT 100

typedef int element;

typedef struct {
    int front;
    int rear;
    element data[MAX_QUEUE_SIZE];
} QueueType;

/* ========== 큐 연산 함수 ========== */

void init_queue(QueueType *q) {
    q->front = 0;
    q->rear = 0;
}

int is_full(QueueType *q) {
    return (q->rear + 1) % MAX_QUEUE_SIZE == q->front;
}

int is_empty(QueueType *q) {
    return q->front == q->rear;
}

void enqueue(QueueType *q, int item) {
    if (is_full(q)) {
        printf("[포화] enqueue(%d) 실패\n", item);
        return;
    }
    q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
    q->data[q->rear] = item;
}

int dequeue(QueueType *q) {
    if (is_empty(q)) {
        printf("[공백] dequeue 실패\n");
        return -1;
    }
    q->front = (q->front + 1) % MAX_QUEUE_SIZE;
    return q->data[q->front];
}

// 큐의 현재 상태를 출력한다
void queue_print(QueueType *q) {
    printf("[");
    for (int i = 0; i < MAX_QUEUE_SIZE; i++) {
        int is_valid = 0;
        if (q->front < q->rear) {
            if (i > q->front && i <= q->rear) {
                is_valid = 1;
            }
        } else if (q->front > q->rear) {
            if (i > q->front || i <= q->rear) {
                is_valid = 1;
            }
        }

        if (is_valid) {
            printf("%3d", q->data[i]);
        } else {
            printf("   ");
        }
        if (i < MAX_QUEUE_SIZE - 1) printf("|");
    }
    printf("] (size=%d)\n", get_size(q));
}

// 큐의 현재 크기를 반환한다
int get_size(QueueType *q) {
    if (q->rear >= q->front) {
        return q->rear - q->front;
    }
    return MAX_QUEUE_SIZE - q->front + q->rear;
}

/* ========== 메인 함수 ========== */

int main(void) {
    QueueType q;
    int enqueue_count = 0;
    int dequeue_count = 0;
    int fail_count = 0;

    // 난수 생성기 초기화
    srand((unsigned int)time(NULL));

    init_queue(&q);

    printf("========== 큐 시뮬레이션 시작 ==========\n");
    printf("- 삽입 확률: 20%% (0~19)\n");
    printf("- 삭제 확률: 10%% (20~29)\n");
    printf("- 나머지: 아무 동작 안 함 (50%%)\n");
    printf("======================================\n\n");

    for (int i = 0; i < SIMULATION_COUNT; i++) {
        int probability = rand() % 100;  // 0~99

        printf("[%2d] ", i);

        if (probability < 20) {
            // 20% 확률: enqueue (난수 1~100 생성)
            int item = rand() % 100 + 1;
            int old_size = get_size(&q);
            enqueue(&q, item);
            if (get_size(&q) > old_size) {
                printf("enqueue(%d) ", item);
                enqueue_count++;
            }
        }
        else if (probability < 30) {
            // 10% 확률: dequeue
            int old_size = get_size(&q);
            int item = dequeue(&q);
            if (get_size(&q) < old_size) {
                printf("dequeue(%d) ", item);
                dequeue_count++;
            } else {
                fail_count++;
            }
        }
        else {
            // 70% 확률: 아무 동작 안 함
            printf("--- ");
        }

        // 10번마다 또는 마지막에 상태 출력
        if (i % 10 == 9 || i == SIMULATION_COUNT - 1) {
            queue_print(&q);
        } else {
            printf("\n");
        }
    }

    printf("\n========== 시뮬레이션 결과 ==========\n");
    printf("총 enqueue: %d회\n", enqueue_count);
    printf("총 dequeue: %d회\n", dequeue_count);
    printf("실패 횟수: %d회\n", fail_count);
    printf("최종 큐 크기: %d\n", get_size(&q));
    printf("====================================\n");

    return 0;
}
