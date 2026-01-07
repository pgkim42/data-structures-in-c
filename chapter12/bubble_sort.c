#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#define MAX_SIZE 10

// 랜덤 배열 생성
static void generate_random_array(int arr[], size_t size, int max_value) {
    for (size_t i = 0; i < size; i++) {
        arr[i] = rand() % max_value;
    }
}

// 배열 출력 (확정된 영역 구분)
static void print_array_with_boundary(const int arr[], size_t size, size_t boundary) {
    for (size_t i = 0; i < size; i++) {
        if (i == boundary && boundary > 0) {
            printf("| ");  // 확정된 영역 구분
        }
        printf("%d ", arr[i]);
    }
    if (boundary == 0) {
        printf("|");  // 전체 미확정
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

// 버블 정렬
// 시간 복잡도: O(n²) (최적화 시 최선 O(n))
// 공간 복잡도: O(1)
// 안정 정렬: Yes
void bubble_sort(int arr[], size_t size) {
    bool swapped;
    size_t pass_count = 0;

    for (size_t i = 0; i < size - 1; i++) {
        swapped = false;

        // 인접한 원소 비교 및 교환
        for (size_t j = 0; j < size - 1 - i; j++) {
            if (arr[j] > arr[j + 1]) {
                // 교환
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
                swapped = true;
            }
        }

        pass_count++;

        // 과정 출력
        if (swapped) {
            printf("Pass %zu: %d이(가) 위치 %zu에 확정됨\n",
                   pass_count, arr[size - 1 - i], size - 1 - i);
        } else {
            printf("Pass %zu: 교환 없음 → 이미 정렬됨 (조기 종료)\n", pass_count);
        }

        printf("       ");
        print_array_with_boundary(arr, size, size - 1 - i);
        printf("\n");

        // Early termination: 교환이 없으면 이미 정렬된 것
        if (!swapped) {
            break;
        }
    }
}

int main(void) {
    int arr[MAX_SIZE];
    const size_t size = MAX_SIZE;

    srand((unsigned int)time(NULL));

    printf("=== 버블 정렬 (Bubble Sort) ===\n\n");

    generate_random_array(arr, size, 100);

    printf("원본 배열:\n   ");
    print_array_simple(arr, size);
    printf("\n");

    printf("--- 정렬 과정 ---\n");
    bubble_sort(arr, size);

    printf("정렬된 배열:\n   ");
    print_array_simple(arr, size);

    return 0;
}
