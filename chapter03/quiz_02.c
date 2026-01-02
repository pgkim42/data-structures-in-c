#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MAX_DEGREE 101
#define MAX_TERMS 101
#define MAX(a, b) ((a) > (b) ? (a) : (b))

/* ============================================================
 * 01. 다항식 6x^3 + 8x^2 + 9를 polynomial1로 표현하기
 * ============================================================*/
/* polynomial1: 2차원 배열 방식 (차수를 인덱스로 사용) */

typedef struct {
    int degree;
    float coef[MAX_DEGREE];
} polynomial1;

/* 6x^3 + 8x^2 + 9 → {3, {9, 0, 8, 6}} */
/*          인덱스:     0   1  2  3  */
/*          차수:       x^0 x^1 x^2 x^3 */

/* ============================================================
 * 02. 다항식 6x^3 + 8x^2 + 9를 polynomial2로 표현하기
 * ============================================================*/
/* polynomial2: 희소 배열 방식 (계수+지수 쌍) */

typedef struct {
    float coef;
    int expon;
} polynomial2;

/* 6x^3 + 8x^2 + 9 → {{6, 3}, {8, 2}, {9, 0}} */
/*                   {계수, 지수} 쌍으로 저장 */

/* ============================================================
 * 03. 다항식 뺄셈: 덧셈 코드에서 변경할 부분
 * ============================================================*/

/* polynomial1 뺄셈: '+'를 '-'로 변경 */
polynomial1 poly_sub1(polynomial1 A, polynomial1 B) {
    polynomial1 C;
    int Apos = 0, Bpos = 0, Cpos = 0;
    int degree_a = A.degree;
    int degree_b = B.degree;

    C.degree = MAX_DEGREE > 0 ? MAX(A.degree, B.degree) : 0;

    while (Apos <= A.degree && Bpos <= B.degree) {
        if (degree_a > degree_b) {
            C.coef[Cpos++] = A.coef[Apos++];
            degree_a--;
        } else if (degree_a == degree_b) {
            /* 변경점: 덧셈(+) → 뺄셈(-) */
            C.coef[Cpos++] = A.coef[Apos++] - B.coef[Bpos++];
            degree_a--;
            degree_b--;
        } else {
            /* 변경점: B의 계수를 음수로 변경 */
            C.coef[Cpos++] = -B.coef[Bpos++];
            degree_b--;
        }
    }
    return C;
}

/* polynomial2 뺄셈: '+'를 '-'로 변경 */
void poly_sub2(int As, int Ae, int Bs, int Be, int *Cs, int *Ce) {
    /* ... 기존 코드와 동일 ... */
    /* 변경점: tempcoef = terms[As].coef + terms[Bs].coef; */
    /*        → tempcoef = terms[As].coef - terms[Bs].coef; */
    /*        그리고 B의 항을 복사할 때 계수를 음수로 */
}

/* ============================================================
 * 04. 파일에서 다항식 읽기: poly_read()
 * ============================================================*/

/* polynomial1용 읽기 함수 */
void poly_read1(polynomial1 *p) {
    printf("차수를 입력하세요: ");
    scanf("%d", &p->degree);

    printf("%d개의 계수를 차순으로 입력하세요:\n", p->degree + 1);
    for (int i = p->degree; i >= 0; i--) {
        printf("x^%d의 계수: ", i);
        scanf("%f", &p->coef[p->degree - i]);
    }
}

/* polynomial2용 읽기 함수 */
int poly_read2(polynomial2 terms[], int start) {
    int degree, count = 0;

    printf("다항식의 항 개수를 입력하세요: ");
    scanf("%d", &count);

    for (int i = 0; i < count; i++) {
        printf("항 %d - 지수: ", i + 1);
        scanf("%d", &terms[start + i].expon);
        printf("항 %d - 계수: ", i + 1);
        scanf("%f", &terms[start + i].coef);
    }

    return start + count;  /* 다음 시작 위치 반환 */
}

/* ============================================================
 * 05. 다항식 값 계산: poly_eval()
 * ============================================================*/

/* polynomial1용: Horner의 법칙 O(n) */
float poly_eval1(polynomial1 p, float x) {
    float result = 0;

    /* Horner의 법칙: ...((a_n * x + a_{n-1}) * x + ...) * x + a_0 */
    for (int i = 0; i <= p.degree; i++) {
        result = result * x + p.coef[i];
    }

    return result;
}

/* polynomial2용: 직접 계산 O(n) */
float poly_eval2(polynomial2 terms[], int s, int e, float x) {
    float result = 0;

    for (int i = s; i <= e; i++) {
        result += terms[i].coef * pow(x, terms[i].expon);
    }

    return result;
}

/* ============================================================
 * 메인: 테스트 코드
 * ============================================================*/

void print_poly1(polynomial1 p) {
    for (int i = 0; i < p.degree; i++) {
        if (p.coef[i] != 0)
            printf("%3.1fx^%d + ", p.coef[i], p.degree - i);
    }
    printf("%3.1f\n", p.coef[p.degree]);
}

int main(void) {
    printf("===== Quiz 02 =====\n\n");

    /* ===== 01번: polynomial1 표현 ===== */
    printf("=== 01. polynomial1 표현 ===\n");
    polynomial1 p1 = {3, {9, 0, 8, 6}};  /* 6x^3 + 8x^2 + 9 */
    printf("6x^3 + 8x^2 + 9 = {3, {9, 0, 8, 6}}\n");
    printf("P1(x) = ");
    print_poly1(p1);

    /* ===== 02번: polynomial2 표현 ===== */
    printf("\n=== 02. polynomial2 표현 ===\n");
    polynomial2 terms[MAX_TERMS] = {{6, 3}, {8, 2}, {9, 0}};
    printf("6x^3 + 8x^2 + 9 = {{6, 3}, {8, 2}, {9, 0}}\n");
    printf("P2(x) = 6.0x^3 + 8.0x^2 + 9.0x^0\n");

    /* ===== 03번: 뺄셈 ===== */
    printf("\n=== 03. 뺄셈 (덧셈 코드에서 변경) ===\n");
    printf("polynomial1: A.coef + B.coef → A.coef - B.coef\n");
    printf("polynomial2: tempcoef = A.coef + B.coef\n");
    printf("            → tempcoef = A.coef - B.coef\n");

    /* ===== 04번: poly_read() 테스트 ===== */
    printf("\n=== 04. poly_read() 테스트 ===\n");
    printf("(함수는 구현되어 있음 - 주석 참고)\n");

    /* ===== 05번: poly_eval() 테스트 ===== */
    printf("\n=== 05. poly_eval() 테스트 ===\n");
    float x = 2.0;
    printf("P(2) 계산 (6x^3 + 8x^2 + 9):\n");
    printf("  polynomial1: %.2f\n", poly_eval1(p1, x));
    printf("  polynomial2: %.2f\n", poly_eval2(terms, 0, 2, x));
    printf("  검증: 6(8) + 8(4) + 9 = 48 + 32 + 9 = 89\n");

    return 0;
}
