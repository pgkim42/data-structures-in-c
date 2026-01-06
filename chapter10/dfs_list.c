/**
 * Chapter 10: 그래프 (Graph) - 깊이 우선 탐색 (DFS)
 *
 * 인접 리스트 기반 DFS 구현 (재귀 방식)
 *
 * 예제 그래프:
 *     0 --- 1 --- 2
 *     |     |     |
 *     3 --- 4 ---+
 *
 * DFS(0) 예상 순서: 0 → 3 → 4 → 1 → 2 (헤드 삽입 순서에 따라)
 *
 * 시간 복잡도: O(V + E) - 인접 리스트는 실제 간선만 순회
 * 공간 복잡도: O(V) - visited 배열 + 재귀 스택
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 50
#define TRUE 1
#define FALSE 0

// 그래프 노드 (연결 리스트)
typedef struct GraphNode {
    int vertex;                // 인접 정점 번호
    struct GraphNode *link;    // 다음 노드 포인터
} GraphNode;

// 그래프 구조체
typedef struct GraphType {
    int n;                     // 정점의 개수
    GraphNode *adjlist[MAX_VERTICES];  // 각 정점의 인접 리스트 헤드
} GraphType;

// 전역 방문 배열
int visited[MAX_VERTICES];

// 그래프 초기화
void init(GraphType *g) {
    g->n = 0;
    for (int i = 0; i < MAX_VERTICES; i++) {
        g->adjlist[i] = NULL;
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
void insert_edge(GraphType *g, int u, int v) {
    if (u >= g->n || v >= g->n) {
        fprintf(stderr, "그래프: 정점 번호 오류\n");
        return;
    }

    // u -> v 간선 추가
    GraphNode *node = (GraphNode *)malloc(sizeof(GraphNode));
    if (node == NULL) {
        fprintf(stderr, "메모리 할당 오류\n");
        return;
    }
    node->vertex = v;
    node->link = g->adjlist[u];
    g->adjlist[u] = node;

    // v -> u 간선 추가 (무방향 그래프이므로 대칭)
    node = (GraphNode *)malloc(sizeof(GraphNode));
    if (node == NULL) {
        fprintf(stderr, "메모리 할당 오류\n");
        return;
    }
    node->vertex = u;
    node->link = g->adjlist[v];
    g->adjlist[v] = node;
}

// 인접 리스트 출력 함수
void print_adj_list(GraphType *g) {
    printf("\n인접 리스트:\n");
    for (int i = 0; i < g->n; i++) {
        printf("[%d] -> ", i);
        GraphNode *p = g->adjlist[i];
        while (p != NULL) {
            printf("%d -> ", p->vertex);
            p = p->link;
        }
        printf("NULL\n");
    }
}

// 그래프 메모리 해제
void destroy_graph(GraphType *g) {
    for (int i = 0; i < g->n; i++) {
        GraphNode *p = g->adjlist[i];
        while (p != NULL) {
            GraphNode *temp = p;
            p = p->link;
            free(temp);
        }
        g->adjlist[i] = NULL;
    }
    g->n = 0;
}

/**
 * 깊이 우선 탐색 (DFS) - 인접 리스트, 재귀 구현
 *
 * @param g 그래프 포인터
 * @param v 현재 방문 중인 정점
 */
void dfs_list(GraphType *g, int v) {
    visited[v] = TRUE;  // 현재 정점 방문 표시
    printf("%d ", v);   // 정점 출력

    // 인접 리스트 순회 (실제로 연결된 정점만)
    for (GraphNode *p = g->adjlist[v]; p != NULL; p = p->link) {
        if (!visited[p->vertex]) {
            dfs_list(g, p->vertex);  // 재귀 호출
        }
    }
}

int main(void) {
    GraphType *g;

    // 그래프 동적 할당
    g = (GraphType *)malloc(sizeof(GraphType));
    if (g == NULL) {
        fprintf(stderr, "메모리 할당 오류\n");
        return 1;
    }

    // 그래프 초기화
    init(g);

    // 정점 5개 삽입 (0~4)
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

    // 인접 리스트 출력
    print_adj_list(g);

    // DFS 탐색
    // visited 배열 초기화
    for (int i = 0; i < g->n; i++) {
        visited[i] = FALSE;
    }

    printf("\n깊이 우선 탐색 (DFS) 순회:\n");
    printf("시작 정점: 0\n");
    printf("방문 순서: ");
    dfs_list(g, 0);
    printf("\n");

    // 다른 시작 정점으로 테스트
    printf("\n--- 다른 시작 정점 테스트 ---\n");

    for (int i = 0; i < g->n; i++) {
        visited[i] = FALSE;  // visited 배열 재초기화
    }
    printf("시작 정점: 2\n");
    printf("방문 순서: ");
    dfs_list(g, 2);
    printf("\n");

    // 메모리 해제
    destroy_graph(g);
    free(g);

    return 0;
}
