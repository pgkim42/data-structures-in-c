/*
 * bracket_checker.c: 괄호 짝 검사 (Bracket Matching)
 *
 * 스택을 활용한 전형적인 응용 문제
 * - 열린 괄호: 스택에 push
 * - 닫힌 괄호: 스택에서 pop 후 짝 비교
 * - 끝까지 검사 후 스택이 비어 있으면 성공
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STACK_SIZE 100

typedef char element;

/* 스택 구조체 */
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

/* 삽입 */
void push(StackType *s, element item) {
    if (is_full(s)) {
        fprintf(stderr, "스택 포화 상태\n");
        return;
    }
    s->data[++s->top] = item;
}

/* 삭제 */
element pop(StackType *s) {
    if (is_empty(s)) {
        fprintf(stderr, "스택 공백 상태\n");
        exit(1);
    }
    return s->data[s->top--];
}

/* 조회 */
element peek(StackType *s) {
    if (is_empty(s)) {
        fprintf(stderr, "스택 공백 상태\n");
        exit(1);
    }
    return s->data[s->top];
}

/*
 * 괄호 짝이 맞는지 확인
 */
int is_matching_pair(char open, char close) {
    if (open == '(' && close == ')') return 1;
    if (open == '[' && close == ']') return 1;
    if (open == '{' && close == '}') return 1;
    return 0;
}

/*
 * 괄호 검사 함수
 * 반환값: 1 = 정상, 0 = 오류
 */
int check_brackets(const char *str) {
    StackType s;
    init_stack(&s);

    for (int i = 0; str[i] != '\0'; i++) {
        char ch = str[i];

        /* 열린 괄호: push */
        if (ch == '(' || ch == '[' || ch == '{') {
            push(&s, ch);
        }
        /* 닫힌 괄호: pop 후 짝 비교 */
        else if (ch == ')' || ch == ']' || ch == '}') {
            if (is_empty(&s)) {
                printf("오류: 닫힌 괄호가 남음 (열린 괄호 부족)\n");
                return 0;
            }
            char open = pop(&s);
            if (!is_matching_pair(open, ch)) {
                printf("오류: 괄호 짝이 안 맞음: %c 와 %c\n", open, ch);
                return 0;
            }
        }
    }

    /* 스택이 비어있어야 모든 괄호가 짝이 맞음 */
    if (!is_empty(&s)) {
        printf("오류: 열린 괄호가 남음 (%d개)\n", s.top + 1);
        return 0;
    }

    return 1;
}

int main(void) {
    char *test_cases[] = {
        "(A + B) * (C - D)",         /* 정상 */
        "((({{{[[]]}}}))",           /* 정상 */
        "({)}",                      /* 정상 */
        "(]",                        /* 오류: 짝 불일치 */
        "((())",                     /* 오류: 열린 괄호 남음 */
        "A + B)",                    /* 오류: 닫힌 괄호 남음 */
    };
    int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);

    printf("===== 괄호 짝 검사 =====\n\n");

    for (int i = 0; i < num_cases; i++) {
        printf("검사: \"%s\"\n", test_cases[i]);
        if (check_brackets(test_cases[i])) {
            printf("결과: 정상\n\n");
        } else {
            printf("결과: 오류\n\n");
        }
    }

    /* 사용자 입력 테스트 (원하면 주석 해제) */
    /*
    char input[256];
    printf("검사할 수식을 입력하세요: ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = '\0';  // 개행 문자 제거

    if (check_brackets(input)) {
        printf("괄호가 정상입니다.\n");
    } else {
        printf("괄호에 오류가 있습니다.\n");
    }
    */

    return 0;
}
