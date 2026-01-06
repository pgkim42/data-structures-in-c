/*
 * Kruskal's Minimum Spanning Tree Algorithm
 *
 * 시간 복잡도: O(E log E) - 간선 정렬이 지배적
 * 공간 복잡도: O(V + E)
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define MAX_VERTICES 100
#define MAX_EDGES    (2 * MAX_VERTICES)

// ============================================================
// Disjoint Set Union (Union-Find) 자료구조
// ============================================================

typedef struct {
    int *parent;  // 각 노드의 부모 인덱스 (-1이면 루트)
    int size;     // 전체 노드 수
} DSU;

/**
 * DSU 초기화: 각 노드를 독립된 집합으로 만듦
 * @param dsu  초기화할 DSU 구조체 포인터
 * @param n    노드의 개수
 */
void dsu_init(DSU *dsu, int n) {
    dsu->parent = (int *)malloc(n * sizeof(int));
    dsu->size = n;

    for (int i = 0; i < n; i++) {
        dsu->parent[i] = -1;  // -1은 루트 노드를 의미
    }
}

/**
 * DSU 메모리 해제
 * @param dsu  해제할 DSU 구조체 포인터
 */
void dsu_destroy(DSU *dsu) {
    free(dsu->parent);
    dsu->parent = NULL;
    dsu->size = 0;
}

/**
 * Path Compression 없는 기본 Find 연산
 * @param dsu   DSU 구조체 포인터
 * @param node  루트를 찾을 노드
 * @return      루트 노드의 인덱스
 */
int dsu_find(DSU *dsu, int node) {
    if (dsu->parent[node] == -1) {
        return node;  // 루트 노드
    }
    return dsu_find(dsu, dsu->parent[node]);  // 재귀적으로 루트 탐색
}

/**
 * Union 연산: 두 노드가 속한 집합을 병합
 * @param dsu  DSU 구조체 포인터
 * @param a    첫 번째 노드
 * @param b    두 번째 노드
 * @return     병합 성공 시 true, 이미 같은 집합이면 false
 */
bool dsu_union(DSU *dsu, int a, int b) {
    int root_a = dsu_find(dsu, a);
    int root_b = dsu_find(dsu, b);

    if (root_a == root_b) {
        return false;  // 이미 같은 집합 (사이클 형성)
    }

    // root_b를 root_a의 자식으로 연결
    dsu->parent[root_b] = root_a;
    return true;
}

// ============================================================
// Graph 자료구조
// ============================================================

typedef struct {
    int from;    // 시작 정점
    int to;      // 끝 정점
    int weight;  // 간선 가중치
} Edge;

typedef struct {
    int num_vertices;  // 정점 개수
    int num_edges;     // 간선 개수
    Edge edges[MAX_EDGES];  // 간선 리스트 (간선 기반 표현)
} Graph;

/**
 * 그래프 초기화
 * @param g             초기화할 그래프 포인터
 * @param num_vertices  정점 개수
 */
void graph_init(Graph *g, int num_vertices) {
    g->num_vertices = num_vertices;
    g->num_edges = 0;
}

/**
 * 그래프에 간선 추가
 * @param g       그래프 포인터
 * @param from    시작 정점
 * @param to      끝 정점
 * @param weight  간선 가중치
 * @return        성공 시 true, 용량 초과 시 false
 */
bool graph_add_edge(Graph *g, int from, int to, int weight) {
    if (g->num_edges >= MAX_EDGES) {
        fprintf(stderr, "Error: Edge capacity exceeded\n");
        return false;
    }

    g->edges[g->num_edges].from = from;
    g->edges[g->num_edges].to = to;
    g->edges[g->num_edges].weight = weight;
    g->num_edges++;

    return true;
}

// ============================================================
// qsort를 위한 비교 함수
// ============================================================

/**
 * 간선 가중치 기준 오름차순 정렬 비교 함수
 */
int compare_edges(const void *a, const void *b) {
    const Edge *ea = (const Edge *)a;
    const Edge *eb = (const Edge *)b;

    if (ea->weight == eb->weight) {
        return ea->from - eb->from;  // 가중치가 같으면 from 기준 정렬
    }
    return ea->weight - eb->weight;
}

// ============================================================
// Kruskal MST 알고리즘
// ============================================================

/**
 * Kruskal 알고리즘으로 최소 신장 트리 구축 및 출력
 * @param g  입력 그래프 (읽기 전용)
 */
void kruskal_mst(const Graph *g) {
    DSU dsu;
    int edges_in_mst = 0;   // MST에 포함된 간선 수
    int total_weight = 0;    // MST 총 가중치

    // 1단계: DSU 초기화
    dsu_init(&dsu, g->num_vertices);

    // 2단계: 간선을 가중치 오름차순 정렬
    qsort((void *)g->edges, g->num_edges, sizeof(Edge), compare_edges);

    printf("Kruskal MST 알고리즘\n");
    printf("정점 수: %d, 간선 수: %d\n\n", g->num_vertices, g->num_edges);

    // 3단계: 가중치가 작은 간선부터 순회하며 MST 구성
    for (int i = 0; i < g->num_edges && edges_in_mst < g->num_vertices - 1; i++) {
        Edge e = g->edges[i];
        int root_u = dsu_find(&dsu, e.from);
        int root_v = dsu_find(&dsu, e.to);

        if (root_u != root_v) {
            // 두 정점이 다른 집합에 속하면 사이클이 생기지 않음
            printf("  간선 선택: (%d, %d) 가중치: %d\n", e.from, e.to, e.weight);
            dsu_union(&dsu, e.from, e.to);
            total_weight += e.weight;
            edges_in_mst++;
        }
        // 같은 집합이면 이미 연결된 상태 → 사이클 형성으로 스킵
    }

    printf("\nMST 완성:\n");
    printf("  선택된 간선 수: %d\n", edges_in_mst);
    printf("  총 가중치: %d\n", total_weight);

    dsu_destroy(&dsu);
}

// ============================================================
// Main
// ============================================================

int main(void) {
    Graph g;

    // 7개 정점을 가진 그래프 초기화
    graph_init(&g, 7);

    // 간선 추가 (무방향 그래프)
    graph_add_edge(&g, 0, 1, 29);
    graph_add_edge(&g, 1, 2, 16);
    graph_add_edge(&g, 2, 3, 12);
    graph_add_edge(&g, 3, 4, 22);
    graph_add_edge(&g, 4, 5, 27);
    graph_add_edge(&g, 5, 0, 10);
    graph_add_edge(&g, 6, 1, 15);
    graph_add_edge(&g, 6, 3, 18);
    graph_add_edge(&g, 6, 4, 25);

    kruskal_mst(&g);

    return 0;
}
