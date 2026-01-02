/*
 * infix_to_postfix.c: 중위 표기 → 후위 표기 변환
 *
 * 스택을 활용하여 중위 표기 수식을 후위 표기로 변환
 * - 피연산자(숫자, 문자): 그대로 출력
 * - '(': 스택에 push
 * - ')': '(' 나올 때까지 pop 후 출력
 * - 연산자: 우선순위 비교하면서 처리
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
        exit(1);
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
 * 연산자 우선순위 반환 (클수록 높음)
 */
int precedence(char op) {
    switch (op) {
        case '*':
        case '/':
            return 2;
        case '+':
        case '-':
            return 1;
        default:
            return 0;
    }
}

/*
 * 연산자인지 확인
 */
int is_operator(char ch) {
    return ch == '+' || ch == '-' || ch == '*' || ch == '/';
}

/*
 * 중위 표기를 후위 표기로 변환
 *
 * 알고리즘 (Shunting-yard):
 * 1. 피연산자: 그대로 출력
 * 2. '(': 스택에 push
 * 3. ')': '(' 나올 때까지 pop 후 출력
 * 4. 연산자: 스택 top의 연산자가 우선순위가 높거나 같으면 pop 후 출력
 *          그 후 현재 연산자 push
 * 5. 수식 끝: 스택에 남은 것 모두 pop
 */
void infix_to_postfix(const char *infix, char *postfix) {
    StackType s;
    init_stack(&s);

    int j = 0;  /* postfix 인덱스 */

    for (int i = 0; infix[i] != '\0'; i++) {
        char ch = infix[i];

        if (isspace(ch)) {
            continue;  /* 공백 무시 */
        }
        /* 피연산자: 바로 출력 */
        else if (isalnum(ch)) {
            postfix[j++] = ch;
            postfix[j++] = ' ';  /* 피연산자 구분을 위해 공백 추가 */
        }
        /* 왼쪽 괄호: push */
        else if (ch == '(') {
            push(&s, ch);
        }
        /* 오른쪽 괄호: '(' 나올 때까지 pop */
        else if (ch == ')') {
            while (!is_empty(&s) && peek(&s) != '(') {
                postfix[j++] = pop(&s);
                postfix[j++] = ' ';
            }
            pop(&s);  /* '(' 제거 */
        }
        /* 연산자: 우선순위 비교 */
        else if (is_operator(ch)) {
            while (!is_empty(&s) && peek(&s) != '(' &&
                   precedence(peek(&s)) >= precedence(ch)) {
                postfix[j++] = pop(&s);
                postfix[j++] = ' ';
            }
            push(&s, ch);
        }
    }

    /* 스택에 남은 연산자 모두 pop */
    while (!is_empty(&s)) {
        postfix[j++] = pop(&s);
        postfix[j++] = ' ';
    }

    postfix[j - 1] = '\0';  /* 마지막 공백 제거하고 종료 */
}

int main(void) {
    char *test_cases[] = {
        "3 + 4",                    /* 3 4 + */
        "3 + 4 * 5",                /* 3 4 5 * + */
        "( 3 + 4 ) * 5",            /* 3 4 + 5 * */
        "5 + ( ( 1 + 2 ) * 4 ) - 3"  /* 5 1 2 + 4 * + 3 - */
    };
    int num_cases = sizeof(test_cases) / sizeof(test_cases[0]);

    printf("===== 중위 표기 → 후위 표기 변환 =====\n\n");

    for (int i = 0; i < num_cases; i++) {
        char postfix[256];
        infix_to_postfix(test_cases[i], postfix);
        printf("중위: %s\n", test_cases[i]);
        printf("후위: %s\n\n", postfix);
    }

    return 0;
}
