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

// 배열 출력 (정렬된 영역과 하이라이트 표시)
static void print_array_with_highlight(const int arr[], size_t size,
                                        size_t sorted_end, size_t highlight) {
    for (size_t i = 0; i < size; i++) {
        if (i == sorted_end && sorted_end < size) {
            printf("| ");  // 정렬된 영역과 미정렬 영역 구분
        }
        if (i == highlight) {
            printf("[%d] ", arr[i]);  // 현재 처리 중인 원소 강조
        } else {
            printf("%d ", arr[i]);
        }
    }
    if (sorted_end < size) {
        printf("|");  // 마지막 구분
    }
    printf("\n");
}

// 배열 간단 출력
static void print_array_simple(const int arr[], size_t size) {
    for (size_t i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

// 삽입 정렬
// 시간 복잡도: 최악 O(n²), 최선 O(n) (이미 정렬된 경우)
// 공간 복잡도: O(1)
// 안정 정렬: Yes
void insertion_sort(int arr[], size_t size) {
    for (size_t i = 1; i < size; i++) {
        int key = arr[i];
        size_t j = i;

        // key보다 큰 원소들을 오른쪽으로 이동
        while (j > 0 && arr[j - 1] > key) {
            arr[j] = arr[j - 1];
            j--;
        }
        arr[j] = key;

        // 과정 출력
        if (j == i) {
            // 이동 없이 제자리인 경우
            printf("Pass %zu: %d은(는) 이미 올바른 위치\n", i, key);
            printf("       ");
            print_array_with_highlight(arr, size, i + 1, i);
        } else {
            // 삽입 발생
            printf("Pass %zu: %d을(를) 인덱스 %zu에 삽입\n", i, key, j);
            printf("       ");
            print_array_with_highlight(arr, size, i + 1, j);
        }
        printf("\n");
    }
}

int main(void) {
    int arr[MAX_SIZE];
    const size_t size = MAX_SIZE;

    srand((unsigned int)time(NULL));

    printf("=== 삽입 정렬 (Insertion Sort) ===\n\n");

    generate_random_array(arr, size, 100);

    printf("원본 배열:\n   ");
    print_array_simple(arr, size);
    printf("\n");

    printf("--- 정렬 과정 ---\n");
    insertion_sort(arr, size);

    printf("정렬된 배열:\n   ");
    print_array_simple(arr, size);

    return 0;
}
