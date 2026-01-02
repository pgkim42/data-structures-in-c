/*
 * stack_dynamic_array.c: 동적 배열 스택 (구조체 + 동적 배열)
 *
 * 스택 데이터를 동적으로 할당하여 크기 조절 가능
 * - 고정 크기 제한 없음
 * - 필요시 용량 확장 가능 (resize)
 */

#include <stdio.h>
#include <stdlib.h>

#define INIT_CAPACITY 10  /* 초기 용량 */

typedef int element;

/* 스택 구조체: 동적 배열 + 크기 정보 */
typedef struct {
    element *data;     /* 동적 배열 포인터 */
    int top;           /* top 인덱스 */
    int capacity;      /* 현재 용량 */
} StackType;

/* 스택 초기화 */
void init_stack(StackType *s) {
    s->data = malloc(INIT_CAPACITY * sizeof(element));
    if (s->data == NULL) {
        fprintf(stderr, "메모리 할당 실패\n");
        exit(1);
    }
    s->top = -1;
    s->capacity = INIT_CAPACITY;
}

/* 스택 메모리 해제 */
void cleanup_stack(StackType *s) {
    free(s->data);
    s->data = NULL;
    s->top = -1;
    s->capacity = 0;
}

/* 공백 상태 검출 */
int is_empty(StackType *s) {
    return s->top == -1;
}

/* 포화 상태 검출 */
int is_full(StackType *s) {
    return s->top == s->capacity - 1;
}

/*
 * 용량 확장 (내부 함수)
 * 기존 용량의 2배로 확장
 */
void resize_stack(StackType *s) {
    int new_capacity = s->capacity * 2;
    element *new_data = realloc(s->data, new_capacity * sizeof(element));

    if (new_data == NULL) {
        fprintf(stderr, "메모리 재할당 실패\n");
        free(s->data);
        exit(1);
    }

    s->data = new_data;
    s->capacity = new_capacity;
    printf("[용량 확장: %d → %d]\n", s->capacity / 2, s->capacity);
}

/*
 * 삽입 연산
 *
 * 에러 처리: 포화 시 자동 용량 확장
 */
void push(StackType *s, element item) {
    if (is_full(s)) {
        resize_stack(s);  /* 자동 확장 */
    }
    s->data[++s->top] = item;
}

/*
 * 삭제 연산
 *
 * 에러 처리: 공백 시 exit(1)
 */
element pop(StackType *s) {
    if (is_empty(s)) {
        fprintf(stderr, "스택 공백 상태\n");
        exit(1);
    }
    return s->data[s->top--];
}

/*
 * 조회 연산: top 요소 확인
 */
element peek(StackType *s) {
    if (is_empty(s)) {
        fprintf(stderr, "스택 공백 상태\n");
        exit(1);
    }
    return s->data[s->top];
}

/* 현재 크기 정보 출력 */
void print_stack_info(StackType *s) {
    printf("용량: %d, 요소 개수: %d\n", s->capacity, s->top + 1);
}

int main(void) {
    StackType s;

    printf("===== 동적 배열 스택 =====\n\n");

    init_stack(&s);
    print_stack_info(&s);

    /* 15개 push (초기 용량 10 초과 → 자동 확장) */
    for (int i = 1; i <= 15; i++) {
        push(&s, i);
    }
    print_stack_info(&s);

    printf("\npop 결과:\n");
    for (int i = 0; i < 5; i++) {
        printf("pop: %d\n", pop(&s));
    }
    print_stack_info(&s);

    cleanup_stack(&s);

    return 0;
}
