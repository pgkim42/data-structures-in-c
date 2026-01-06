/**
 * Chapter 10: 그래프 (Graph) - 인접 행렬 (Adjacency Matrix)
 *
 * 무방향 그래프를 인접 행렬로 구현
 *
 * 예제 그래프:
 *     0 --- 1 --- 2
 *     |     |     |
 *     3 --- 4 ---+
 *
 * 인접 행렬:
 *      0  1  2  3  4
 *   0 [0][1][0][1][0]
 *   1 [1][0][1][0][1]
 *   2 [0][1][0][0][1]
 *   3 [1][0][0][0][1]
 *   4 [0][1][1][1][0]
 *
 * 시간 복잡도:
 * - 간선 삽입: O(1)
 * - 두 정점 간 연결 확인: O(1)
 * - 정점의 모든 이웃 찾기: O(n)
 * - 공간 복잡도: O(n²)
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 50

typedef struct GraphType {
    int n;  // 정점의 개수
    int adj_matrix[MAX_VERTICES][MAX_VERTICES];
} GraphType;

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

    // 특정 정점 간 연결 확인 테스트
    printf("\n연결 확인:\n");
    printf("  정점 0과 1: %s\n", g->adj_matrix[0][1] ? "연결됨" : "연결 안됨");
    printf("  정점 0과 2: %s\n", g->adj_matrix[0][2] ? "연결됨" : "연결 안됨");
    printf("  정점 3과 4: %s\n", g->adj_matrix[3][4] ? "연결됨" : "연결 안됨");

    // 정점 1의 인접 정점들 출력
    printf("\n정점 1의 인접 정점들: ");
    for (int i = 0; i < g->n; i++) {
        if (g->adj_matrix[1][i]) {
            printf("%d ", i);
        }
    }
    printf("\n");

    free(g);
    return 0;
}
