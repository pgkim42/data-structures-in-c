/*
 * Dijkstra's Shortest Path Algorithm
 *
 * 시간 복잡도: O(E log V) - 힙 연산이 지배적
 * 공간 복잡도: O(V + E)
 *
 * 다익스트라 알고리즘은 시작 정점부터 모든 정점까지의
 * 최단 거리를 찾습니다. (음의 가중치 불가)
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
    int distance; // 시작점부터의 거리 (key)
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
    MinHeap *heap = (MinHeap *)malloc(sizeof(MinHeap));
    heap->data = (HeapNode *)malloc(capacity * sizeof(HeapNode));
    heap->pos = (int *)malloc(capacity * sizeof(int));

    // pos 배열 초기화: -1은 "힙에 없음"을 의미
    for (int i = 0; i < capacity; i++) {
        heap->pos[i] = -1;
    }

    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

/**
 * 힙 메모리 해제
 * @param heap 해제할 힙 포인터
 */
void heap_destroy(MinHeap *heap) {
    if (heap) {
        free(heap->data);
        free(heap->pos);
        free(heap);
    }
}

/**
 * 두 힙 노드 교환 (위치 정보도 함께 갱신)
 */
void heap_swap_nodes(MinHeap *heap, int i, int j) {
    HeapNode temp = heap->data[i];
    heap->data[i] = heap->data[j];
    heap->data[j] = temp;

    // 위치 정보 갱신
    heap->pos[heap->data[i].vertex] = i;
    heap->pos[heap->data[j].vertex] = j;
}

/**
 * 힙 속성 유지 (위로 버블업)
 * @param heap 힙 포인터
 * @param index 현재 노드 인덱스
 */
void heap_sift_up(MinHeap *heap, int index) {
    int parent = (index - 1) / 2;

    while (index > 0 && heap->data[index].distance < heap->data[parent].distance) {
        heap_swap_nodes(heap, index, parent);
        index = parent;
        parent = (index - 1) / 2;
    }
}

/**
 * 힙 속성 유지 (아래로 버블다운)
 * @param heap 힙 포인터
 * @param index 현재 노드 인덱스
 */
void heap_sift_down(MinHeap *heap, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < heap->size && heap->data[left].distance < heap->data[smallest].distance) {
        smallest = left;
    }
    if (right < heap->size && heap->data[right].distance < heap->data[smallest].distance) {
        smallest = right;
    }

    if (smallest != index) {
        heap_swap_nodes(heap, index, smallest);
        heap_sift_down(heap, smallest);
    }
}

/**
 * 힙에 노드 삽입
 * @param heap 힙 포인터
 * @param vertex 정점 번호
 * @param distance 시작점부터의 거리
 * @return 성공 시 true, 실패 시 false
 */
bool heap_push(MinHeap *heap, int vertex, int distance) {
    if (heap->size >= heap->capacity) {
        return false;
    }

    heap->data[heap->size].vertex = vertex;
    heap->data[heap->size].distance = distance;
    heap->pos[vertex] = heap->size;
    heap->size++;

    heap_sift_up(heap, heap->size - 1);
    return true;
}

/**
 * 힙에서 최소값 추출
 * @param heap 힙 포인터
 * @param vertex 추출된 정점 번호를 저장할 포인터
 * @param distance 추출된 거리를 저장할 포인터
 * @return 성공 시 true, 빈 힙이면 false
 */
bool heap_pop(MinHeap *heap, int *vertex, int *distance) {
    if (heap->size == 0) {
        return false;
    }

    *vertex = heap->data[0].vertex;
    *distance = heap->data[0].distance;

    // 마지막 노드를 루트로 이동
    heap->data[0] = heap->data[heap->size - 1];
    heap->pos[heap->data[0].vertex] = 0;
    heap->size--;

    heap_sift_down(heap, 0);
    return true;
}

/**
 * 힙 내 특정 정점의 거리 감소 (다익스트라 핵심 연산)
 * @param heap 힙 포인터
 * @param vertex 대상 정점
 * @param new_distance 새로운 (더 작은) 거리
 * @return 성공 시 true, 정점이 힙에 없으면 false
 */
bool heap_decrease_key(MinHeap *heap, int vertex, int new_distance) {
    int index = heap->pos[vertex];

    // 정점이 힙에 없는 경우 (이미 추출됨)
    if (index >= heap->size) {
        return false;
    }

    heap->data[index].distance = new_distance;
    heap_sift_up(heap, index);
    return true;
}

/**
 * 힙이 비어있는지 확인
 * @param heap 힙 포인터
 * @return 비어있으면 true
 */
bool heap_is_empty(const MinHeap *heap) {
    return heap->size == 0;
}

/**
 * 정점이 힙에 있는지 확인
 * @param heap 힙 포인터
 * @param vertex 확인할 정점
 * @return 힙에 있으면 true
 */
bool heap_contains(const MinHeap *heap, int vertex) {
    int position = heap->pos[vertex];
    return position >= 0 && position < heap->size;
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
 * @return 생성된 그래프 포인터
 */
Graph *graph_create(int num_vertices) {
    Graph *graph = (Graph *)malloc(sizeof(Graph));
    graph->num_vertices = num_vertices;
    graph->array = (AdjList *)calloc(num_vertices, sizeof(AdjList));
    return graph;
}

/**
 * 그래프 메모리 해제
 * @param graph 해제할 그래프 포인터
 */
void graph_destroy(Graph *graph) {
    if (graph) {
        for (int i = 0; i < graph->num_vertices; i++) {
            AdjNode *current = graph->array[i].head;
            while (current) {
                AdjNode *temp = current;
                current = current->next;
                free(temp);
            }
        }
        free(graph->array);
        free(graph);
    }
}

/**
 * 방향 그래프에 간선 추가
 * @param graph 그래프 포인터
 * @param from 시작 정점
 * @param to 끝 정점
 * @param weight 간선 가중치
 * @return 성공 시 true
 */
bool graph_add_edge(Graph *graph, int from, int to, int weight) {
    AdjNode *new_node = (AdjNode *)malloc(sizeof(AdjNode));
    new_node->to = to;
    new_node->weight = weight;
    new_node->next = graph->array[from].head;
    graph->array[from].head = new_node;

    return true;
}

/**
 * 무방향 그래프에 간선 추가 (양방향)
 * @param graph 그래프 포인터
 * @param vertex1 첫 번째 정점
 * @param vertex2 두 번째 정점
 * @param weight 간선 가중치
 * @return 성공 시 true
 */
bool graph_add_undirected_edge(Graph *graph, int vertex1, int vertex2, int weight) {
    graph_add_edge(graph, vertex1, vertex2, weight);
    graph_add_edge(graph, vertex2, vertex1, weight);
    return true;
}

// ============================================================
// Dijkstra 최단 경로 알고리즘
// ============================================================

/**
 * 경로 추적 및 출력 (재귀)
 * @param parent 이전 정점 배열
 * @param vertex 현재 정점
 */
void print_path_recursive(const int *parent, int vertex) {
    if (parent[vertex] == vertex) {
        printf("%d", vertex);
        return;
    }
    print_path_recursive(parent, parent[vertex]);
    printf(" → %d", vertex);
}

/**
 * 시작 정점부터 목적 정점까지의 경로 출력
 * @param parent 이전 정점 배열
 * @param destination 목적 정점
 */
void print_path(const int *parent, int destination) {
    printf("  경로: ");
    print_path_recursive(parent, destination);
    printf("\n");
}

/**
 * 다익스트라 알고리즘으로 최단 경로 탐색
 * @param graph 입력 그래프 (읽기 전용)
 * @param start 시작 정점
 */
void dijkstra_shortest_path(const Graph *graph, int start) {
    int num_vertices = graph->num_vertices;

    // 방문 여부 배열
    bool *visited = (bool *)calloc(num_vertices, sizeof(bool));

    // 시작점부터 각 정점까지의 최단 거리
    int *distance = (int *)malloc(num_vertices * sizeof(int));
    for (int i = 0; i < num_vertices; i++) {
        distance[i] = INF;
    }
    distance[start] = 0;

    // 최단 경로 추적을 위한 이전 정점 배열
    int *parent = (int *)malloc(num_vertices * sizeof(int));
    for (int i = 0; i < num_vertices; i++) {
        parent[i] = -1;
    }
    parent[start] = start;

    // 최소 힙 생성
    MinHeap *heap = heap_create(num_vertices);

    // 시작 정점을 힙에 추가 (거리 0)
    heap_push(heap, start, 0);

    printf("다익스트라 최단 경로 알고리즘\n");
    printf("정점 수: %d, 시작 정점: %d\n\n", num_vertices, start);

    while (!heap_is_empty(heap)) {
        int current_vertex, current_distance;
        heap_pop(heap, &current_vertex, &current_distance);

        // 이미 방문한 정점이면 스킵
        if (visited[current_vertex]) {
            continue;
        }
        visited[current_vertex] = true;

        printf("정점 %d 확정 (거리: %d)\n", current_vertex, current_distance);

        // 현재 정점의 모든 인접 정점 확인
        AdjNode *neighbor = graph->array[current_vertex].head;
        while (neighbor) {
            int adjacent_vertex = neighbor->to;
            int edge_weight = neighbor->weight;

            // 방문하지 않은 인접 정점에 대해 거리 갱신
            if (!visited[adjacent_vertex]) {
                // 새로운 거리 = 현재 정점까지의 거리 + 간선 가중치
                int new_distance = distance[current_vertex] + edge_weight;

                // 더 짧은 경로를 발견하면 갱신
                if (new_distance < distance[adjacent_vertex]) {
                    distance[adjacent_vertex] = new_distance;
                    parent[adjacent_vertex] = current_vertex;

                    if (heap_contains(heap, adjacent_vertex)) {
                        heap_decrease_key(heap, adjacent_vertex, new_distance);
                    } else {
                        heap_push(heap, adjacent_vertex, new_distance);
                    }

                    printf("  → %d 갱신: %d (경로: %d → %d)\n",
                           adjacent_vertex, new_distance,
                           current_vertex, adjacent_vertex);
                }
            }
            neighbor = neighbor->next;
        }
    }

    printf("\n========== 최단 경로 결과 ==========\n");
    for (int i = 0; i < num_vertices; i++) {
        printf("정점 %d: ", i);
        if (distance[i] == INF) {
            printf("도달 불가\n");
        } else {
            printf("최단 거리 = %d\n", distance[i]);
            print_path(parent, i);
        }
    }

    // 메모리 정리
    free(visited);
    free(distance);
    free(parent);
    heap_destroy(heap);
}

// ============================================================
// Main
// ============================================================

int main(void) {
    // 방향 그래프 생성
    //      10      5
    //  0 ────→ 1 ────→ 2
    //  │       │      ↑
    //  │5      │3     │2
    //  ↓       ↓      │
    //  3 ────→ 4 ─────┘
    //      1
    Graph *graph = graph_create(5);

    // 간선 추가 (방향 그래프)
    graph_add_edge(graph, 0, 1, 10);
    graph_add_edge(graph, 0, 3, 5);
    graph_add_edge(graph, 1, 2, 5);
    graph_add_edge(graph, 1, 4, 3);
    graph_add_edge(graph, 2, 4, 2);
    graph_add_edge(graph, 3, 4, 1);

    // 다익스트라 알고리즘 실행 (정점 0에서 시작)
    dijkstra_shortest_path(graph, 0);

    graph_destroy(graph);

    return 0;
}
