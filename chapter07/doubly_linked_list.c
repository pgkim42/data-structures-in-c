#include <stdio.h>
#include <stdlib.h>

/* ============================================================
 * 노드 및 리스트 구조체 정의
 * ============================================================ */
typedef int element;

typedef struct DListNode {
    element data;
    struct DListNode *prev;
    struct DListNode *next;
} DListNode;

typedef struct {
    DListNode *head;
    DListNode *tail;
    int length;
} DoublyList;

/* ============================================================
 * 에러 처리 함수
 * ============================================================ */
void error(const char *message) {
    fprintf(stderr, "[ERROR] %s\n", message);
    exit(1);
}

/* ============================================================
 * 기본 연산
 * ============================================================ */

/* 리스트 초기화
 * Time Complexity: O(1)
 */
void init(DoublyList *list) {
    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
}

/* 리스트가 비었는지 확인
 * Time Complexity: O(1)
 */
int is_empty(DoublyList *list) {
    return list->head == NULL;
}

/* 리스트 길이 반환
 * Time Complexity: O(1)
 */
int get_length(DoublyList *list) {
    return list->length;
}

/* ============================================================
 * 삽입 연산
 * ============================================================ */

/* 리스트 첫 번째에 노드 삽입
 * Time Complexity: O(1)
 */
void insert_first(DoublyList *list, element data) {
    DListNode *node = (DListNode *)malloc(sizeof(DListNode));
    if (node == NULL) {
        error("Memory allocation failed");
    }
    node->data = data;
    node->prev = NULL;
    node->next = list->head;

    if (is_empty(list)) {
        list->tail = node;
    } else {
        list->head->prev = node;
    }
    list->head = node;
    list->length++;
}

/* 리스트 마지막에 노드 삽입
 * Time Complexity: O(1)
 */
void insert_last(DoublyList *list, element data) {
    DListNode *node = (DListNode *)malloc(sizeof(DListNode));
    if (node == NULL) {
        error("Memory allocation failed");
    }
    node->data = data;
    node->prev = list->tail;
    node->next = NULL;

    if (is_empty(list)) {
        list->head = node;
    } else {
        list->tail->next = node;
    }
    list->tail = node;
    list->length++;
}

/* position 위치에 노드 삽입 (0-based)
 * Time Complexity: O(n) where n = position
 */
void insert(DoublyList *list, int position, element data) {
    DListNode *node;
    DListNode *p;
    int i;

    if (position < 0 || position > list->length) {
        error("Invalid position");
    }

    if (position == 0) {
        insert_first(list, data);
        return;
    }

    if (position == list->length) {
        insert_last(list, data);
        return;
    }

    node = (DListNode *)malloc(sizeof(DListNode));
    if (node == NULL) {
        error("Memory allocation failed");
    }
    node->data = data;

    /* position-1 위치의 노드 찾기 */
    p = list->head;
    for (i = 0; i < position - 1; i++) {
        p = p->next;
    }

    node->prev = p;
    node->next = p->next;
    p->next->prev = node;
    p->next = node;
    list->length++;
}

/* ============================================================
 * 삭제 연산
 * ============================================================ */

/* 리스트 첫 번째 노드 삭제
 * Time Complexity: O(1)
 */
void delete_first(DoublyList *list) {
    DListNode *removed;

    if (is_empty(list)) {
        error("List is empty");
    }

    removed = list->head;
    list->head = removed->next;

    if (list->head == NULL) {
        /* 리스트가 비어짐 */
        list->tail = NULL;
    } else {
        list->head->prev = NULL;
    }

    free(removed);
    list->length--;
}

/* 리스트 마지막 노드 삭제
 * Time Complexity: O(1)
 * - prev 포인터로 바로 접근 가능 (단일 연결 리스트의 O(n)과 대조적)
 */
void delete_last(DoublyList *list) {
    DListNode *removed;

    if (is_empty(list)) {
        error("List is empty");
    }

    removed = list->tail;
    list->tail = removed->prev;

    if (list->tail == NULL) {
        /* 리스트가 비어짐 */
        list->head = NULL;
    } else {
        list->tail->next = NULL;
    }

    free(removed);
    list->length--;
}

/* position 위치의 노드 삭제 (0-based)
 * Time Complexity: O(n) where n = position
 */
void delete(DoublyList *list, int position) {
    DListNode *removed;
    int i;

    if (is_empty(list)) {
        error("List is empty");
    }

    if (position < 0 || position >= list->length) {
        error("Invalid position");
    }

    if (position == 0) {
        delete_first(list);
        return;
    }

    if (position == list->length - 1) {
        delete_last(list);
        return;
    }

    /* position 위치의 노드 찾기 */
    removed = list->head;
    for (i = 0; i < position; i++) {
        removed = removed->next;
    }

    removed->prev->next = removed->next;
    removed->next->prev = removed->prev;
    free(removed);
    list->length--;
}

/* ============================================================
 * 기타 연산
 * ============================================================ */

/* index 위치의 노드 포인터 반환 (0-based)
 * Time Complexity: O(n) where n = index
 */
DListNode *get_entry(DoublyList *list, int index) {
    DListNode *p;
    int i;

    if (index < 0 || index >= list->length) {
        error("Invalid index");
    }

    p = list->head;
    for (i = 0; i < index; i++) {
        p = p->next;
    }

    return p;
}

/* position 위치의 데이터를 item으로 교체
 * Time Complexity: O(n) where n = position
 */
void replace(DoublyList *list, int position, element item) {
    DListNode *node;

    node = get_entry(list, position);
    node->data = item;
}

/* data 값을 가진 노드 탐색
 * Time Complexity: O(n) where n = 리스트 길이
 */
DListNode *search(DoublyList *list, element data) {
    DListNode *p = list->head;

    while (p != NULL) {
        if (p->data == data) {
            return p;
        }
        p = p->next;
    }

    return NULL;
}

/* 리스트 전체 출력 (순방향)
 * Time Complexity: O(n)
 */
void print_list(DoublyList *list) {
    DListNode *p = list->head;

    printf("LIST [ ");
    while (p != NULL) {
        printf("%d ", p->data);
        p = p->next;
    }
    printf("] (len=%d)\n", list->length);
}

/* 리스트 전체 출력 (역방향)
 * Time Complexity: O(n)
 * - tail부터 prev로 순회
 */
void print_reverse(DoublyList *list) {
    DListNode *p = list->tail;

    printf("LIST REVERSE [ ");
    while (p != NULL) {
        printf("%d ", p->data);
        p = p->prev;
    }
    printf("] (len=%d)\n", list->length);
}

/* 리스트 역순 (head와 tail 교체)
 * Time Complexity: O(1)
 * - 링크가 이미 양방향이므로 head와 tail만 swap하면 됨
 * - 단일 연결 리스트의 O(n)과 대조적
 */
void reverse(DoublyList *list) {
    DListNode *temp;

    temp = list->head;
    list->head = list->tail;
    list->tail = temp;
}

/* 리스트 전체 초기화 (모든 노드 삭제)
 * Time Complexity: O(n)
 */
void clear(DoublyList *list) {
    DListNode *p = list->head;
    DListNode *next;

    while (p != NULL) {
        next = p->next;
        free(p);
        p = next;
    }

    list->head = NULL;
    list->tail = NULL;
    list->length = 0;
}

/* ============================================================
 * 시간 복잡도 요약
 * ============================================================
 * Operation        | Time Complexity | Notes
 * -----------------|-----------------|------------------------
 * init()           | O(1)            |
 * is_empty()       | O(1)            |
 * get_length()     | O(1)            |
 * insert_first()   | O(1)            |
 * insert_last()    | O(1)            |
 * insert(pos,val)  | O(n)            | n=position
 * delete_first()   | O(1)            |
 * delete_last()    | O(1)            | prev로 접근
 * delete(pos)      | O(n)            | n=position
 * get_entry()      | O(n)            | n=index
 * replace()        | O(n)            | n=position
 * search()         | O(n)            | 선형 탐색
 * print_list()     | O(n)            | 순방향
 * print_reverse()  | O(n)            | 역방향 (tail부터)
 * reverse()        | O(1)            | head, tail만 swap!
 * clear()          | O(n)            |
 *
 * * 이중 연결 리스트의 장점:
 *   - insert_first/last, delete_first/last 모두 O(1)
 *   - 양방향 순회 가능
 *   - reverse가 O(1)
 *   - 역방향 출력 가능
 *
 * * 단점:
 *   - 포인터가 2개라 메모리 증가
 *   - 링크 관리가 복잡함
 * ============================================================ */

/* ============================================================
 * 메인 함수
 * ============================================================ */
int main(void) {
    DoublyList list;
    DListNode *found;

    printf("=== 이중 연결 리스트 (Doubly Linked List) ===\n\n");

    init(&list);
    print_list(&list);
    print_reverse(&list);
    printf("is_empty: %d, get_length: %d\n\n", is_empty(&list), get_length(&list));

    // insert_first 테스트
    printf(">> insert_first(10), insert_first(20), insert_first(30)\n");
    insert_first(&list, 10);
    insert_first(&list, 20);
    insert_first(&list, 30);
    print_list(&list);
    print_reverse(&list);
    printf("is_empty: %d, get_length: %d\n\n", is_empty(&list), get_length(&list));

    // insert_last 테스트
    printf(">> insert_last(40), insert_last(50)\n");
    insert_last(&list, 40);
    insert_last(&list, 50);
    print_list(&list);
    printf("\n");

    // insert 테스트 (중간 삽입)
    printf(">> insert(2, 25)\n");
    insert(&list, 2, 25);
    print_list(&list);
    printf("\n");

    // get_entry 테스트
    printf(">> get_entry(3)->data: %d\n\n", get_entry(&list, 3)->data);

    // replace 테스트
    printf(">> replace(1, 99)\n");
    replace(&list, 1, 99);
    print_list(&list);
    printf("\n");

    // reverse 테스트
    printf(">> reverse()\n");
    reverse(&list);
    print_list(&list);
    print_reverse(&list);
    printf("\n");

    // 다시 reverse
    printf(">> reverse() (원상복구)\n");
    reverse(&list);
    print_list(&list);
    printf("\n");

    // search 테스트
    printf(">> search(25)\n");
    found = search(&list, 25);
    if (found != NULL) {
        printf("Found: %d\n\n", found->data);
    } else {
        printf("Not found\n\n");
    }

    // delete_first 테스트
    printf(">> delete_first()\n");
    delete_first(&list);
    print_list(&list);
    printf("\n");

    // delete_last 테스트
    printf(">> delete_last()\n");
    delete_last(&list);
    print_list(&list);
    printf("\n");

    // delete 테스트 (중간 삭제)
    printf(">> delete(1)\n");
    delete(&list, 1);
    print_list(&list);
    printf("\n");

    // clear 테스트
    printf(">> clear()\n");
    clear(&list);
    print_list(&list);
    printf("is_empty: %d, get_length: %d\n", is_empty(&list), get_length(&list));

    return 0;
}
