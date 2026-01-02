/*
 * malloc.c: 동적 메모리 할당 기초 예제
 *
 * malloc()으로 실행 시간에 메모리를 할당하고,
 * free()으로 사용 후 메모리를 반납한다.
 */

#include <stdio.h>
#include <stdlib.h>

#define SIZE 10

int main(void) {
    int *p;

    /* 정수 10개를 저장할 메모리 동적 할당 */
    p = (int *)malloc(SIZE * sizeof(int));

    /* 메모리 할당 실패 확인 */
    if (p == NULL) {
        fprintf(stderr, "메모리 할당 실패\n");
        exit(1);
    }

    /* 배열에 값 저장 */
    for (int i = 0; i < SIZE; i++) {
        p[i] = i;
    }

    /* 배열 내용 출력 */
    printf("===== 동적 메모리 할당 =====\n\n");
    for (int i = 0; i < SIZE; i++) {
        printf("%d ", p[i]);
    }
    printf("\n");

    /* 메모리 반납 (필수!) */
    free(p);
    p = NULL;  /* 댕글링 포인터 방지 */

    return 0;
}
