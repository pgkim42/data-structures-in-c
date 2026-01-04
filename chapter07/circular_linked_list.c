#include <stdio.h>
#include <stdlib.h>

/* ============================================================
 * 노드 및 리스트 구조체 정의
 * ============================================================ */
typedef int element;

typedef struct ListNode {
    element data;
    struct ListNode *link;
} ListNode;

typedef struct {
    ListNode *tail;   /* 마지막 노드 (tail->link가 head) */
    int length;
} CircularList;

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
void init(CircularList *list) {
    list->tail = NULL;
    list->length = 0;
}

/* 리스트가 비었는지 확인
 * Time Complexity: O(1)
 */
int is_empty(CircularList *list) {
    return list->tail == NULL;
}

/* 리스트 길이 반환
 * Time Complexity: O(1)
 */
int get_length(CircularList *list) {
    return list->length;
}

/* ============================================================
 * 삽입 연산
 * ============================================================ */

/* 리스트 첫 번째에 노드 삽입
 * Time Complexity: O(1)
 * - tail->link가 head를 가리키므로 바로 접근 가능
 */
void insert_first(CircularList *list, element data) {
    ListNode *node = (ListNode *)malloc(sizeof(ListNode));
    if (node == NULL) {
        error("Memory allocation failed");
    }
    node->data = data;

    if (is_empty(list)) {
        node->link = node;      /* 자기 자신 참조 */
        list->tail = node;
    } else {
        node->link = list->tail->link;  /* 기존 head 앞에 삽입 */
        list->tail->link = node;
    }
    list->length++;
}

/* 리스트 마지막에 노드 삽입
 * Time Complexity: O(1)
 * - tail을 유지하므로 O(1) 가능 (단일 연결 리스트의 O(n)과 대조적)
 */
void insert_last(CircularList *list, element data) {
    ListNode *node = (ListNode *)malloc(sizeof(ListNode));
    if (node == NULL) {
        error("Memory allocation failed");
    }
    node->data = data;

    if (is_empty(list)) {
        node->link = node;      /* 자기 자신 참조 */
        list->tail = node;
    } else {
        node->link = list->tail->link;  /* head를 가리킴 */
        list->tail->link = node;        /* 기존 tail이 새 노드 가리킴 */
        list->tail = node;              /* tail 갱신 */
    }
    list->length++;
}

/* position 위치에 노드 삽입 (0-based)
 * Time Complexity: O(n) where n = position
 */
void insert(CircularList *list, int position, element data) {
    ListNode *node;
    ListNode *p;
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

    node = (ListNode *)malloc(sizeof(ListNode));
    if (node == NULL) {
        error("Memory allocation failed");
    }
    node->data = data;

    /* position-1 위치의 노드 찾기 (head부터 시작) */
    p = list->tail->link;  /* head부터 */
    for (i = 0; i < position - 1; i++) {
        p = p->link;
    }

    node->link = p->link;
    p->link = node;
    list->length++;
}

/* ============================================================
 * 삭제 연산
 * ============================================================ */

/* 리스트 첫 번째 노드 삭제
 * Time Complexity: O(1)
 * - tail->link로 head에 바로 접근
 */
void delete_first(CircularList *list) {
    ListNode *removed;
    ListNode *new_head;

    if (is_empty(list)) {
        error("List is empty");
    }

    removed = list->tail->link;  /* head */

    if (list->tail == removed) {
        /* 단일 노드: 리스트가 비어짐 */
        list->tail = NULL;
    } else {
        new_head = removed->link;
        list->tail->link = new_head;
    }

    free(removed);
    list->length--;
}

/* 리스트 마지막 노드 삭제
 * Time Complexity: O(n) where n = 리스트 길이
 * - tail 이전 노드를 찾기 위해 순회 필요
 */
void delete_last(CircularList *list) {
    ListNode *removed;
    ListNode *prev;

    if (is_empty(list)) {
        error("List is empty");
    }

    removed = list->tail;

    if (removed->link == removed) {
        /* 단일 노드 */
        list->tail = NULL;
    } else {
        /* tail 이전 노드 찾기 */
        prev = list->tail->link;  /* head부터 */
        while (prev->link != list->tail) {
            prev = prev->link;
        }
        prev->link = list->tail->link;  /* head를 가리킴 */
        list->tail = prev;
    }

    free(removed);
    list->length--;
}

/* position 위치의 노드 삭제 (0-based)
 * Time Complexity: O(n) where n = position
 */
void delete(CircularList *list, int position) {
    ListNode *p, *removed;
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

    /* position-1 위치의 노드 찾기 */
    p = list->tail->link;  /* head부터 */
    for (i = 0; i < position - 1; i++) {
        p = p->link;
    }

    removed = p->link;
    p->link = removed->link;
    free(removed);
    list->length--;
}

/* ============================================================
 * 기타 연산
 * ============================================================ */

/* index 위치의 노드 포인터 반환 (0-based)
 * Time Complexity: O(n) where n = index
 */
ListNode *get_entry(CircularList *list, int index) {
    ListNode *p;
    int i;

    if (index < 0 || index >= list->length) {
        error("Invalid index");
    }

    p = list->tail->link;  /* head부터 */
    for (i = 0; i < index; i++) {
        p = p->link;
    }

    return p;
}

/* position 위치의 데이터를 item으로 교체
 * Time Complexity: O(n) where n = position
 */
void replace(CircularList *list, int position, element item) {
    ListNode *node;

    node = get_entry(list, position);
    node->data = item;
}

/* data 값을 가진 노드 탐색
 * Time Complexity: O(n) where n = 리스트 길이
 * - 발견 시: 노드 포인터 반환
 * - 미발견 시: NULL 반환
 */
ListNode *search(CircularList *list, element data) {
    ListNode *p;

    if (is_empty(list)) {
        return NULL;
    }

    p = list->tail->link;  /* head부터 */

    do {
        if (p->data == data) {
            return p;
        }
        p = p->link;
    } while (p != list->tail->link);  /* 한 바퀴 순회 */

    return NULL;
}

/* 리스트 전체 출력
 * Time Complexity: O(n) where n = 리스트 길이
 * - do-while 사용하여 무한 루프 방지
 */
void print_list(CircularList *list) {
    ListNode *p;

    if (is_empty(list)) {
        printf("LIST [ ] (len=0)\n");
        return;
    }

    printf("LIST [ ");
    p = list->tail->link;  /* head부터 */

    do {
        printf("%d ", p->data);
        p = p->link;
    } while (p != list->tail->link);  /* 한 바퀴 순회 */

    printf("] (len=%d)\n", list->length);
}

/* 리스트 전체 초기화 (모든 노드 삭제)
 * Time Complexity: O(n) where n = 리스트 길이
 * - 순회 후 NULL 처리하여 무한 루프 방지
 */
void clear(CircularList *list) {
    ListNode *p;
    ListNode *next;

    if (is_empty(list)) {
        return;
    }

    p = list->tail->link;  /* head부터 */

    /* 리스트 끊어내기 (무한 루프 방지) */
    list->tail->link = NULL;

    while (p != NULL) {
        next = p->link;
        free(p);
        p = next;
    }

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
 * insert_first()   | O(1)            | tail->link로 head 접근
 * insert_last()    | O(1)            | tail 유지 장점
 * insert(pos,val)  | O(n)            | n=position
 * delete_first()   | O(1)            | tail->link로 head 접근
 * delete_last()    | O(n)            | tail 이전 노드 찾음
 * delete(pos)      | O(n)            | n=position
 * get_entry()      | O(n)            | n=index
 * replace()        | O(n)            | n=position
 * search()         | O(n)            | 선형 탐색
 * print_list()     | O(n)            | do-while 사용
 * clear()          | O(n)            |
 *
 * * 원형 연결 리스트의 장점:
 *   - insert_first, insert_last 모두 O(1)
 *   - 어떤 노드에서든 순환 가능
 *   - 응용: CPU 스케줄링 (Round Robin)
 *
 * * 단점:
 *   - delete_last가 O(n)
 *   - 순회 시 종료 조건 주의 필요
 * ============================================================ */

/* ============================================================
 * 메인 함수
 * ============================================================ */
int main(void) {
    CircularList list;
    ListNode *found;

    printf("=== 원형 연결 리스트 (Circular Linked List) ===\n\n");

    init(&list);
    print_list(&list);
    printf("is_empty: %d, get_length: %d\n\n", is_empty(&list), get_length(&list));

    // insert_first 테스트
    printf(">> insert_first(10), insert_first(20), insert_first(30)\n");
    insert_first(&list, 10);
    insert_first(&list, 20);
    insert_first(&list, 30);
    print_list(&list);
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

    // search 테스트
    printf(">> search(25)\n");
    found = search(&list, 25);
    if (found != NULL) {
        printf("Found: %d\n\n", found->data);
    } else {
        printf("Not found\n\n");
    }

    printf(">> search(999)\n");
    found = search(&list, 999);
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
