#include <stdio.h>

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MAX_DEGREE 101

/* 다항식 구조체: 차수와 계수 배열 */
typedef struct {
    int degree;            /* 최고 차수 */
    float coef[MAX_DEGREE]; /* 계수 배열 (내림차순 저장) */
} polynomial;

/*
 * 다항식 덧셈: C = A + B
 * 두 다항식을 받아서 더한 결과를 반환
 */
polynomial poly_add1(polynomial A, polynomial B) {
    polynomial C;
    int Apos = 0, Bpos = 0, Cpos = 0;
    int degree_a = A.degree;
    int degree_b = B.degree;

    C.degree = MAX(A.degree, B.degree);

    /* 두 다항식의 모든 항을 처리 */
    while (Apos <= A.degree && Bpos <= B.degree) {
        if (degree_a > degree_b) {
            /* A의 차수가 더 높은 경우: A의 항만 복사 */
            C.coef[Cpos++] = A.coef[Apos++];
            degree_a--;
        } else if (degree_a == degree_b) {
            /* 차수가 같은 경우: 계수 더하기 */
            C.coef[Cpos++] = A.coef[Apos++] + B.coef[Bpos++];
            degree_a--;
            degree_b--;
        } else {
            /* B의 차수가 더 높은 경우: B의 항만 복사 */
            C.coef[Cpos++] = B.coef[Bpos++];
            degree_b--;
        }
    }
    return C;
}

/*
 * 다항식 출력 함수
 * 예: 3x^5 + 6x^4 + 10
 */
void print_poly(polynomial p) {
    for (int i = 0; i < p.degree; i++) {
        printf("%3.1fx^%d + ", p.coef[i], p.degree - i);
    }
    printf("%3.1f\n", p.coef[p.degree]);
}

int main(void) {
    printf("===== 3.3 다항식 덧셈 =====\n\n");

    /* a(x) = 3x^5 + 6x^4 + 10 */
    polynomial a = {5, {3, 6, 0, 0, 0, 10}};

    /* b(x) = 7x^4 + 5x^2 + 1 */
    polynomial b = {4, {7, 0, 5, 0, 1}};

    polynomial c;

    printf("A(x) = ");
    print_poly(a);
    printf("B(x) = ");
    print_poly(b);

    c = poly_add1(a, b);

    printf("C(x) = A(x) + B(x)\n");
    printf("C(x) = ");
    print_poly(c);

    return 0;
}
