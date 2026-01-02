/*
 * stack_array.c: 일반적인 배열 스택 (구조체 캡슐화 + 동적 할당)
 *
 * 스택을 구조체로 캡슐화하고 동적으로 할당하여 사용
 * - 전역 변수 없음
 * - 여러 스택을 독립적으로 생성 가능
 * - 필요할 때 malloc으로 생성, free로 해제
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_SIZE 100

typedef int element;

/* 스택 구조체: 데이터와 top을 하나로 묶음 */
typedef struct {
    element data[MAX_STACK_SIZE];
    int top;
} StackType;

/* 스택 초기화 */
void init_stack(StackType *s) {
    s->top = -1;
}

/* 공백 상태 검출 */
int is_empty(StackType *s) {
    return s->top == -1;
}

/* 포화 상태 검출 */
int is_full(StackType *s) {
    return s->top == MAX_STACK_SIZE - 1;
}

/*
 * 삽입 연산
 *
 * 에러 처리: 스택이 가득 차면 return (회복 가능)
 */
void push(StackType *s, element item) {
    if (is_full(s)) {
        fprintf(stderr, "스택 포화 상태\n");
        return;
    }
    s->data[++s->top] = item;
}

/*
 * 삭제 연산
 *
 * 에러 처리: 스택이 비면 exit(1) (회복 불가능)
 */
element pop(StackType *s) {
    if (is_empty(s)) {
        fprintf(stderr, "스택 공백 상태\n");
        exit(1);
    }
    return s->data[s->top--];
}

/*
 * 조회 연산: top 요소 확인 (제거 없음)
 */
element peek(StackType *s) {
    if (is_empty(s)) {
        fprintf(stderr, "스택 공백 상태\n");
        exit(1);
    }
    return s->data[s->top];
}

int main(void) {
    /* 스택을 동적으로 할당 */
    StackType *s = malloc(sizeof(StackType));
    if (s == NULL) {
        fprintf(stderr, "메모리 할당 실패\n");
        return 1;
    }

    printf("===== 일반적인 배열 스택 =====\n\n");

    init_stack(s);  /* 스택 초기화 (필수!) */

    push(s, 1);
    push(s, 2);
    push(s, 3);

    printf("pop: %d\n", pop(s));  // 3
    printf("pop: %d\n", pop(s));  // 2
    printf("pop: %d\n", pop(s));  // 1

    /* 메모리 반납 */
    free(s);
    s = NULL;

    return 0;
}
