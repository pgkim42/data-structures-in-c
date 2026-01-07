/*
 * Floyd-Warshall All-Pairs Shortest Path Algorithm
 *
 * 시간 복잡도: O(V³)
 * 공간 복잡도: O(V²)
 *
 * 플로이드-워셜 알고리즘은 모든 정점 쌍 간의 최단 경로를 찾습니다.
 * 음의 가중치 간선이 있어도 동작합니다 (음의 사이클 제외).
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <stdbool.h>

#define MAX_VERTICES 100
#define INF          INT_MAX

// ============================================================
// 그래프 자료구조 (가중치 인접 행렬)
// ============================================================

typedef struct {
    int num_vertices;
    int distance[MAX_VERTICES][MAX_VERTICES];  // 최단 거리 행렬
    int next[MAX_VERTICES][MAX_VERTICES];       // 경로 재구성용 (다음 정점)
} Graph;

// ============================================================
// 함수 프로토타입
// ============================================================

bool floyd_warshall(Graph *graph);
bool has_negative_cycle(const Graph *graph);

// ============================================================
// 그래프 함수
// ============================================================

/**
 * 그래프 생성 및 초기화
 * @param num_vertices 정점 개수
 * @return 생성된 그래프 포인터
 */
Graph *graph_create(int num_vertices) {
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->num_vertices = num_vertices;

    // 거리 행렬 초기화
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            if (i == j) {
                graph->distance[i][j] = 0;      // 자기 자신까지 0
                graph->next[i][j] = i;
            } else {
                graph->distance[i][j] = INF;    // 무한대로 초기화
                graph->next[i][j] = -1;         // 경로 없음
            }
        }
    }

    return graph;
}

/**
 * 그래프 메모리 해제
 * @param graph 해제할 그래프 포인터
 */
void graph_destroy(Graph *graph) {
    free(graph);
}

/**
 * 방향 그래프에 간선 추가
 * @param graph 그래프 포인터
 * @param from 시작 정점
 * @param to 끝 정점
 * @param weight 간선 가중치
 */
void graph_add_edge(Graph *graph, int from, int to, int weight) {
    graph->distance[from][to] = weight;
    graph->next[from][to] = to;
}

/**
 * 무방향 그래프에 간선 추가 (양방향)
 * @param graph 그래프 포인터
 * @param vertex1 첫 번째 정점
 * @param vertex2 두 번째 정점
 * @param weight 간선 가중치
 */
void graph_add_undirected_edge(Graph *graph, int vertex1, int vertex2, int weight) {
    graph_add_edge(graph, vertex1, vertex2, weight);
    graph_add_edge(graph, vertex2, vertex1, weight);
}

// ============================================================
// Floyd-Warshall 알고리즘
// ============================================================

/**
 * 플로이드-워셜 알고리즘 실행
 * @param graph 그래프 포인터 (거리 행렬이 직접 수정됨)
 * @return 음의 사이클이 있으면 true, 없으면 false
 */
bool floyd_warshall(Graph *graph) {
    int num_vertices = graph->num_vertices;

    printf("플로이드-워셜 알고리즘 실행\n");
    printf("정점 수: %d\n\n", num_vertices);

    // k: 경유 정점
    for (int k = 0; k < num_vertices; k++) {
        // i: 시작 정점
        for (int i = 0; i < num_vertices; i++) {
            // j: 도착 정점
            for (int j = 0; j < num_vertices; j++) {
                // i→k와 k→j 경로가 존재하고
                // i→k→j가 i→j보다 짧으면 갱신
                if (graph->distance[i][k] != INF &&
                    graph->distance[k][j] != INF) {

                    long long via_distance = (long long)graph->distance[i][k] +
                                             (long long)graph->distance[k][j];

                    if (via_distance < graph->distance[i][j]) {
                        graph->distance[i][j] = (int)via_distance;
                        // next[i][j] 갱신: i에서 k로 가는 첫 번째 정점
                        graph->next[i][j] = graph->next[i][k];
                    }
                }
            }
        }
    }

    // 음의 사이클 탐지
    return has_negative_cycle(graph);
}

/**
 * 음의 사이클 탐지
 * @param graph 그래프 포인터
 * @return 음의 사이클이 있으면 true, 없으면 false
 *
 * distance[i][i] < 0이면 정점 i에서 출발해 i로 돌아오는 경로가 음수
 * 즉, 음의 사이클이 존재함을 의미
 */
bool has_negative_cycle(const Graph *graph) {
    for (int i = 0; i < graph->num_vertices; i++) {
        if (graph->distance[i][i] < 0) {
            return true;
        }
    }
    return false;
}

/**
 * 경로 출력 (재귀)
 * @param graph 그래프 포인터
 * @param from 시작 정점
 * @param to 도착 정점
 */
void print_path_recursive(const Graph *graph, int from, int to) {
    if (graph->next[from][to] == -1) {
        printf("(경로 없음)");
        return;
    }

    printf("%d", from);
    while (from != to) {
        from = graph->next[from][to];
        printf(" → %d", from);
    }
}

/**
 * 특정 경로 출력
 * @param graph 그래프 포인터
 * @param from 시작 정점
 * @param to 도착 정점
 */
void print_path(const Graph *graph, int from, int to) {
    printf("  경로: ");
    print_path_recursive(graph, from, to);
    printf("\n");
}

/**
 * 최단 거리 행렬 출력
 * @param graph 그래프 포인터
 */
void print_distance_matrix(const Graph *graph) {
    int num_vertices = graph->num_vertices;

    printf("\n========== 최단 거리 행렬 ==========\n");
    printf("     ");
    for (int j = 0; j < num_vertices; j++) {
        printf("[%2d] ", j);
    }
    printf("\n");

    for (int i = 0; i < num_vertices; i++) {
        printf("[%2d] ", i);
        for (int j = 0; j < num_vertices; j++) {
            if (graph->distance[i][j] == INF) {
                printf(" INF ");
            } else {
                printf("%4d ", graph->distance[i][j]);
            }
        }
        printf("\n");
    }
}

/**
 * 모든 정점 쌍의 최단 경로 출력
 * @param graph 그래프 포인터
 */
void print_shortest_paths(const Graph *graph) {
    int num_vertices = graph->num_vertices;

    printf("\n========== 모든 정점 쌍 최단 경로 ==========\n");
    for (int i = 0; i < num_vertices; i++) {
        for (int j = 0; j < num_vertices; j++) {
            if (i != j) {
                printf("%d → %d: ", i, j);
                if (graph->distance[i][j] == INF) {
                    printf("도달 불가\n");
                } else {
                    printf("거리 = %d\n", graph->distance[i][j]);
                    print_path(graph, i, j);
                }
            }
        }
    }
}

// ============================================================
// Main
// ============================================================

int main(void) {
    // ========================================
    // 테스트 1: 일반 그래프 (음의 사이클 없음)
    // ========================================
    printf("========================================\n");
    printf("테스트 1: 일반 그래프 (음의 사이클 없음)\n");
    printf("========================================\n\n");

    //      10      5
    //  0 ──── 1 ──── 2
    //  │       │     │
    //  │5      │3    │2
    //  │       │     │
    //  3 ──── 4 ─────┘
    //      1
    Graph *graph = graph_create(5);

    // 간선 추가 (무방향 그래프)
    graph_add_undirected_edge(graph, 0, 1, 10);
    graph_add_undirected_edge(graph, 0, 3, 5);
    graph_add_undirected_edge(graph, 1, 2, 5);
    graph_add_undirected_edge(graph, 1, 4, 3);
    graph_add_undirected_edge(graph, 2, 4, 2);
    graph_add_undirected_edge(graph, 3, 4, 1);

    // 플로이드-워셜 알고리즘 실행
    bool has_cycle = floyd_warshall(graph);

    // 음의 사이클 탐지 결과
    if (has_cycle) {
        printf("\n⚠️  음의 사이클이 존재합니다! 최단 경로가 정의되지 않습니다.\n");
    } else {
        printf("\n✅ 음의 사이클이 없습니다.\n");
    }

    // 결과 출력
    print_distance_matrix(graph);
    print_shortest_paths(graph);

    graph_destroy(graph);

    // ========================================
    // 테스트 2: 음의 사이클이 있는 그래프
    // ========================================
    printf("\n\n========================================\n");
    printf("테스트 2: 음의 사이클이 있는 그래프\n");
    printf("========================================\n\n");

    //  0 → 1 → 2
    //  ↑         ↓
    //  └─────────┘   (-10 간선)
    //
    //  0 → 1 → 2 → 0 사이클의 합: 3 + 4 + (-10) = -3 (음수!)
    Graph *graph_with_cycle = graph_create(3);

    graph_add_edge(graph_with_cycle, 0, 1, 3);
    graph_add_edge(graph_with_cycle, 1, 2, 4);
    graph_add_edge(graph_with_cycle, 2, 0, -10);  // 음의 간선 (사이클 형성)

    printf("그래프 구조:\n");
    printf("  0 → 1 (가중치: 3)\n");
    printf("  1 → 2 (가중치: 4)\n");
    printf("  2 → 0 (가중치: -10)  ← 음의 간선!\n");
    printf("\n사이클 0 → 1 → 2 → 0의 합: 3 + 4 + (-10) = -3\n\n");

    // 플로이드-워셜 알고리즘 실행
    has_cycle = floyd_warshall(graph_with_cycle);

    // 음의 사이클 탐지 결과
    if (has_cycle) {
        printf("\n⚠️  음의 사이클이 존재합니다! 최단 경로가 정의되지 않습니다.\n");
        printf("   (음의 사이클을 무한히 반복하면 거리가 -∞가 됩니다)\n");
    } else {
        printf("\n✅ 음의 사이클이 없습니다.\n");
    }

    // 대각선 값 확인 (음의 사이클 탐지)
    printf("\n대각선 값 확인 (distance[i][i]):\n");
    for (int i = 0; i < 3; i++) {
        printf("  distance[%d][%d] = %d %s\n",
               i, i, graph_with_cycle->distance[i][i],
               graph_with_cycle->distance[i][i] < 0 ? "← 음수!" : "");
    }

    graph_destroy(graph_with_cycle);

    return 0;
}
