/*
 * stack_integer.c: 정수 스택 (배열 기반 구현)
 *
 * 스택: LIFO (Last In First Out) 자료구조
 * - push: top 위치에 요소 추가
 * - pop: top 요소 제거 후 반환
 * - peek: top 요소 확인 (제거 없음)
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_SIZE 100

typedef int element;

/* 전역 변수: 스택 배열과 top 인덱스 */
element stack[MAX_STACK_SIZE];
int top = -1;  /* -1이면 공백 상태 */

/* 공백 상태 검출 */
int is_empty(void) {
    return top == -1;
}

/* 포화 상태 검출 */
int is_full(void) {
    return top == MAX_STACK_SIZE - 1;
}

/*
 * 삽입 연산: top 다음 위치에 요소 추가
 *
 * 에러 처리: 스택이 가득 차면 return (회복 가능)
 * - push는 "추가 안 하면 그만"이므로 프로그램 계속 실행 가능
 */
void push(element item) {
    if (is_full()) {
        fprintf(stderr, "스택 포화 상태\n");
        return;  // 추가 실패해도 계속 실행 가능
    }
    stack[++top] = item;  /* top 증가 후 저장 */
}

/*
 * 삭제 연산: top 요소 제거 후 반환
 *
 * 에러 처리: 스택이 비면 exit(1) (회복 불가능)
 * - pop는 "반환할 값이 없음" - 무엇을 반환할까?
 * - 쓰레기 값 반환하면 버그 추정이 어려우므로 종료
 */
element pop(void) {
    if (is_empty()) {
        fprintf(stderr, "스택 공백 상태\n");
        exit(1);  // 반환할 유효한 값 없음, 종료가 안전
    }
    return stack[top--];  /* 반환 후 top 감소 */
}

/*
 * 조회 연산: top 요소 확인 (제거 없음)
 */
element peek(void) {
    if (is_empty()) {
        fprintf(stderr, "스택 공백 상태\n");
        exit(1);
    }
    return stack[top];
}

int main(void) {
    printf("===== 정수 스택 =====\n\n");

    push(1);
    push(2);
    push(3);

    printf("pop: %d\n", pop());  // 3
    printf("pop: %d\n", pop());  // 2
    printf("pop: %d\n", pop());  // 1

    return 0;
}
