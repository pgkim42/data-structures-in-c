/**
 * Chapter 10: 그래프 (Graph) - 인접 리스트 (Adjacency List)
 *
 * 무방향 그래프를 인접 리스트로 구현
 *
 * 예제 그래프:
 *     0 --- 1 --- 2
 *     |     |     |
 *     3 --- 4 ---+
 *
 * 인접 리스트:
 *   [0] -> 1 -> 3 -> NULL
 *   [1] -> 0 -> 2 -> 4 -> NULL
 *   [2] -> 1 -> 4 -> NULL
 *   [3] -> 0 -> 4 -> NULL
 *   [4] -> 1 -> 2 -> 3 -> NULL
 *
 * 시간 복잡도:
 * - 간선 삽입: O(1)
 * - 두 정점 간 연결 확인: O(차수)
 * - 정점의 모든 이웃 찾기: O(차수)
 * - 공간 복잡도: O(n + e) → 희소 그래프에서 인접 행렬보다 효율적
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_VERTICES 50

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

// 간선 삽입 연산, v를 u의 인접 리스트에 삽입한다 (무방향 그래프)
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
    node->link = g->adjlist[u];  // 헤드에 삽입
    g->adjlist[u] = node;

    // v -> u 간선 추가 (무방향 그래프이므로 대칭)
    node = (GraphNode *)malloc(sizeof(GraphNode));
    if (node == NULL) {
        fprintf(stderr, "메모리 할당 오류\n");
        return;
    }
    node->vertex = u;
    node->link = g->adjlist[v];  // 헤드에 삽입
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

// 두 정점 간 연결 확인 함수
int is_adjacent(GraphType *g, int u, int v) {
    GraphNode *p = g->adjlist[u];
    while (p != NULL) {
        if (p->vertex == v) return 1;
        p = p->link;
    }
    return 0;
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

    // 특정 정점의 인접 정점 개수 출력
    printf("\n각 정점의 차수(인접 정점 수):\n");
    for (int i = 0; i < g->n; i++) {
        int degree = 0;
        GraphNode *p = g->adjlist[i];
        while (p != NULL) {
            degree++;
            p = p->link;
        }
        printf("  정점 [%d]: 차수 %d\n", i, degree);
    }

    printf("\n연결 확인:\n");
    printf("  정점 0과 1: %s\n", is_adjacent(g, 0, 1) ? "연결됨" : "연결 안됨");
    printf("  정점 0과 2: %s\n", is_adjacent(g, 0, 2) ? "연결됨" : "연결 안됨");
    printf("  정점 3과 4: %s\n", is_adjacent(g, 3, 4) ? "연결됨" : "연결 안됨");

    // 메모리 해제
    destroy_graph(g);
    free(g);

    return 0;
}
