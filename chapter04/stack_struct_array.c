/*
 * stack_struct_array.c: 구조체 스택 (배열 기반 구현)
 *
 * 정수 대신 구조체(element)를 저장하는 스택
 * - 학생 정보 (학번, 이름, 주소)를 요소로 관리
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_SIZE 100
#define MAX_STRING 100

/* 학생 구조체 정의 */
typedef struct {
    int student_no;
    char name[MAX_STRING];
    char address[MAX_STRING];
} element;

/* 전역 변수: 스택 배열과 top 인덱스 */
element stack[MAX_STACK_SIZE];
int top = -1;

/* 공백 상태 검출 */
int is_empty(void) {
    return top == -1;
}

/* 포화 상태 검출 */
int is_full(void) {
    return top == MAX_STACK_SIZE - 1;
}

/*
 * 삽입 연산: 구조체 요소를 스택에 추가
 *
 * 에러 처리: 스택이 가득 차면 return (회복 가능)
 */
void push(const element item) {
    if (is_full()) {
        fprintf(stderr, "스택 포화 상태\n");
        return;  // 추가 실패해도 계속 실행 가능
    }
    stack[++top] = item;
}

/*
 * 삭제 연산: top 요소 제거 후 반환
 *
 * 에러 처리: 스택이 비면 exit(1) (회복 불가능)
 * - 반환할 유효한 구조체가 없으므로 종료
 */
element pop(void) {
    if (is_empty()) {
        fprintf(stderr, "스택 공백 상태\n");
        exit(1);  // 반환할 값 없음, 종료가 안전
    }
    return stack[top--];
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
    printf("===== 구조체 스택 =====\n\n");

    /* 입력 요소: 학생 정보 */
    element ie = {20190001, "Hong", "Seoul"};
    element oe;

    push(ie);
    oe = pop();

    printf("학번: %d\n", oe.student_no);
    printf("이름: %s\n", oe.name);
    printf("주소: %s\n", oe.address);

    return 0;
}
