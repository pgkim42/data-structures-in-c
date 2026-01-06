/*
 * Prim's Minimum Spanning Tree Algorithm
 *
 * 시간 복잡도: O(E log V) - 힙 연산이 지배적
 * 공간 복잡도: O(V + E)
 *
 * Prim 알고리즘은 임의의 정점에서 시작하여,
 * 매 단계마다 현재 MST에서 가장 가까운 정점을 추가합니다.
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>

#define MAX_VERTICES 100
#define INF          INT_MAX

// ============================================================
// Min-Heap 자료구조 (우선순위 큐)
// ============================================================

typedef struct {
    int vertex;   // 정점 번호
    int weight;   // MST에서의 연결 비용 (key)
} HeapNode;

typedef struct {
    HeapNode *data;     // 힙 데이터 배열
    int *pos;           // 정점별 힙 내 위치 (decrease_key용)
    int size;           // 현재 힙 크기
    int capacity;       // 힙 최대 용량
} MinHeap;

/**
 * 힙 생성
 * @param capacity 힙 최대 용량
 * @return 생성된 힙 포인터
 */
MinHeap *heap_create(int capacity) {
    MinHeap *h = (MinHeap *)malloc(sizeof(MinHeap));
    h->data = (HeapNode *)malloc(capacity * sizeof(HeapNode));
    h->pos = (int *)malloc(capacity * sizeof(int));
    h->size = 0;
    h->capacity = capacity;
    return h;
}

/**
 * 힙 메모리 해제
 * @param h 해제할 힙 포인터
 */
void heap_destroy(MinHeap *h) {
    if (h) {
        free(h->data);
        free(h->pos);
        free(h);
    }
}

/**
 * 두 힙 노드 교환 (위치 정보도 함께 갱신)
 */
void heap_swap_nodes(MinHeap *h, int i, int j) {
    HeapNode temp = h->data[i];
    h->data[i] = h->data[j];
    h->data[j] = temp;

    // 위치 정보 갱신
    h->pos[h->data[i].vertex] = i;
    h->pos[h->data[j].vertex] = j;
}

/**
 * 힙 속성 유지 (위로 버블업)
 * @param h  힙 포인터
 * @param idx 현재 노드 인덱스
 */
void heap_sift_up(MinHeap *h, int idx) {
    int parent = (idx - 1) / 2;

    while (idx > 0 && h->data[idx].weight < h->data[parent].weight) {
        heap_swap_nodes(h, idx, parent);
        idx = parent;
        parent = (idx - 1) / 2;
    }
}

/**
 * 힙 속성 유지 (아래로 버블다운)
 * @param h  힙 포인터
 * @param idx 현재 노드 인덱스
 */
void heap_sift_down(MinHeap *h, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < h->size && h->data[left].weight < h->data[smallest].weight) {
        smallest = left;
    }
    if (right < h->size && h->data[right].weight < h->data[smallest].weight) {
        smallest = right;
    }

    if (smallest != idx) {
        heap_swap_nodes(h, idx, smallest);
        heap_sift_down(h, smallest);
    }
}

/**
 * 힙에 노드 삽입
 * @param h      힙 포인터
 * @param vertex 정점 번호
 * @param weight 가중치 (key 값)
 * @return       성공 시 true, 실패 시 false
 */
bool heap_push(MinHeap *h, int vertex, int weight) {
    if (h->size >= h->capacity) {
        return false;
    }

    h->data[h->size].vertex = vertex;
    h->data[h->size].weight = weight;
    h->pos[vertex] = h->size;
    h->size++;

    heap_sift_up(h, h->size - 1);
    return true;
}

/**
 * 힙에서 최소값 추출
 * @param h  힙 포인터
 * @param vertex 추출된 정점 번호를 저장할 포인터
 * @param weight 추출된 가중치를 저장할 포인터
 * @return       성공 시 true, 빈 힙이면 false
 */
bool heap_pop(MinHeap *h, int *vertex, int *weight) {
    if (h->size == 0) {
        return false;
    }

    *vertex = h->data[0].vertex;
    *weight = h->data[0].weight;

    // 마지막 노드를 루트로 이동
    h->data[0] = h->data[h->size - 1];
    h->pos[h->data[0].vertex] = 0;
    h->size--;

    heap_sift_down(h, 0);
    return true;
}

/**
 * 힙 내 특정 정점의 가중치 감소 (Prim 핵심 연산)
 * @param h         힙 포인터
 * @param vertex    대상 정점
 * @param new_weight 새로운 (더 작은) 가중치
 * @return          성공 시 true, 정점이 힙에 없으면 false
 */
bool heap_decrease_key(MinHeap *h, int vertex, int new_weight) {
    int idx = h->pos[vertex];

    // 정점이 힙에 없는 경우 (이미 추출됨)
    if (idx >= h->size) {
        return false;
    }

    h->data[idx].weight = new_weight;
    heap_sift_up(h, idx);
    return true;
}

/**
 * 힙이 비어있는지 확인
 * @param  h 힙 포인터
 * @return   비어있으면 true
 */
bool heap_is_empty(const MinHeap *h) {
    return h->size == 0;
}

/**
 * 정점이 힙에 있는지 확인
 * @param  h      힙 포인터
 * @param  vertex 확인할 정점
 * @return        힙에 있으면 true
 */
bool heap_contains(const MinHeap *h, int vertex) {
    return h->pos[vertex] < h->size;
}

// ============================================================
// 그래프 자료구조 (인접 리스트)
// ============================================================

typedef struct AdjNode {
    int to;                // 인접 정점
    int weight;            // 간선 가중치
    struct AdjNode *next;  // 다음 인접 노드
} AdjNode;

typedef struct {
    AdjNode *head;  // 인접 리스트 헤드
} AdjList;

typedef struct {
    int num_vertices;  // 정점 개수
    AdjList *array;    // 정점별 인접 리스트 배열
} Graph;

/**
 * 그래프 생성
 * @param num_vertices 정점 개수
 * @return             생성된 그래프 포인터
 */
Graph *graph_create(int num_vertices) {
    Graph *g = (Graph *)malloc(sizeof(Graph));
    g->num_vertices = num_vertices;
    g->array = (AdjList *)calloc(num_vertices, sizeof(AdjList));
    return g;
}

/**
 * 그래프 메모리 해제
 * @param g 해제할 그래프 포인터
 */
void graph_destroy(Graph *g) {
    if (g) {
        for (int i = 0; i < g->num_vertices; i++) {
            AdjNode *curr = g->array[i].head;
            while (curr) {
                AdjNode *temp = curr;
                curr = curr->next;
                free(temp);
            }
        }
        free(g->array);
        free(g);
    }
}

/**
 * 무방향 그래프에 간선 추가
 * @param g       그래프 포인터
 * @param from    시작 정점
 * @param to      끝 정점
 * @param weight  간선 가중치
 * @return        성공 시 true
 */
bool graph_add_edge(Graph *g, int from, int to, int weight) {
    // from -> to 방향 추가
    AdjNode *new_node = (AdjNode *)malloc(sizeof(AdjNode));
    new_node->to = to;
    new_node->weight = weight;
    new_node->next = g->array[from].head;
    g->array[from].head = new_node;

    // to -> from 방향 추가 (무방향 그래프)
    new_node = (AdjNode *)malloc(sizeof(AdjNode));
    new_node->to = from;
    new_node->weight = weight;
    new_node->next = g->array[to].head;
    g->array[to].head = new_node;

    return true;
}

// ============================================================
// Prim MST 알고리즘
// ============================================================

/**
 * Prim 알고리즘으로 최소 신장 트리 구축 및 출력
 * @param g     입력 그래프 (읽기 전용)
 * @param start 시작 정점
 */
void prim_mst(const Graph *g, int start) {
    int num_vertices = g->num_vertices;

    // 방문 여부 배열
    bool *visited = (bool *)calloc(num_vertices, sizeof(bool));

    // MST에서의 부모 정점 (트리 재구성용)
    int *parent = (int *)malloc(num_vertices * sizeof(int));
    for (int i = 0; i < num_vertices; i++) {
        parent[i] = -1;  // -1은 루트 또는 미연결
    }

    // 최소 힙 생성
    MinHeap *h = heap_create(num_vertices);

    // 시작 정점을 힙에 추가 (가중치 0)
    heap_push(h, start, 0);
    parent[start] = start;  // 자기 자신을 부모로 (루트 표시)

    int total_weight = 0;
    int edges_in_mst = 0;

    printf("Prim MST 알고리즘\n");
    printf("정점 수: %d, 시작 정점: %d\n\n", num_vertices, start);

    while (!heap_is_empty(h) && edges_in_mst < num_vertices - 1) {
        int u, weight;
        heap_pop(h, &u, &weight);

        // 이미 방문한 정점이면 스킵
        if (visited[u]) {
            continue;
        }
        visited[u] = true;

        // 루트가 아니면 간선 출력
        if (parent[u] != u) {
            printf("  간선 선택: (%d, %d) 가중치: %d\n",
                   parent[u], u, weight);
            total_weight += weight;
            edges_in_mst++;
        }

        // u의 모든 인접 정점 확인
        AdjNode *curr = g->array[u].head;
        while (curr) {
            int v = curr->to;
            int w = curr->weight;

            // 방문하지 않은 인접 정점을 힙에 추가
            if (!visited[v]) {
                if (heap_contains(h, v)) {
                    // 이미 힙에 있으면 더 작은 가중치로 갱신
                    heap_decrease_key(h, v, w);
                    parent[v] = u;
                } else {
                    // 힙에 없으면 새로 추가
                    heap_push(h, v, w);
                    parent[v] = u;
                }
            }
            curr = curr->next;
        }
    }

    printf("\nMST 완성:\n");
    printf("  선택된 간선 수: %d\n", edges_in_mst);
    printf("  총 가중치: %d\n", total_weight);

    // 메모리 정리
    free(visited);
    free(parent);
    heap_destroy(h);
}

// ============================================================
// Main
// ============================================================

int main(void) {
    // 7개 정점을 가진 그래프 생성
    Graph *g = graph_create(7);

    // 간선 추가 (무방향 그래프)
    // kruskal.c와 동일한 테스트 데이터
    graph_add_edge(g, 0, 1, 29);
    graph_add_edge(g, 1, 2, 16);
    graph_add_edge(g, 2, 3, 12);
    graph_add_edge(g, 3, 4, 22);
    graph_add_edge(g, 4, 5, 27);
    graph_add_edge(g, 5, 0, 10);
    graph_add_edge(g, 6, 1, 15);
    graph_add_edge(g, 6, 3, 18);
    graph_add_edge(g, 6, 4, 25);

    // Prim 알고리즘 실행 (정점 0에서 시작)
    prim_mst(g, 0);

    graph_destroy(g);

    return 0;
}
