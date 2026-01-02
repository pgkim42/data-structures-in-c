#include <stdio.h>

#define SIZE 6

/*
 * 배열 요소 입력 함수
 * list[]는 사실 포인터: int *list와 동일
 */
void get_integers(int list[]) {
    printf("6개 정수 입력: ");
    for (int i = 0; i < SIZE; i++) {
        scanf("%d", &list[i]);
    }
}

/*
 * 배열 요소 합계 계산 함수
 * 포인터 연산을 사용한 배열 접근
 * list[i]    == *(list + i)  ← 두 표현은 완전히 동일
 */
int cal_sum(const int list[]) {
    int sum = 0;
    for (int i = 0; i < SIZE; i++) {
        sum += *(list + i);  /* list[i]와 같음 */
    }
    return sum;
}

int main(void) {
    int list[SIZE];

    printf("===== 배열과 포인터 =====\n\n");

    get_integers(list);
    printf("합 = %d\n", cal_sum(list));

    return 0;
}
