/*
 * Topological Sort (Using Kahn's Algorithm)
 *
 * 시간 복잡도: O(V + E) (인접 리스트 사용 시) / O(V^2) (인접 행렬 사용 시)
 * 공간 복잡도: O(V)
 *
 * 위상 정렬은 순서가 정해진 작업을 차례대로 수행할 때 사용합니다.
 * DAG(유향 비순환 그래프)에서만 수행 가능합니다.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_VERTICES 100

// ============================================================
// 그래프 및 큐 자료구조
// ============================================================

typedef struct {
    int num_vertices;
    int adj[MAX_VERTICES][MAX_VERTICES]; // 인접 행렬 (간선 여부)
    int indegree[MAX_VERTICES];          // 진입 차수 (들어오는 간선 수)
} Graph;

typedef struct {
    int items[MAX_VERTICES];
    int front;
    int rear;
} Queue;

// ============================================================
// 큐(Queue) 함수 (배열 기반 원형 큐 아님, 단순 선형 큐)
// ============================================================

void queue_init(Queue *q) {
    q->front = 0;
    q->rear = 0;
}

bool queue_is_empty(Queue *q) {
    return q->front == q->rear;
}

void enqueue(Queue *q, int value) {
    q->items[q->rear++] = value;
}

int dequeue(Queue *q) {
    return q->items[q->front++];
}

// ============================================================
// 그래프 함수
// ============================================================

Graph *graph_create(int num_vertices) {
    Graph *g = (Graph *)malloc(sizeof(Graph));
    g->num_vertices = num_vertices;

    for (int i = 0; i < num_vertices; i++) {
        g->indegree[i] = 0;
        for (int j = 0; j < num_vertices; j++) {
            g->adj[i][j] = 0;
        }
    }
    return g;
}

// 방향 그래프 간선 추가 (from -> to)
void graph_add_edge(Graph *g, int from, int to) {
    g->adj[from][to] = 1;
    g->indegree[to]++; // 도착점의 진입 차수 증가
}

// ============================================================
// 위상 정렬 (Topological Sort)
// ============================================================

void topological_sort(Graph *g) {
    Queue q;
    queue_init(&q);

    // 결과 저장용 배열
    int result[MAX_VERTICES];
    int result_idx = 0;

    printf("위상 정렬 시작\n");
    printf("초기 진입 차수: ");
    for(int i=0; i<g->num_vertices; i++) printf("[%d]:%d ", i, g->indegree[i]);
    printf("\n\n");

    // 1. 진입 차수가 0인 모든 노드를 큐에 삽입
    for (int i = 0; i < g->num_vertices; i++) {
        if (g->indegree[i] == 0) {
            enqueue(&q, i);
        }
    }

    // 2. 큐가 빌 때까지 반복
    while (!queue_is_empty(&q)) {
        int current = dequeue(&q);

        // 결과 배열에 현재 방문한 노드 추가
        result[result_idx++] = current;

        // 해당 노드와 연결된 모든 노드들의 진입 차수 감소
        for (int next = 0; next < g->num_vertices; next++) {
            if (g->adj[current][next] == 1) { // 연결되어 있다면
                g->indegree[next]--; // 진입 차수 감소 (간선 삭제 효과)

                // 진입 차수가 0이 되었다면 큐에 삽입
                if (g->indegree[next] == 0) {
                    enqueue(&q, next);
                }
            }
        }
    }

    // 3. 사이클 판단 및 결과 출력
    // 방문한 노드의 개수가 전체 노드 수보다 적다면 사이클이 존재한다는 뜻
    if (result_idx < g->num_vertices) {
        printf("사이클이 발생하여 위상 정렬을 완료할 수 없습니다.\n");
    } else {
        printf("위상 정렬 결과: ");
        for (int i = 0; i < result_idx; i++) {
            printf("%d ", result[i]);
            if (i < result_idx - 1) printf("→ ");
        }
        printf("\n");
    }
}

int main(void) {
    //      1       4
    //    ↗   ↘   ↗   ↘
    //  0       2       5
    //    ↘   ↗   ↘   ↗
    //      3       4 (No, 2->4 added)

    /* * 예제 그래프 구조 (선수 과목이라 생각하면 편합니다)
     * 0번 완료해야 -> 1, 3 가능
     * 1번 완료해야 -> 2 가능
     * 3번 완료해야 -> 2 가능
     * 2번 완료해야 -> 4, 5 가능
     * 4번 완료해야 -> 5 가능
     */

    Graph *g = graph_create(6);

    graph_add_edge(g, 0, 1);
    graph_add_edge(g, 0, 3);
    graph_add_edge(g, 1, 2);
    graph_add_edge(g, 3, 2);
    graph_add_edge(g, 2, 4);
    graph_add_edge(g, 2, 5);
    graph_add_edge(g, 4, 5);

    topological_sort(g);

    // 0 -> 1 -> 3 -> 2 -> 4 -> 5 순서 (1과 3의 순서는 바뀔 수 있음)
    // 또는
    // 0 -> 3 -> 1 -> 2 -> 4 -> 5

    free(g);
    return 0;
}