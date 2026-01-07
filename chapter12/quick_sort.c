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
    printf("[");
    for (size_t i = 0; i < size; i++) {
        printf("%d", arr[i]);
        if (i < size - 1) printf(" ");
    }
    printf("]");
}

// 배열 일부 출력 (범위 지정)
static void print_array_range(const int arr[], size_t left, size_t right) {
    printf("[");
    for (size_t i = left; i <= right; i++) {
        printf("%d", arr[i]);
        if (i < right) printf(" ");
    }
    printf("]");
}

// 분할 함수 (Lomuto Partition Scheme)
// 피벗을 기준으로 작은 값은 왼쪽, 큰 값은 오른쪽으로 배치
// 반환: 피벗의 최종 위치
static size_t partition(int arr[], size_t low, size_t high) {
    int pivot = arr[high];  // 마지막 원소를 피벗으로 선택
    size_t i = low;         // 작은 값들의 마지막 위치

    printf("  피벗=%d: ", pivot);
    print_array_range(arr, low, high);
    printf(" → ");

    for (size_t j = low; j < high; j++) {
        if (arr[j] <= pivot) {
            // arr[i]와 arr[j] 교환
            int temp = arr[i];
            arr[i] = arr[j];
            arr[j] = temp;
            i++;
        }
    }

    // 피벗을 올바른 위치로 이동
    int temp = arr[i];
    arr[i] = arr[high];
    arr[high] = temp;

    // 분할 결과 출력
    size_t pivot_pos = i;
    if (pivot_pos > low) {
        print_array_range(arr, low, pivot_pos - 1);
    } else {
        printf("[]");
    }
    printf(" [%d] ", arr[pivot_pos]);
    if (pivot_pos < high) {
        print_array_range(arr, pivot_pos + 1, high);
    } else {
        printf("[]");
    }
    printf("\n");

    return pivot_pos;
}

// 퀵 정렬 (재귀)
static void quick_sort_recursive(int arr[], size_t low, size_t high, int depth) {
    (void)depth;  // 미사용 매개변수 (향상된 출력을 위해 예비됨)
    if (low >= high) {
        return;
    }

    // 분할 및 피벗 위치 획득
    size_t pi = partition(arr, low, high);

    // 왼쪽 부분 정렬
    if (pi > 0) {  // underflow 방지
        quick_sort_recursive(arr, low, pi - 1, depth + 1);
    }

    // 오른쪽 부분 정렬
    quick_sort_recursive(arr, pi + 1, high, depth + 1);
}

// 퀵 정렬 진입점
void quick_sort(int arr[], size_t size) {
    if (size > 0) {
        quick_sort_recursive(arr, 0, size - 1, 0);
    }
}

int main(void) {
    int arr[MAX_SIZE];
    const size_t size = MAX_SIZE;

    srand((unsigned int)time(NULL));

    printf("=== 퀵 정렬 (Quick Sort) ===\n");
    printf("Divide and Conquer: 피벗 기준 분할 → 재귀 정렬\n");
    printf("평균 O(n log n), 최악 O(n²)\n\n");

    generate_random_array(arr, size, 100);

    printf("원본 배열: ");
    print_array_simple(arr, size);
    printf("\n\n");

    printf("=== 정렬 과정 ===\n");
    quick_sort(arr, size);

    printf("\n정렬된 배열: ");
    print_array_simple(arr, size);
    printf("\n");

    return 0;
}
