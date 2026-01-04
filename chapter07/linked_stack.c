#include <stdio.h>
#include <stdlib.h>

/* ============================================================
 * 노드 및 스택 구조체 정의
 * ============================================================ */
typedef int element;

typedef struct StackNode {
    element data;
    struct StackNode *link;
} StackNode;

typedef struct {
    StackNode *top;
} StackType;

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

/* 스택 초기화
 * Time Complexity: O(1)
 */
void init_stack(StackType *s) {
    s->top = NULL;
}

/* 스택이 비었는지 확인
 * Time Complexity: O(1)
 */
int is_empty(StackType *s) {
    return s->top == NULL;
}

/* 스택이 가득 찼는지 확인
 * Time Complexity: O(1)
 * - 동적 할당이므로 항상 0 반환 (가득 찰 일 없음)
 */
int is_full(StackType *s) {
    return 0;
}

/* ============================================================
 * 삽입 연산
 * ============================================================ */

/* 스택에 원소 삽입 (push)
 * Time Complexity: O(1)
 * - top에 노드 삽입 (단일 연결 리스트의 insert_first)
 */
void push(StackType *s, element item) {
    StackNode *new_node = (StackNode *)malloc(sizeof(StackNode));
    if (new_node == NULL) {
        error("Memory allocation failed");
    }
    new_node->data = item;
    new_node->link = s->top;  /* 기존 top을 가리킴 */
    s->top = new_node;        /* top이 새 노드를 가리킴 */
}

/* ============================================================
 * 삭제 연산
 * ============================================================ */

/* 스택에서 원소 삭제 및 반환 (pop)
 * Time Complexity: O(1)
 * - top 노드 삭제 후 데이터 반환
 */
element pop(StackType *s) {
    StackNode *removed;
    element data;

    if (is_empty(s)) {
        error("Stack is empty");
    }

    removed = s->top;
    data = removed->data;

    s->top = removed->link;  /* top이 다음 노드를 가리킴 */
    free(removed);

    return data;
}

/* ============================================================
 * 조회 연산
 * ============================================================ */

/* 스택의 top 원소 반환 (삭제 없음)
 * Time Complexity: O(1)
 */
element peek(StackType *s) {
    if (is_empty(s)) {
        error("Stack is empty");
    }
    return s->top->data;
}

/* ============================================================
 * 유틸리티 연산
 * ============================================================ */

/* 스택의 모든 원소 삭제
 * Time Complexity: O(n) where n = 스택 크기
 */
void clear(StackType *s) {
    StackNode *current = s->top;
    StackNode *next;

    while (current != NULL) {
        next = current->link;
        free(current);
        current = next;
    }

    s->top = NULL;
}

/* ============================================================
 * 시간 복잡도 요약
 * ============================================================
 * Operation    | Time Complexity | Notes
 * -------------|-----------------|------------------------
 * init_stack   | O(1)            |
 * is_empty     | O(1)            |
 * is_full      | O(1)            | 항상 0 반환 (동적 할당)
 * push         | O(1)            | top에 삽입
 * pop          | O(1)            | top 삭제 후 반환
 * peek         | O(1)            | top 조회 (미삭제)
 * clear        | O(n)            | 전체 삭제
 *
 * * 연결 리스트 기반 스택의 장점:
 *   - 크기 제한 없음 (동적 할당)
 *   - is_full 불필요
 *   - 모든 연산이 O(1)
 *
 * * 단점:
 *   - malloc/free 오버헤드
 *   - 캐시 비친화적
 * ============================================================ */

/* ============================================================
 * 메인 함수
 * ============================================================ */
int main(void) {
    StackType s;

    printf("=== 연결 리스트 기반 스택 (Linked Stack) ===\n\n");

    init_stack(&s);
    printf("초기화: is_empty = %d, is_full = %d\n\n", is_empty(&s), is_full(&s));

    // push 테스트
    printf(">> push(10), push(20), push(30)\n");
    push(&s, 10);
    push(&s, 20);
    push(&s, 30);
    printf("peek: %d\n", peek(&s));
    printf("is_empty: %d\n\n", is_empty(&s));

    // pop 테스트
    printf(">> pop() = %d\n", pop(&s));
    printf(">> pop() = %d\n", pop(&s));
    printf("peek: %d\n\n", peek(&s));

    // push 추가
    printf(">> push(40), push(50)\n");
    push(&s, 40);
    push(&s, 50);
    printf("peek: %d\n\n", peek(&s));

    // 전체 pop
    printf(">> 전체 pop:\n");
    while (!is_empty(&s)) {
        printf("pop: %d\n", pop(&s));
    }
    printf("\n");

    printf("is_empty: %d\n", is_empty(&s));

    return 0;
}
