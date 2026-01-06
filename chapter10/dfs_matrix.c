/**
 * Chapter 10: 그래프 (Graph) - 깊이 우선 탐색 (DFS)
 *
 * 인접 행렬 기반 DFS 구현 (재귀 방식)
 *
 * 예제 그래프:
 *     0 --- 1 --- 2
 *     |     |     |
 *     3 --- 4 ---+
 *
 * DFS(0) 예상 순서: 0 → 1 → 2 → 4 → 3
 *
 * 시간 복잡도: O(V²) - 인접 행렬은 모든 정점을 검사해야 함
 * 공간 복잡도: O(V) - visited 배열 + 재귀 스택
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 50
#define TRUE 1
#define FALSE 0

typedef struct GraphType {
    int n;  // 정점의 개수
    int adj_matrix[MAX_VERTICES][MAX_VERTICES];
} GraphType;

// 전역 방문 배열
int visited[MAX_VERTICES];

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
    g->adj_matrix[end][start] = 1;  // 무방향 그래프이므로 대칭
}

// 인접 행렬 출력 함수
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

/**
 * 깊이 우선 탐색 (DFS) - 인접 행렬, 재귀 구현
 *
 * @param g 그래프 포인터
 * @param v 현재 방문 중인 정점
 */
void dfs_matrix(GraphType *g, int v) {
    visited[v] = TRUE;  // 현재 정점 방문 표시
    printf("%d ", v);   // 정점 출력

    // 모든 정점 w에 대해
    for (int w = 0; w < g->n; w++) {
        // v와 w가 인접하고, w를 아직 방문하지 않았으면
        if (g->adj_matrix[v][w] == 1 && !visited[w]) {
            dfs_matrix(g, w);  // 재귀 호출
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

    // 인접 행렬 출력
    print_adj_matrix(g);

    // DFS 탐색
    // visited 배열 초기화
    for (int i = 0; i < g->n; i++) {
        visited[i] = 0;  // FALSE (0)로 초기화
    }

    printf("\n깊이 우선 탐색 (DFS) 순회:\n");
    printf("시작 정점: 0\n");
    printf("방문 순서: ");
    dfs_matrix(g, 0);
    printf("\n");

    // 다른 시작 정점으로 테스트 (visited 재초기화 후)
    printf("\n--- 다른 시작 정점 테스트 ---\n");

    for (int i = 0; i < g->n; i++) {
        visited[i] = 0;  // visited 배열 재초기화
    }
    printf("시작 정점: 2\n");
    printf("방문 순서: ");
    dfs_matrix(g, 2);
    printf("\n");

    free(g);
    return 0;
}
