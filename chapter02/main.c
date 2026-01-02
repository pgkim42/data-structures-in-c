#include <stdio.h>

/*
 * ============================================
 * 재귀(Recursion) vs 반복(Iteration)
 * ============================================
 *
 * [재귀] - 함수가 자기 자신을 호출
 *   - 장점: 코드가 깔끔함, 수학적 정의와 일치
 *   - 단점: 스택 메모리 사용, 호출 오버헤드
 *
 * [반복] - for, while 루프 사용
 *   - 장점: 메모리 효율적, 실행 속도 빠름
 *   - 단점: 코드가 더 복잡해질 수 있음
 */

/* ==================== 팩토리얼 ==================== */

/* 재귀적 팩토리얼 계산 */
int factorial_recursive(const int n) {
    if (n <= 1)
        return 1;
    return n * factorial_recursive(n - 1);
}

/* 반복적 팩토리얼 계산 */
int factorial_iterative(const int n) {
    int result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

/* ==================== 거듭제곱 ==================== */

/* 반복적 거듭제곱 계산 (O(n)) */
double slow_power(const double x, const int n) {
    double result = 1.0;
    for (int i = 0; i < n; i++) {
        result *= x;
    }
    return result;
}

/* 재귀적 거듭제곱 계산 (O(log n)) */
double power(const double x, const int n) {
    if (n == 0)
        return 1;
    if (n % 2 == 0)
        return power(x * x, n / 2);
    return x * power(x * x, (n - 1) / 2);
}

/* ==================== 피보나치 ==================== */

/* 재귀적 피보나치 수열 계산 (O(2^n)) */
int fib(const int n) {
    if (n <= 1)
        return n;
    return fib(n - 1) + fib(n - 2);
}

/* 반복적 피보나치 수열 계산 (O(n)) */
int fib_iter(const int n) {
    if (n <= 1)
        return n;

    int pp = 0;
    int p = 1;
    int result = 0;

    for (int i = 2; i <= n; i++) {
        result = p + pp;
        pp = p;
        p = result;
    }

    return result;
}

/* ==================== 하노이의 탑 ==================== */

/* 하노이의 탑: n개의 원판을 from에서 tmp를 거쳐 to로 옮김 (O(2^n)) */
void hanoi_tower(const int n, const char from, const char tmp, const char to) {
    if (n == 1) {
        printf("원판 1을 %c에서 %c으로 옮긴다.\n", from, to);
        return;
    }
    hanoi_tower(n - 1, from, to, tmp);
    printf("원판 %d을 %c에서 %c으로 옮긴다.\n", n, from, to);
    hanoi_tower(n - 1, tmp, from, to);
}

/* ==================== 메인 ==================== */

int main(void) {
    const int n = 5;

    /* 팩토리얼 테스트 */
    printf("===== 팩토리얼 =====\n");
    printf("재귀: %d! = %d\n", n, factorial_recursive(n));
    printf("반복: %d! = %d\n\n", n, factorial_iterative(n));

    /* 거듭제곱 테스트 */
    printf("===== 거듭제곱 =====\n");
    const double x = 2.0;
    const int exp = 10;
    printf("반복: %.0f^%d = %.0f\n", x, exp, slow_power(x, exp));
    printf("재귀: %.0f^%d = %.0f\n\n", x, exp, power(x, exp));

    /* 피보나치 테스트 */
    printf("===== 피보나치 =====\n");
    const int fib_n = 10;
    printf("반복: fib(%d) = %d\n", fib_n, fib_iter(fib_n));
    printf("재귀: fib(%d) = %d\n\n", fib_n, fib(fib_n));

    /* 하노이의 탑 테스트 */
    printf("===== 하노이의 탑 =====\n");
    hanoi_tower(4, 'A', 'B', 'C');

    return 0;
}
