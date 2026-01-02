#include <stdio.h>
#include <stdlib.h>

#define MAX_TERMS 100

/* 희소 행렬의 한 원소: (행, 열, 값) */
typedef struct {
    int row;
    int col;
    int value;
} element;

/* 희소 행렬: 0이 아닌 원소들만 저장 */
typedef struct {
    element data[MAX_TERMS];  /* 0이 아닌 원소 배열 */
    int rows;                  /* 행의 개수 */
    int cols;                  /* 열의 개수 */
    int terms;                 /* 0이 아닌 원소 개수 */
} SparseMatrix;

/*
 * 희소 행렬 전치 함수 (O(terms × cols))
 * 행렬 A를 전치하여 행렬 B를 반환
 * 원소들을 열 순서로 재배열
 */
SparseMatrix matrix_transpose2(SparseMatrix a) {
    SparseMatrix b;
    int bindex;  /* B에서 현재 저장 위치 */

    /* 전치: 행 ↔ 열 교환 */
    b.rows = a.cols;
    b.cols = a.rows;
    b.terms = a.terms;

    if (a.terms > 0) {
        bindex = 0;

        /* A의 열을 기준으로 B에 채움 */
        for (int c = 0; c < a.cols; c++) {
            for (int i = 0; i < a.terms; i++) {
                /* 현재 열에 속하는 원소 찾기 */
                if (a.data[i].col == c) {
                    b.data[bindex].row = a.data[i].col;
                    b.data[bindex].col = a.data[i].row;
                    b.data[bindex].value = a.data[i].value;
                    bindex++;
                }
            }
        }
    }

    return b;
}

/*
 * 희소 행렬 출력 함수
 */
void matrix_print(SparseMatrix a) {
    printf("행: %d, 열: %d, 항 개수: %d\n", a.rows, a.cols, a.terms);
    printf("(행, 열, 값)\n");
    for (int i = 0; i < a.terms; i++) {
        printf("(%d, %d, %d)\n", a.data[i].row, a.data[i].col, a.data[i].value);
    }
}

int main(void) {
    printf("===== 희소 행렬 전치 =====\n\n");

    /* 희소 행렬 M (6×6, 7개의 0이 아닌 원소) */
    SparseMatrix m = {
        /* (row, col, value) */
        {{0, 3, 7}, {1, 0, 9}, {1, 5, 8},
         {3, 1, 5}, {4, 5, 1}, {5, 2, 2}},
        6,  /* 6행 */
        6,  /* 6열 */
        6   /* 6개의 원소 */
    };

    SparseMatrix result;

    printf("원본 행렬 M:\n");
    matrix_print(m);
    printf("\n");

    result = matrix_transpose2(m);

    printf("전치 행렬 M^T:\n");
    matrix_print(result);

    return 0;
}
