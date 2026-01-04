#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 100

/* ============================================================
 * 노드 및 리스트 구조체 정의
 * ============================================================ */
typedef struct {
    int data;
    int next;
} Node;

typedef struct {
    Node nodes[MAX_SIZE];
    int head;
    int free;
} LinkedList;

/* ============================================================
 * 에러 처리 함수
 * Time Complexity: O(1)
 * ============================================================ */
void error(const char *message) {
    fprintf(stderr, "[ERROR] %s\n", message);
    exit(1);
}

/* ============================================================
 * 리스트 초기화
 * Time Complexity: O(n) where n = MAX_SIZE
 * - 초기화 시점에 한 번만 호출되므로 실제 사용에서는 O(1)로 간주
 * ============================================================ */
void init(LinkedList *list) {
    int i;
    for (i = 0; i < MAX_SIZE - 1; i++) {
        list->nodes[i].next = i + 1;
    }
    list->nodes[MAX_SIZE - 1].next = -1;

    list->head = -1;
    list->free = 0;
}

/* ============================================================
 * 유틸리티 함수
 * ============================================================ */

/* 리스트가 비었는지 확인
 * Time Complexity: O(1)
 */
int is_empty(LinkedList *list) {
    return list->head == -1;
}

/* 리스트가 가득 찼는지 확인
 * Time Complexity: O(1)
 */
int is_full(LinkedList *list) {
    return list->free == -1;
}

/* 가용 리스트에서 노드 하나 할당받음
 * Time Complexity: O(1)
 * - malloc과 달리 단순 포인터 변경만 수행
 */
int alloc_node(LinkedList *list) {
    int idx = list->free;
    if (idx != -1) {
        list->free = list->nodes[idx].next;
    }
    return idx;
}

/* 노드를 가용 리스트에 반환
 * Time Complexity: O(1)
 * - free와 달리 단순 연결만 수행
 */
void free_node(LinkedList *list, int idx) {
    list->nodes[idx].next = list->free;
    list->free = idx;
}

/* ============================================================
 * 기본 연산 함수
 * ============================================================ */

/* position 위치의 노드 포인터 반환 (0-based)
 * Time Complexity: O(n) where n = position
 * - 최악의 경우: O(n) (마지막 노드 접근)
 * - 평균: O(n/2) = O(n)
 */
Node *get_entry(LinkedList *list, int position) {
    int i;
    int current;

    if (position < 0) {
        error("Invalid position: negative index");
    }

    current = list->head;
    for (i = 0; i < position && current != -1; i++) {
        current = list->nodes[current].next;
    }

    if (current == -1) {
        error("Invalid position: out of bounds");
    }

    return &(list->nodes[current]);
}

/* 리스트 전체 출력
 * Time Complexity: O(n) where n = 리스트 길이
 */
void print_list(LinkedList *list) {
    int current = list->head;

    printf("LIST [ ");
    while (current != -1) {
        printf("%d ", list->nodes[current].data);
        current = list->nodes[current].next;
    }
    printf("]\n");
}

/* 리스트 길이 반환
 * Time Complexity: O(n) where n = 리스트 길이
 * - 리스트를 처음부터 끝까지 순회
 */
int get_length(LinkedList *list) {
    int count = 0;
    int current = list->head;

    while (current != -1) {
        count++;
        current = list->nodes[current].next;
    }
    return count;
}

/* 리스트 전체 초기화 (모든 노드 삭제)
 * Time Complexity: O(n) where n = 리스트 길이
 * - 모든 노드를 가용 리스트에 반환
 */
void clear(LinkedList *list) {
    int current = list->head;
    int next;

    /* 리스트의 모든 노드를 가용 리스트에 연결 */
    while (current != -1) {
        next = list->nodes[current].next;
        list->nodes[current].next = list->free;
        list->free = current;
        current = next;
    }

    list->head = -1;
}

/* position 위치의 데이터를 item으로 교체
 * Time Complexity: O(n) where n = position
 * - 최악의 경우: O(n)
 * - 평균: O(n/2) = O(n)
 */
void replace(LinkedList *list, int position, int item) {
    Node *node;

    node = get_entry(list, position);
    node->data = item;
}

/* ============================================================
 * 삽입 연산
 * ============================================================ */

/* 리스트 마지막에 노드 삽입
 * Time Complexity: O(n) where n = 리스트 길이
 * - 마지막 노드를 찾기 위해 전체 순회 필요
 */
void insert_last(LinkedList *list, int data) {
    int new_node;
    int current;

    if (is_full(list)) {
        error("List is full");
    }

    new_node = alloc_node(list);
    list->nodes[new_node].data = data;
    list->nodes[new_node].next = -1;

    if (is_empty(list)) {
        list->head = new_node;
    } else {
        current = list->head;
        while (list->nodes[current].next != -1) {
            current = list->nodes[current].next;
        }
        list->nodes[current].next = new_node;
    }
}

/* position 위치에 노드 삽입 (0-based)
 * Time Complexity:
 *   - position == 0: O(1)  (헤드에 삽입)
 *   - position > 0:  O(n)  (n = position)
 * - 최악의 경우: O(n) (마지막에 삽입)
 * - 평균: O(n)
 */
void insert(LinkedList *list, int position, int data) {
    int new_node;
    int current;
    int i;

    if (is_full(list)) {
        error("List is full");
    }

    if (position < 0) {
        error("Invalid position: negative index");
    }

    new_node = alloc_node(list);
    list->nodes[new_node].data = data;

    /* 리스트 처음에 삽입: O(1) */
    if (position == 0) {
        list->nodes[new_node].next = list->head;
        list->head = new_node;
        return;
    }

    /* 중간 또는 마지막에 삽입: O(position) */
    current = list->head;
    for (i = 0; i < position - 1 && current != -1; i++) {
        current = list->nodes[current].next;
    }

    if (current == -1) {
        error("Invalid position: out of bounds");
    }

    list->nodes[new_node].next = list->nodes[current].next;
    list->nodes[current].next = new_node;
}

/* ============================================================
 * 삭제 연산
 * ============================================================ */

/* position 위치의 노드 삭제 (0-based)
 * Time Complexity:
 *   - position == 0: O(1)  (헤드 삭제)
 *   - position > 0:  O(n)  (n = position)
 * - 최악의 경우: O(n) (마지막 노드 삭제)
 * - 평균: O(n)
 */
void delete(LinkedList *list, int position) {
    int current;
    int prev;
    int i;

    if (is_empty(list)) {
        error("List is empty");
    }

    if (position < 0) {
        error("Invalid position: negative index");
    }

    /* 첫 번째 노드 삭제: O(1) */
    if (position == 0) {
        current = list->head;
        list->head = list->nodes[current].next;
        free_node(list, current);
        return;
    }

    /* 그 외 위치 삭제: O(position) */
    prev = list->head;
    for (i = 0; i < position - 1 && prev != -1; i++) {
        prev = list->nodes[prev].next;
    }

    if (prev == -1 || list->nodes[prev].next == -1) {
        error("Invalid position: out of bounds");
    }

    current = list->nodes[prev].next;
    list->nodes[prev].next = list->nodes[current].next;
    free_node(list, current);
}

/* ============================================================
 * 시간 복잡도 요약
 * ============================================================
 * Operation        | Time Complexity | Notes
 * -----------------|-----------------|------------------------
 * init()           | O(n)*           | n=MAX_SIZE, 한 번만 호출
 * is_empty()       | O(1)            |
 * is_full()        | O(1)            |
 * alloc_node()     | O(1)            | malloc보다 빠름
 * free_node()      | O(1)            | free보다 빠름
 * get_entry()      | O(n)            | n=position
 * get_length()     | O(n)            | n=리스트 길이
 * print_list()     | O(n)            | n=리스트 길이
 * clear()          | O(n)            | n=리스트 길이
 * replace()        | O(n)            | n=position
 * insert_last()    | O(n)            | 마지막까지 순회 필요
 * insert(pos, val) | O(n)            | O(1) if pos==0
 * delete(pos)      | O(n)            | O(1) if pos==0
 *
 * * 배열 기반 리스트의 장점:
 *   - 메모리 할당/해제가 O(1)
 *   - 메모리 단편화 없음
 *   - 캐시 친화적 (연속 메모리)
 * ============================================================ */

/* ============================================================
 * 실행 시간 측정 함수
 * ============================================================ */
void benchmark(void) {
    LinkedList list;
    clock_t start, end;
    double cpu_time_used;
    int i;

    printf("\n=== 실행 시간 벤치마크 (Performance Benchmark) ===\n\n");

    /* insert_last 벤치마크 */
    init(&list);
    start = clock();
    for (i = 0; i < 50; i++) {
        insert_last(&list, i);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("insert_last() 50회: %.6f seconds\n", cpu_time_used);

    /* insert (head) 벤치마크 */
    init(&list);
    start = clock();
    for (i = 0; i < 50; i++) {
        insert(&list, 0, i);  // 헤드에 삽입: O(1)
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("insert(pos=0) 50회: %.6f seconds (O(1) per op)\n", cpu_time_used);

    /* get_entry 벤치마크 */
    init(&list);
    for (i = 0; i < 50; i++) {
        insert_last(&list, i);
    }
    start = clock();
    for (i = 0; i < 1000; i++) {
        get_entry(&list, 49);  // 마지막 노드 접근
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("get_entry(49) 1000회: %.6f seconds\n", cpu_time_used);

    /* delete 벤치마크 */
    init(&list);
    for (i = 0; i < 50; i++) {
        insert_last(&list, i);
    }
    start = clock();
    for (i = 49; i >= 0; i--) {
        delete(&list, 0);  // 헤드 삭제: O(1)
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;
    printf("delete(pos=0) 50회: %.6f seconds (O(1) per op)\n", cpu_time_used);
}

/* ============================================================
 * 메인 함수
 * ============================================================ */
int main(void) {
    LinkedList list;

    printf("=== 배열 기반 연결 리스트 (Linked List using Array) ===\n\n");

    init(&list);
    print_list(&list);
    printf("is_empty: %d, is_full: %d, get_length: %d\n\n",
           is_empty(&list), is_full(&list), get_length(&list));

    // insert_last 테스트
    printf(">> insert_last(10), insert_last(20), insert_last(30)\n");
    insert_last(&list, 10);
    insert_last(&list, 20);
    insert_last(&list, 30);
    print_list(&list);
    printf("is_empty: %d, get_length: %d\n\n", is_empty(&list), get_length(&list));

    // get_entry 테스트
    printf(">> get_entry(1)->data: %d\n\n", get_entry(&list, 1)->data);

    // insert 테스트 (중간 삽입)
    printf(">> insert(1, 15)\n");
    insert(&list, 1, 15);
    print_list(&list);
    printf("get_length: %d\n\n", get_length(&list));

    // insert 테스트 (처음 삽입)
    printf(">> insert(0, 5)\n");
    insert(&list, 0, 5);
    print_list(&list);
    printf("\n");

    // replace 테스트
    printf(">> replace(2, 99)\n");
    replace(&list, 2, 99);
    print_list(&list);
    printf("\n");

    // delete 테스트 (중간 삭제)
    printf(">> delete(2)\n");
    delete(&list, 2);
    print_list(&list);
    printf("\n");

    // delete 테스트 (처음 삭제)
    printf(">> delete(0)\n");
    delete(&list, 0);
    print_list(&list);
    printf("\n");

    // delete 테스트 (마지막 삭제)
    printf(">> delete(2)\n");
    delete(&list, 2);
    print_list(&list);
    printf("\n");

    // clear 테스트
    printf(">> clear()\n");
    clear(&list);
    print_list(&list);
    printf("is_empty: %d, get_length: %d\n\n", is_empty(&list), get_length(&list));

    // clear 후 재사용 테스트
    printf(">> clear 후 insert_last(100), insert_last(200)\n");
    insert_last(&list, 100);
    insert_last(&list, 200);
    print_list(&list);
    printf("\n");

    /* 벤치마크 실행 */
    benchmark();

    return 0;
}
