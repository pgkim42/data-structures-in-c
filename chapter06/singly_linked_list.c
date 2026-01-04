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
    ListNode *head;
    int length;
} ListType;

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
 * Time Complexity: O(1)
 * ============================================================ */
void init(ListType *L) {
    L->head = NULL;
    L->length = 0;
}

/* ============================================================
 * 유틸리티 함수
 * ============================================================ */

/* 리스트가 비었는지 확인
 * Time Complexity: O(1)
 */
int is_empty(ListType *L) {
    return L->head == NULL;
}

/* 리스트 길이 반환
 * Time Complexity: O(1)
 * - length 멤버 변수를 사용하므로 상수 시간
 */
int get_length(ListType *L) {
    return L->length;
}

/* ============================================================
 * 삽입 연산
 * ============================================================ */

/* 리스트 첫 번째에 노드 삽입
 * Time Complexity: O(1)
 * - 헤드 포인터만 변경하면 됨
 */
void insert_first(ListType *L, element data) {
    ListNode *node = (ListNode *)malloc(sizeof(ListNode));
    if (node == NULL) {
        error("Memory allocation failed");
    }

    node->data = data;
    node->link = L->head;
    L->head = node;
    L->length++;
}

/* position 위치에 노드 삽입 (0-based)
 * Time Complexity:
 *   - position == 0: O(1)
 *   - position > 0:  O(n) where n = position
 */
void insert(ListType *L, int position, element data) {
    ListNode *node;
    ListNode *p;
    int i;

    if (position < 0 || position > L->length) {
        error("Invalid position");
    }

    if (position == 0) {
        insert_first(L, data);
        return;
    }

    node = (ListNode *)malloc(sizeof(ListNode));
    if (node == NULL) {
        error("Memory allocation failed");
    }
    node->data = data;

    /* position-1 위치의 노드 찾기 */
    p = L->head;
    for (i = 0; i < position - 1; i++) {
        p = p->link;
    }

    node->link = p->link;
    p->link = node;
    L->length++;
}

/* ============================================================
 * 삭제 연산
 * ============================================================ */

/* 리스트 첫 번째 노드 삭제
 * Time Complexity: O(1)
 * - 헤드 포인터만 변경하면 됨
 */
void delete_first(ListType *L) {
    ListNode *removed;

    if (is_empty(L)) {
        error("List is empty");
    }

    removed = L->head;
    L->head = L->head->link;
    free(removed);
    L->length--;
}

/* position 위치의 노드 삭제 (0-based)
 * Time Complexity:
 *   - position == 0: O(1)
 *   - position > 0:  O(n) where n = position
 */
void delete(ListType *L, int position) {
    ListNode *p;
    ListNode *removed;
    int i;

    if (is_empty(L)) {
        error("List is empty");
    }

    if (position < 0 || position >= L->length) {
        error("Invalid position");
    }

    if (position == 0) {
        delete_first(L);
        return;
    }

    /* position-1 위치의 노드 찾기 */
    p = L->head;
    for (i = 0; i < position - 1; i++) {
        p = p->link;
    }

    removed = p->link;
    p->link = removed->link;
    free(removed);
    L->length--;
}

/* ============================================================
 * 기본 연산 함수
 * ============================================================ */

/* index 위치의 노드 포인터 반환 (0-based)
 * Time Complexity: O(n) where n = index
 * - 최악의 경우: O(n)
 * - 평균: O(n/2) = O(n)
 */
ListNode *get_entry(ListType *L, int index) {
    ListNode *p;
    int i;

    if (index < 0 || index >= L->length) {
        error("Invalid index");
    }

    p = L->head;
    for (i = 0; i < index; i++) {
        p = p->link;
    }

    return p;
}

/* 리스트 전체 출력
 * Time Complexity: O(n) where n = 리스트 길이
 */
void print_list(ListType *L) {
    ListNode *p = L->head;

    printf("LIST [ ");
    while (p != NULL) {
        printf("%d ", p->data);
        p = p->link;
    }
    printf("] (len=%d)\n", L->length);
}

/* ============================================================
 * 메모리 해제 함수
 * ============================================================ */
void clear(ListType *L) {
    while (!is_empty(L)) {
        delete_first(L);
    }
}

/* ============================================================
 * 탐색 및 병합 함수
 * ============================================================ */

/* data 값을 가진 노드 탐색
 * Time Complexity: O(n) where n = 리스트 길이
 * - 발견 시: 노드 포인터 반환
 * - 미발견 시: NULL 반환
 */
ListNode *search_list(ListType *L, element data) {
    ListNode *p = L->head;

    while (p != NULL) {
        if (p->data == data) {
            return p;
        }
        p = p->link;
    }

    return NULL;  /* 찾지 못함 */
}

/* 두 리스트를 하나로 합침 (L1 뒤에 L2를 연결)
 * Time Complexity: O(n) where n = L1의 길이
 * - L1의 마지막 노드를 찾아야 하므로 O(n)
 * - L2는 연결 후 초기화됨
 */
void concat_list(ListType *L1, ListType *L2) {
    ListNode *p;

    /* L1이 비어있으면 L2의 헤드를 L1에 복사 */
    if (is_empty(L1)) {
        L1->head = L2->head;
        L1->length = L2->length;
    } else {
        /* L1의 마지막 노드 찾기 */
        p = L1->head;
        while (p->link != NULL) {
            p = p->link;
        }
        /* L2를 L1 뒤에 연결 */
        p->link = L2->head;
        L1->length += L2->length;
    }

    /* L2 초기화 */
    L2->head = NULL;
    L2->length = 0;
}

/* 리스트를 역순으로 만듦
 * Time Complexity: O(n) where n = 리스트 길이
 * - 세 개의 포인터(prev, current, next)를 사용하여 링크를 뒤집음
 * - 공간 복잡도: O(1) (추가 메모리 없음)
 */
void reverse(ListType *L) {
    ListNode *prev = NULL;
    ListNode *current = L->head;
    ListNode *next;

    while (current != NULL) {
        next = current->link;   /* 다음 노드 저장 */
        current->link = prev;   /* 링크 역방향으로 변경 */
        prev = current;         /* prev를 현재 노드로 이동 */
        current = next;         /* current를 다음 노드로 이동 */
    }

    L->head = prev;  /* 헤드를 마지막 노드로 변경 */
}

/* ============================================================
 * 시간 복잡도 요약
 * ============================================================
 * Operation        | Time Complexity | Notes
 * -----------------|-----------------|------------------------
 * init()           | O(1)            |
 * is_empty()       | O(1)            |
 * get_length()     | O(1)            | length 멤버 사용
 * insert_first()   | O(1)            |
 * insert(pos,val)  | O(n)            | O(1) if pos==0
 * delete_first()   | O(1)            |
 * delete(pos)      | O(n)            | O(1) if pos==0
 * get_entry()      | O(n)            | n=index
 * print_list()     | O(n)            |
 * clear()          | O(n)            |
 * search_list()    | O(n)            | 선형 탐색
 * concat_list()    | O(n)            | n=L1 길이
 * reverse()        | O(n)            | 공간 O(1), 제자리 뒤집기
 *
 * * 포인터 기반 연결 리스트의 장점:
 *   - 동적 크기: 제한 없이 노드 추가 가능
 *   - 삽입/삭제: O(1) if 헤드에서 수행
 *   - 유연성: 런타임에 크기 조절 가능
 *
 * * 단점:
 *   - malloc/free 오버헤드
 *   - 캐시 비친화적 (비연속 메모리)
 *   - 포인터 추가 메모리
 * ============================================================ */

/* ============================================================
 * 메인 함수
 * ============================================================ */
int main(void) {
    ListType list, list1, list2;
    ListNode *found;

    printf("=== 단일 연결 리스트 (Singly Linked List) ===\n\n");

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

    // insert 테스트 (중간 삽입)
    printf(">> insert(1, 15)\n");
    insert(&list, 1, 15);
    print_list(&list);
    printf("\n");

    // insert 테스트 (마지막 삽입)
    printf(">> insert(4, 5)\n");
    insert(&list, 4, 5);
    print_list(&list);
    printf("\n");

    // get_entry 테스트
    printf(">> get_entry(2)->data: %d\n\n", get_entry(&list, 2)->data);

    // delete_first 테스트
    printf(">> delete_first()\n");
    delete_first(&list);
    print_list(&list);
    printf("\n");

    // delete 테스트 (중간 삭제)
    printf(">> delete(2)\n");
    delete(&list, 2);
    print_list(&list);
    printf("\n");

    // delete 테스트 (마지막 삭제)
    printf(">> delete(2)\n");
    delete(&list, 2);
    print_list(&list);
    printf("\n");

    // search_list 테스트
    printf(">> search_list(15)\n");
    found = search_list(&list, 15);
    if (found != NULL) {
        printf("Found: %d\n\n", found->data);
    } else {
        printf("Not found\n\n");
    }

    printf(">> search_list(999)\n");
    found = search_list(&list, 999);
    if (found != NULL) {
        printf("Found: %d\n\n", found->data);
    } else {
        printf("Not found\n\n");
    }

    // clear 테스트
    printf(">> clear()\n");
    clear(&list);
    print_list(&list);
    printf("is_empty: %d, get_length: %d\n\n", is_empty(&list), get_length(&list));

    // concat_list 테스트
    printf("=== concat_list 테스트 ===\n");
    init(&list1);
    init(&list2);

    insert_first(&list1, 10);
    insert_first(&list1, 20);
    insert_first(&list1, 30);
    printf("List1: ");
    print_list(&list1);

    insert_first(&list2, 40);
    insert_first(&list2, 50);
    printf("List2: ");
    print_list(&list2);

    printf(">> concat_list(&list1, &list2)\n");
    concat_list(&list1, &list2);
    printf("List1: ");
    print_list(&list1);
    printf("List2: ");
    print_list(&list2);

    // reverse 테스트
    printf("\n=== reverse 테스트 ===\n");
    printf(">> reverse(&list1)\n");
    reverse(&list1);
    printf("List1: ");
    print_list(&list1);

    return 0;
}
