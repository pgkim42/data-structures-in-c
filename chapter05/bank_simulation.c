#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_QUEUE_SIZE 100
#define TELLERS 2  // 은행원(창구) 수

// 고객 정보 구조체
typedef struct {
    int id;             // 고객 번호
    int arrival_time;   // 도착 시각
    int service_time;   // 서비스 소요 시간
} Customer;

// 큐 구조체
typedef struct {
    int front;
    int rear;
    Customer data[MAX_QUEUE_SIZE];
} QueueType;

/* ========== 유틸리티 함수 ========== */

void error(char *message) {
    fprintf(stderr, "%s\n", message);
    exit(1);
}

/* ========== 기본 연산 함수 ========== */

// 큐를 초기화한다
void init_queue(QueueType *q) {
    q->front = 0;
    q->rear = 0;
}

// 큐가 포화상태인지 확인한다
int is_full(QueueType *q) {
    return (q->rear + 1) % MAX_QUEUE_SIZE == q->front;
}

// 큐가 공백상태인지 확인한다
int is_empty(QueueType *q) {
    return q->front == q->rear;
}

// 큐에 고객을 삽입한다
void enqueue(QueueType *q, Customer customer) {
    if (is_full(q)) {
        error("큐가 포화상태입니다.");
    }
    q->rear = (q->rear + 1) % MAX_QUEUE_SIZE;
    q->data[q->rear] = customer;
}

// 큐에서 고객을 꺼낸다
Customer dequeue(QueueType *q) {
    if (is_empty(q)) {
        error("큐가 공백상태입니다.");
    }
    q->front = (q->front + 1) % MAX_QUEUE_SIZE;
    return q->data[q->front];
}

/* ========== 메인 함수 ========== */

int main(void) {
    int minutes = 60;                    // 시뮬레이션 총 시간 (분)
    int total_wait = 0;                  // 전체 대기 시간
    int total_customers = 0;             // 전체 고객 수
    int service_time[TELLERS] = {0};     // 각 창구의 남은 서비스 시간
    int service_customer[TELLERS] = {0}; // 각 창구의 현재 고객 ID
    QueueType queue;

    init_queue(&queue);
    srand((unsigned int)time(NULL));

    printf("========== 은행 서비스 시뮬레이션 시작 ==========\n");
    printf("- 시뮬레이션 시간: %d분\n", minutes);
    printf("- 은행원(창구) 수: %d명\n", TELLERS);
    printf("- 고객 도착 확률: 30%%\n");
    printf("- 서비스 시간: 1~3분 (랜덤)\n");
    printf("===============================================\n\n");

    for (int clock = 0; clock < minutes; clock++) {
        printf("[%d분] ", clock);

        // 30% 확률로 새로운 고객 도착
        if ((rand() % 10) < 3) {
            Customer customer;
            customer.id = total_customers++;
            customer.arrival_time = clock;
            customer.service_time = rand() % 3 + 1;  // 1~3분
            enqueue(&queue, customer);
            printf("고객%d 도착 ", customer.id);
        }

        // 각 창구별 서비스 처리
        for (int t = 0; t < TELLERS; t++) {
            if (service_time[t] > 0) {
                // 서비스 진행 중
                service_time[t]--;
                printf("[창구%d: 고객%d 서비스중(%d)] ", t, service_customer[t], service_time[t]);
            } else {
                // 창구가 비면 대기열에서 다음 고객 호출
                if (!is_empty(&queue)) {
                    Customer customer = dequeue(&queue);
                    service_customer[t] = customer.id;
                    service_time[t] = customer.service_time;
                    int wait_time = clock - customer.arrival_time;
                    printf("[창구%d: 고객%d 시작(대기%d)] ", t, customer.id, wait_time);
                    total_wait += wait_time;
                }
            }
        }
        printf("\n");
    }

    printf("\n========== 시뮬레이션 결과 ==========\n");
    printf("총 서비스 고객: %d명\n", total_customers);
    printf("전체 대기 시간: %d분\n", total_wait);
    if (total_customers > 0) {
        printf("평균 대기 시간: %.1f분\n", (double)total_wait / total_customers);
    }
    printf("===================================\n");

    return 0;
}
