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

// 배열 간단 출력
static void print_array_simple(const int arr[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// 쉘 정렬
// 시간 복잡도: Gap sequence에 따라 다름 (Knuth: O(n^1.5))
// 공간 복잡도: O(1)
// 안정 정렬: No
void shell_sort(int arr[], size_t size) {
    // Knuth's sequence: 1, 4, 13, 40, 121, ...
    size_t gap = 1;
    while (gap < size / 3) {
        gap = 3 * gap + 1;
    }

    size_t step = 0;

    // gap을 줄여가며 정렬
    while (gap > 0) {
        step++;

        // gap 간격으로 떨어진 원소들에 대해 삽입 정렬 수행
        for (size_t i = gap; i < size; i++) {
            int temp = arr[i];
            size_t j = i;

            // gap 간격으로 이동하며 적절한 위치 찾기
            while (j >= gap && arr[j - gap] > temp) {
                arr[j] = arr[j - gap];
                j -= gap;
            }
            arr[j] = temp;
        }

        // 과정 출력
        printf("Gap = %zu: ", gap);
        print_array_simple(arr, size);

        // gap 축소
        gap /= 3;
    }

    printf("\n총 %zu단계 거침\n", step);
}

int main(void) {
    int arr[MAX_SIZE];
    const size_t size = MAX_SIZE;

    srand((unsigned int)time(NULL));

    printf("=== 쉘 정렬 (Shell Sort) ===\n");
    printf("Gap Sequence: Knuth (1, 4, 13, 40, ...)\n\n");

    generate_random_array(arr, size, 100);

    printf("원본 배열:\n   ");
    print_array_simple(arr, size);
    printf("\n");

    printf("--- 정렬 과정 ---\n");
    shell_sort(arr, size);

    printf("\n정렬된 배열:\n   ");
    print_array_simple(arr, size);

    return 0;
}
