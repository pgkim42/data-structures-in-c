/*
 * postfix_eval.c: 후위 표기 수식 계산 (Postfix Expression Evaluator)
 *
 * 스택을 활용하여 후위 표기 수식을 계산
 * - 피연산자(숫자): 스택에 push
 * - 연산자(+, -, *, /): 스택에서 2개 pop 후 계산, 결과 push
 * - 수식 끝: 스택에 남은 값이 결과
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_STACK_SIZE 100

typedef int element;

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
        exit(1);
    }
    s->data[++s->top] = item;
}

/* 삭제 */
element pop(StackType *s) {
    if (is_empty(s)) {
        fprintf(stderr, "수식 오류: 피연산자 부족\n");
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
 * 연산 수행
 */
int compute(int op1, int op2, char op) {
    switch (op) {
        case '+': return op1 + op2;
        case '-': return op1 - op2;
        case '*': return op1 * op2;
        case '/': return op1 / op2;
        default:
            fprintf(stderr, "알 수 없는 연산자: %c\n", op);
            exit(1);
    }
}

/*
 * 문자열을 정수로 변환
 */
int string_to_int(const char *str) {
    int result = 0;
    while (*str) {
        result = result * 10 + (*str - '0');
        str++;
    }
    return result;
}

/*
 * 후위 표기 수식 계산
 */
int eval_postfix(const char *expr) {
    StackType s;
    init_stack(&s);

    char token[20];  /* 숫자 토큰 저장 버퍼 */
    int token_idx = 0;

    for (int i = 0; expr[i] != '\0'; i++) {
        char ch = expr[i];

        if (ch == ' ') {
            /* 공백: 토큰 처리 */
            if (token_idx > 0) {
                token[token_idx] = '\0';
                push(&s, string_to_int(token));
                token_idx = 0;
            }
        }
        else if (isdigit(ch)) {
            /* 숫자: 토큰 버퍼에 저장 */
            token[token_idx++] = ch;
        }
        else if (ch == '+' || ch == '-' || ch == '*' || ch == '/') {
            /* 연산자: 피연산자 2개 꺼내서 계산 */
            int op2 = pop(&s);  /* 주의: 먼저 꺼나는 게 두 번째 피연산자 */
            int op1 = pop(&s);
            int result = compute(op1, op2, ch);
            push(&s, result);
        }
    }

    /* 마지막 토큰 처리 (수식이 공백으로 안 끝날 경우) */
    if (token_idx > 0) {
        token[token_idx] = '\0';
        push(&s, string_to_int(token));
    }

    return peek(&s);
}

int main(void) {
    /* 테스트 케이스: 수식, (중위 표기) */
    char *test_cases[] = {
        "3 4 +",                 /* 3 + 4 = 7 */
        "3 4 + 5 *",             /* (3 + 4) × 5 = 35 */
        "7 2 3 * -",             /* 7 - (2 × 3) = 1 */
        "5 1 2 + 4 * + 3 -",     /* 5 + ((1 + 2) × 4) - 3 = 14 */
        "2 3 * 4 5 * +",         /* (2 × 3) + (4 × 5) = 26 */
    };
    int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);

    printf("===== 후위 표기 수식 계산 =====\n\n");

    for (int i = 0; i < num_cases; i++) {
        int result = eval_postfix(test_cases[i]);
        printf("%s\n", test_cases[i]);
        printf("결과: %d\n\n", result);
    }

    return 0;
}
