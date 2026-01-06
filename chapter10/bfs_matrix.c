/**
 * Chapter 10: 그래프 (Graph) - 너비 우선 탐색 (BFS)
 *
 * 인접 행렬 기반 BFS 구현 (큐 사용)
 *
 * 예제 그래프:
 *     0 --- 1 --- 2
 *     |     |     |
 *     3 --- 4 ---+
 *
 * BFS(0) 예상 순서: 0 → 1 → 3 → 2 → 4 (레벨 단위)
 *
 * 시간 복잡도: O(V²) - 인접 행렬은 모든 정점을 검사해야 함
 * 공간 복잡도: O(V) - visited 배열 + 큐
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 50
#define MAX_QUEUE_SIZE 100
#define TRUE 1
#define FALSE 0

// 그래프 구조체
typedef struct GraphType {
    int n;  // 정점의 개수
    int adj_matrix[MAX_VERTICES][MAX_VERTICES];
} GraphType;

// 큐 구조체
typedef struct {
    int data[MAX_QUEUE_SIZE];
    int front;
    int rear;
} QueueType;

// 전역 방문 배열
int visited[MAX_VERTICES];

// ==================== 큐 함수 ====================

void init_queue(QueueType *q) {
    q->front = 0;
    q->rear = 0;
}

int is_empty(QueueType *q) {
    return q->front == q->rear;
}

int is_full(QueueType *q) {
    return q->rear == MAX_QUEUE_SIZE - 1;
}

void enqueue(QueueType *q, int item) {
    if (is_full(q)) {
        fprintf(stderr, "큐: 포화 상태\n");
        return;
    }
    q->data[++(q->rear)] = item;
}

int dequeue(QueueType *q) {
    if (is_empty(q)) {
        fprintf(stderr, "큐: 공백 상태\n");
        return -1;
    }
    return q->data[++(q->front)];
}

// ==================== 그래프 함수 ====================

// 그래프 초기화
void init(GraphType *g) {
    g->n = 0;
    for (int i = 0; i < MAX_VERTICES; i++) {
        for (int j = 0; j < MAX_VERTICES; j++) {
            g->adj_matrix[i][j] = 0;
        }
    }
}

// 정점 삽입 연산
void insert_vertex(GraphType *g, int v) {
    if (((g->n) + 1) > MAX_VERTICES) {
        fprintf(stderr, "그래프: 정점 개수 초과\n");
        return;
    }
    g->n++;
}

// 간선 삽입 연산 (무방향 그래프)
void insert_edge(GraphType *g, int start, int end) {
    if (start >= g->n || end >= g->n) {
        fprintf(stderr, "그래프: 정점 번호 오류\n");
        return;
    }
    g->adj_matrix[start][end] = 1;
    g->adj_matrix[end][start] = 1;
}

// 인접 행렬 출력
void print_adj_matrix(GraphType *g) {
    printf("\n인접 행렬:\n");
    printf("    ");
    for (int i = 0; i < g->n; i++) {
        printf("[%d]", i);
    }
    printf("\n");

    for (int i = 0; i < g->n; i++) {
        printf("[%d] ", i);
        for (int j = 0; j < g->n; j++) {
            printf(" %d ", g->adj_matrix[i][j]);
        }
        printf("\n");
    }
}

// ==================== BFS 함수 ====================

/**
 * 너비 우선 탐색 (BFS) - 인접 행렬, 큐 구현
 *
 * @param g 그래프 포인터
 * @param v 시작 정점
 */
void bfs_matrix(GraphType *g, int v) {
    QueueType q;
    init_queue(&q);

    visited[v] = TRUE;     // 시작 정점 방문 표시
    printf("%d ", v);      // 정점 출력
    enqueue(&q, v);        // 큐에 삽입

    while (!is_empty(&q)) {
        v = dequeue(&q);   // 큐에서 꺼내기

        // 모든 정점 w에 대해 인접 확인
        for (int w = 0; w < g->n; w++) {
            // v와 w가 인접하고, w를 아직 방문하지 않았으면
            if (g->adj_matrix[v][w] == 1 && !visited[w]) {
                visited[w] = TRUE;     // ★ 큐에 넣을 때 방문 표시!
                printf("%d ", w);
                enqueue(&q, w);
            }
        }
    }
}

// ==================== 메인 함수 ====================

int main(void) {
    GraphType *g;

    g = (GraphType *)malloc(sizeof(GraphType));
    if (g == NULL) {
        fprintf(stderr, "메모리 할당 오류\n");
        return 1;
    }

    init(g);

    // 정점 5개 삽입
    printf("정점 삽입: ");
    for (int i = 0; i < 5; i++) {
        insert_vertex(g, i);
        printf("%d ", i);
    }
    printf("\n");

    // 간선 삽입
    // 그래프 구조:
    //     0 --- 1 --- 2
    //     |     |     |
    //     3 --- 4 ---+
    printf("\n간선 삽입:\n");
    insert_edge(g, 0, 1);
    printf("  (0, 1) 추가\n");
    insert_edge(g, 0, 3);
    printf("  (0, 3) 추가\n");
    insert_edge(g, 1, 2);
    printf("  (1, 2) 추가\n");
    insert_edge(g, 1, 4);
    printf("  (1, 4) 추가\n");
    insert_edge(g, 2, 4);
    printf("  (2, 4) 추가\n");
    insert_edge(g, 3, 4);
    printf("  (3, 4) 추가\n");

    print_adj_matrix(g);

    // BFS 탐색
    for (int i = 0; i < g->n; i++) {
        visited[i] = FALSE;
    }

    printf("\n너비 우선 탐색 (BFS) 순회:\n");
    printf("시작 정점: 0\n");
    printf("방문 순서: ");
    bfs_matrix(g, 0);
    printf("\n");

    // 다른 시작 정점으로 테스트
    printf("\n--- 다른 시작 정점 테스트 ---\n");

    for (int i = 0; i < g->n; i++) {
        visited[i] = FALSE;
    }
    printf("시작 정점: 2\n");
    printf("방문 순서: ");
    bfs_matrix(g, 2);
    printf("\n");

    free(g);
    return 0;
}
