#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_DEGREE 100

/* ============================================================
 * 노드 및 다항식 구조체 정의
 * ============================================================ */
typedef struct PolyNode {
    int coef;               /* 계수 (coefficient) */
    int expon;              /* 차수 (exponent) */
    struct PolyNode *link;  /* 다음 항 포인터 */
} PolyNode;

typedef struct {
    PolyNode *head;         /* 첫 번째 항 */
    int length;             /* 항의 개수 */
} Polynomial;

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

/* 다항식 초기화
 * Time Complexity: O(1)
 */
void init(Polynomial *poly) {
    poly->head = NULL;
    poly->length = 0;
}

/* 다항식이 비었는지 확인
 * Time Complexity: O(1)
 */
int is_empty(Polynomial *poly) {
    return poly->head == NULL;
}

/* 항의 개수 반환
 * Time Complexity: O(1)
 */
int get_length(Polynomial *poly) {
    return poly->length;
}

/* 최고차항 차수 반환
 * Time Complexity: O(1)
 * - 내림차순 정렬 유지 시 첫 항의 차수가 최고차수
 */
int get_degree(Polynomial *poly) {
    if (is_empty(poly)) {
        return -1;  /* 빈 다항식 */
    }
    return poly->head->expon;
}

/* ============================================================
 * 항 추가/삭제/탐색
 * ============================================================ */

/* 항 추가 (차수 내림차순 유지)
 * Time Complexity: O(n) where n = 항의 개수
 * - 같은 차수가 있으면 계수 더함
 * - 계수가 0이 되면 해당 항 제거
 */
void add_term(Polynomial *poly, int coef, int expon) {
    PolyNode *new_node;
    PolyNode *prev, *current;

    /* 계수가 0이면 추가하지 않음 */
    if (coef == 0) {
        return;
    }

    prev = NULL;
    current = poly->head;

    /* 삽입 위치 찾기 (차수 내림차순) */
    while (current != NULL && current->expon > expon) {
        prev = current;
        current = current->link;
    }

    /* 같은 차수가 있으면 계수 더함 */
    if (current != NULL && current->expon == expon) {
        current->coef += coef;
        /* 계수가 0이 되면 노드 제거 */
        if (current->coef == 0) {
            if (prev == NULL) {
                poly->head = current->link;
            } else {
                prev->link = current->link;
            }
            free(current);
            poly->length--;
        }
        return;
    }

    /* 새 노드 생성 및 삽입 */
    new_node = (PolyNode *)malloc(sizeof(PolyNode));
    if (new_node == NULL) {
        error("Memory allocation failed");
    }
    new_node->coef = coef;
    new_node->expon = expon;

    if (prev == NULL) {
        /* 맨 앞에 삽입 */
        new_node->link = poly->head;
        poly->head = new_node;
    } else {
        /* 중간에 삽입 */
        new_node->link = current;
        prev->link = new_node;
    }
    poly->length++;
}

/* 특정 차수의 항 제거
 * Time Complexity: O(n)
 */
void remove_term(Polynomial *poly, int expon) {
    PolyNode *prev, *current;

    prev = NULL;
    current = poly->head;

    while (current != NULL && current->expon != expon) {
        prev = current;
        current = current->link;
    }

    if (current == NULL) {
        return;  /* 찾지 못함 */
    }

    if (prev == NULL) {
        poly->head = current->link;
    } else {
        prev->link = current->link;
    }

    free(current);
    poly->length--;
}

/* 특정 차수의 항 찾기 (계수 반환, 없으면 0)
 * Time Complexity: O(n)
 */
int get_term(Polynomial *poly, int expon) {
    PolyNode *current = poly->head;

    while (current != NULL && current->expon >= expon) {
        if (current->expon == expon) {
            return current->coef;
        }
        current = current->link;
    }
    return 0;  /* 해당 차수의 항 없음 */
}

/* ============================================================
 * 다항식 연산
 * ============================================================ */

/* 두 다항식 덧셈: result = p1 + p2
 * Time Complexity: O(n + m) where n, m = 각 다항식의 항 개수
 * - 병합 정렬의 merge 단계와 유사
 */
void add_poly(Polynomial *p1, Polynomial *p2, Polynomial *result) {
    PolyNode *a = p1->head;
    PolyNode *b = p2->head;

    init(result);

    while (a != NULL && b != NULL) {
        if (a->expon > b->expon) {
            add_term(result, a->coef, a->expon);
            a = a->link;
        } else if (a->expon < b->expon) {
            add_term(result, b->coef, b->expon);
            b = b->link;
        } else {
            /* 같은 차수: 계수 더함 */
            int sum = a->coef + b->coef;
            if (sum != 0) {
                add_term(result, sum, a->expon);
            }
            a = a->link;
            b = b->link;
        }
    }

    /* 남은 항들 추가 */
    while (a != NULL) {
        add_term(result, a->coef, a->expon);
        a = a->link;
    }
    while (b != NULL) {
        add_term(result, b->coef, b->expon);
        b = b->link;
    }
}

/* 두 다항식 곱셈: result = p1 * p2
 * Time Complexity: O(n * m) where n, m = 각 다항식의 항 개수
 * - 모든 항의 조합에 대해 곱셈 수행
 */
void mult_poly(Polynomial *p1, Polynomial *p2, Polynomial *result) {
    PolyNode *a, *b;

    init(result);

    for (a = p1->head; a != NULL; a = a->link) {
        for (b = p2->head; b != NULL; b = b->link) {
            /* 계수 곱하기, 차수 더하기 */
            add_term(result, a->coef * b->coef, a->expon + b->expon);
        }
    }
}

/* 다항식 값 계산: f(x)
 * Time Complexity: O(n) where n = 항의 개수
 * - Horner's Method 사용 가능하나 구현 단순화를 위해 직접 계산
 */
int eval_poly(Polynomial *poly, int x) {
    PolyNode *current = poly->head;
    int result = 0;

    while (current != NULL) {
        result += current->coef * (int)pow(x, current->expon);
        current = current->link;
    }
    return result;
}

/* 0 계수 항 제거 (다항식 정리)
 * Time Complexity: O(n)
 */
void simplify(Polynomial *poly) {
    PolyNode *current = poly->head;
    PolyNode *prev = NULL;
    PolyNode *temp;

    while (current != NULL) {
        if (current->coef == 0) {
            temp = current;
            if (prev == NULL) {
                poly->head = current->link;
            } else {
                prev->link = current->link;
            }
            current = current->link;
            free(temp);
            poly->length--;
        } else {
            prev = current;
            current = current->link;
        }
    }
}

/* ============================================================
 * 입출력 연산
 * ============================================================ */

/* 다항식 출력
 * Time Complexity: O(n)
 */
void print_poly(Polynomial *poly) {
    PolyNode *current = poly->head;

    if (is_empty(poly)) {
        printf("0");
        return;
    }

    while (current != NULL) {
        if (current->coef > 0 && current != poly->head) {
            printf(" + ");
        } else if (current->coef < 0) {
            printf(" - ");
        }

        if (current->coef < 0) {
            printf("%d", -current->coef);
        } else {
            printf("%d", current->coef);
        }

        if (current->expon > 1) {
            printf("x^%d", current->expon);
        } else if (current->expon == 1) {
            printf("x");
        }
        /* expon == 0 이면 상수항만 출력 */

        current = current->link;
    }
    printf(" (len=%d)\n", poly->length);
}

/* 다항식 모든 항 삭제 (메모리 해제)
 * Time Complexity: O(n)
 */
void clear(Polynomial *poly) {
    PolyNode *current = poly->head;
    PolyNode *next;

    while (current != NULL) {
        next = current->link;
        free(current);
        current = next;
    }

    poly->head = NULL;
    poly->length = 0;
}

/* ============================================================
 * 시간 복잡도 요약
 * ============================================================
 * Operation        | Time Complexity | Notes
 * -----------------|-----------------|------------------------
 * init()           | O(1)            |
 * is_empty()       | O(1)            |
 * get_length()     | O(1)            |
 * get_degree()     | O(1)            | 첫 항의 차수
 * add_term()       | O(n)            | 위치 찾기 + 삽입
 * remove_term()    | O(n)            | 선형 탐색
 * get_term()       | O(n)            | 선형 탐색
 * add_poly()       | O(n+m)          | merge 방식
 * mult_poly()      | O(n*m)          | 이중 루프
 * eval_poly()      | O(n)            | 각 항 계산
 * simplify()       | O(n)            | 순회하며 제거
 * print_poly()     | O(n)            |
 * clear()          | O(n)            |
 * ============================================================ */

/* ============================================================
 * 메인 함수
 * ============================================================ */
int main(void) {
    Polynomial p1, p2, result;

    printf("=== 다항식 연결 리스트 (Polynomial using Linked List) ===\n\n");

    // 다항식 p1: 3x^6 + 7x^3 - 2x^2 + 5
    printf("=== 다항식 생성 ===\n");
    init(&p1);
    add_term(&p1, 3, 6);
    add_term(&p1, 7, 3);
    add_term(&p1, -2, 2);
    add_term(&p1, 5, 0);
    printf("p1 = "); print_poly(&p1);
    printf("p1 degree: %d, length: %d\n\n", get_degree(&p1), get_length(&p1));

    // 다항식 p2: 2x^6 - 5x^3 + 4x^2 + 1
    init(&p2);
    add_term(&p2, 2, 6);
    add_term(&p2, -5, 3);
    add_term(&p2, 4, 2);
    add_term(&p2, 1, 0);
    printf("p2 = "); print_poly(&p2);
    printf("p2 degree: %d, length: %d\n\n", get_degree(&p2), get_length(&p2));

    // 덧셈 테스트
    printf("=== 다항식 덧셈 ===\n");
    add_poly(&p1, &p2, &result);
    printf("p1 + p2 = "); print_poly(&result);
    printf("expected: 5x^6 + 2x^3 + 2x^2 + 6\n\n");

    // 곱셈 테스트 (간단한 예)
    printf("=== 다항식 곱셈 ===\n");
    Polynomial p3, p4;
    init(&p3);
    init(&p4);
    // p3 = x + 1, p4 = x - 1 => p3 * p4 = x^2 - 1
    add_term(&p3, 1, 1);
    add_term(&p3, 1, 0);
    add_term(&p4, 1, 1);
    add_term(&p4, -1, 0);
    printf("p3 = "); print_poly(&p3);
    printf("p4 = "); print_poly(&p4);
    mult_poly(&p3, &p4, &result);
    printf("p3 * p4 = "); print_poly(&result);
    printf("expected: x^2 - 1\n\n");

    // eval_poly 테스트
    printf("=== 다항식 값 계산 ===\n");
    printf("p3(2) = %d (expected: 3)\n", eval_poly(&p3, 2));
    printf("p3(5) = %d (expected: 6)\n", eval_poly(&p3, 5));

    // remove_term 테스트
    printf("\n=== 항 제거 ===\n");
    printf("Before: "); print_poly(&p1);
    remove_term(&p1, 3);  /* x^3 항 제거 */
    printf("After remove_term(p1, 3): "); print_poly(&p1);

    // get_term 테스트
    printf("\n=== 항 조회 ===\n");
    printf("get_term(p1, 6) = %d (expected: 3)\n", get_term(&p1, 6));
    printf("get_term(p1, 3) = %d (expected: 0)\n", get_term(&p1, 3));

    // simplify 테스트
    printf("\n=== 다항식 정리 (simplify) ===\n");
    Polynomial p5;
    init(&p5);
    add_term(&p5, 3, 2);
    add_term(&p5, 0, 1);  /* 0계수 항 */
    add_term(&p5, 0, 0);  /* 0계수 항 */
    printf("Before simplify: "); print_poly(&p5);
    simplify(&p5);
    printf("After simplify:  "); print_poly(&p5);

    // 메모리 정리
    clear(&p1);
    clear(&p2);
    clear(&p3);
    clear(&p4);
    clear(&p5);
    clear(&result);

    return 0;
}
