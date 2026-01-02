#include <stdio.h>

#define ROWS 3
#define COLS 3

/*
 * 행렬 전치 함수
 * A[r][c] → B[c][r] (행과 열을 서로 맞바꿈)
 */
void matrix_transpose(int A[ROWS][COLS], int B[ROWS][COLS]) {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            B[c][r] = A[r][c];
        }
    }
}

/*
 * 행렬 출력 함수
 */
void matrix_print(int A[ROWS][COLS]) {
    for (int r = 0; r < ROWS; r++) {
        for (int c = 0; c < COLS; c++) {
            printf("%2d ", A[r][c]);
        }
        printf("\n");
    }
}

int main(void) {
    printf("===== 행렬 전치 =====\n\n");

    /* 원본 행렬 */
    int array1[ROWS][COLS] = {
        {2, 3, 0},
        {8, 9, 1},
        {7, 0, 5}
    };

    /* 전치 행렬을 저장할 배열 */
    int array2[ROWS][COLS];

    matrix_transpose(array1, array2);

    printf("원본 행렬:\n");
    matrix_print(array1);
    printf("\n전치 행렬:\n");
    matrix_print(array2);

    return 0;
}
