#include <stdio.h>

/*
 * 두 정수를 교환하는 함수
 * 포인터를 사용하여 호출자의 변수 값을 직접 변경
 */
void swap(int *px, int *py) {
    const int tmp = *px;  /* px가 가리키는 값 저장 */
    *px = *py;            /* px 위치에 py의 값 저장 */
    *py = tmp;            /* py 위치에 tmp 값 저장 */
}

int main(void) {
    int a = 1, b = 2;

    printf("===== 포인터를 이용한 swap =====\n\n");
    printf("swap 호출 전: a = %d, b = %d\n", a, b);
    swap(&a, &b);  /* 변수의 주소를 전달 */
    printf("swap 호출 후: a = %d, b = %d\n", a, b);

    return 0;
}
