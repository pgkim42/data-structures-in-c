#include <stdio.h>
#include <stdlib.h>

#define MAX_TERMS 101

/* 항(term): 계수와 지수 */
typedef struct {
    float coef;   /* 계수 */
    int expon;    /* 지수 */
} polynomial;

/*
 * 전역 배열로 다항식 저장
 * 여러 다항식이 하나의 배열을 공유하며
 * 시작/끝 인덱스로 구분한다
 */
polynomial terms[MAX_TERMS] = {
    /* A(x) = 8x^3 + 7x^1 + 1 */
    {8, 3}, {7, 1}, {1, 0},
    /* B(x) = 10x^3 + 3x^2 + 1 */
    {10, 3}, {3, 2}, {1, 0}
};

/* 다음으로 사용 가능한 배열 인덱스 */
int avail = 6;

/*
 * 두 정수를 비교하여 결과 문자 반환
 * 반환값: '>', '=', '<'
 */
char compare(int a, int b) {
    if (a > b) return '>';
    else if (a == b) return '=';
    else return '<';
}

/*
 * 새로운 항을 전역 배열에 추가
 */
void attach(float coef, int expon) {
    if (avail >= MAX_TERMS) {
        fprintf(stderr, "항의 개수가 너무 많음\n");
        exit(1);
    }
    terms[avail].coef = coef;
    terms[avail].expon = expon;
    avail++;
}

/*
 * 다항식 덧셈: C = A + B
 *
 * 매개변수:
 *   As, Ae: 다항식 A의 시작/끝 인덱스
 *   Bs, Be: 다항식 B의 시작/끝 인덱스
 *   Cs, Ce: 결과 다항식 C의 시작/끝 인덱스 (포인터)
 */
void poly_add2(int As, int Ae, int Bs, int Be, int *Cs, int *Ce) {
    float tempcoef;

    *Cs = avail;  /* C의 시작 위치 저장 */

    /* A와 B의 항을 차순으로 비교하며 덧셈 수행 */
    while (As <= Ae && Bs <= Be) {
        switch (compare(terms[As].expon, terms[Bs].expon)) {
            case '>':  /* A의 차수가 더 높음 */
                attach(terms[As].coef, terms[As].expon);
                As++;
                break;
            case '=':  /* 차수가 같음: 계수 더함 */
                tempcoef = terms[As].coef + terms[Bs].coef;
                if (tempcoef != 0)  /* 계수가 0이면 추가하지 않음 */
                    attach(tempcoef, terms[As].expon);
                As++;
                Bs++;
                break;
            case '<':  /* B의 차수가 더 높음 */
                attach(terms[Bs].coef, terms[Bs].expon);
                Bs++;
                break;
        }
    }

    /* A의 남은 항들 복사 */
    for (; As <= Ae; As++)
        attach(terms[As].coef, terms[As].expon);

    /* B의 남은 항들 복사 */
    for (; Bs <= Be; Bs++)
        attach(terms[Bs].coef, terms[Bs].expon);

    *Ce = avail - 1;  /* C의 끝 위치 저장 */
}

/*
 * 다항식 출력 함수
 * s: 시작 인덱스, e: 끝 인덱스
 */
void print_poly(int s, int e) {
    for (int i = s; i < e; i++) {
        printf("%3.1fx^%d + ", terms[i].coef, terms[i].expon);
    }
    printf("%3.1fx^%d\n", terms[e].coef, terms[e].expon);
}

int main(void) {
    printf("===== 3.3 희소 다항식 덧셈 =====\n\n");

    /* A: 인덱스 0~2, B: 인덱스 3~5 */
    int As = 0, Ae = 2;
    int Bs = 3, Be = 5;
    int Cs, Ce;

    poly_add2(As, Ae, Bs, Be, &Cs, &Ce);

    printf("A(x) = ");
    print_poly(As, Ae);
    printf("B(x) = ");
    print_poly(Bs, Be);
    printf("C(x) = A(x) + B(x)\n");
    printf("C(x) = ");
    print_poly(Cs, Ce);

    return 0;
}
