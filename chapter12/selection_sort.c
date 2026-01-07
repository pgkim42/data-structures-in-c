#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 10

// 랜덤 배열 생성
static void generate_random_array(int arr[], size_t size, int max_value) {
    for (size_t i = 0; i < size; i++) {
        arr[i] = rand() % max_value;
    }
}

// 배열 출력 (sorted_index까지 정렬 완료 표시)
static void print_array(const int arr[], size_t size, size_t sorted_index, size_t highlight) {
    for (size_t i = 0; i < size; i++) {
        if (i == sorted_index) {
            printf("| ");  // 정렬된 영역 구분
        }
        if (i == highlight) {
            printf("[%d] ", arr[i]);  // 최솟값/교환 위치 강조
        } else {
            printf("%d ", arr[i]);
        }
    }
    if (sorted_index < size) {
        printf("|");  // 마지막 구분
    }
    printf("\n");
}

// 배열 간단 출력 (결과용)
static void print_array_simple(const int arr[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// 선택 정렬
// 시간 복잡도: O(n²)
// 공간 복잡도: O(1)
void selection_sort(int arr[], size_t size) {
    for (size_t i = 0; i < size - 1; i++) {
        size_t min_idx = i;

        // 최솟값 탐색
        for (size_t j = i + 1; j < size; j++) {
            if (arr[j] < arr[min_idx]) {
                min_idx = j;
            }
        }

        // 교환 (최솟값이 현재 위치와 다를 때만)
        if (min_idx != i) {
            int temp = arr[i];
            arr[i] = arr[min_idx];
            arr[min_idx] = temp;

            printf("Pass %zu: %d을(를) 인덱스 %zu로 이동\n",
                   i + 1, arr[i], i);
        } else {
            printf("Pass %zu: %d은(는) 이미 올바른 위치\n",
                   i + 1, arr[i]);
        }

        // 현재 상태 출력 (정렬된 영역 표시)
        printf("       ");
        print_array(arr, size, i + 1, size);  // highlight 없음
        printf("\n");
    }
}

int main(void) {
    int arr[MAX_SIZE];
    const size_t size = MAX_SIZE;

    srand((unsigned int)time(NULL));

    printf("=== 선택 정렬 (Selection Sort) ===\n\n");

    generate_random_array(arr, size, 100);

    printf("원본 배열:\n   ");
    print_array_simple(arr, size);
    printf("\n");

    printf("--- 정렬 과정 ---\n");
    selection_sort(arr, size);

    printf("정렬된 배열:\n   ");
    print_array_simple(arr, size);

    return 0;
}
