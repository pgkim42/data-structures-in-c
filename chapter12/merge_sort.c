#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#define MAX_SIZE 8  // 병합 정렬은 재귀 깊이가 깊어서 크기를 줄임

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

// 병합 함수: 두 개의 정렬된 부분 배열을 하나로 병합
// arr[left..mid]와 arr[mid+1..right]가 이미 정렬되어 있음
static void merge(int arr[], size_t left, size_t mid, size_t right) {
    size_t n1 = mid - left + 1;  // 왼쪽 배열 크기
    size_t n2 = right - mid;      // 오른쪽 배열 크기

    // 임시 배열 할당
    int *L = (int *)malloc(n1 * sizeof(int));
    int *R = (int *)malloc(n2 * sizeof(int));

    if (L == NULL || R == NULL) {
        free(L);
        free(R);
        return;
    }

    // 데이터 복사
    for (size_t i = 0; i < n1; i++) {
        L[i] = arr[left + i];
    }
    for (size_t j = 0; j < n2; j++) {
        R[j] = arr[mid + 1 + j];
    }

    // 병합 과정 출력
    printf("  병합: ");
    print_array_range(L, 0, n1 - 1);
    printf(" + ");
    print_array_range(R, 0, n2 - 1);
    printf(" → ");

    // 두 배열을 병합
    size_t i = 0, j = 0, k = left;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {  // 안정 정렬을 위해 <= 사용
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }

    // 남은 원소 복사
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }

    print_array_range(arr, left, right);
    printf("\n");

    free(L);
    free(R);
}

// 병합 정렬 (재귀)
static void merge_sort_recursive(int arr[], size_t left, size_t right, int depth) {
    if (left >= right) {
        // 분할의 끝: 원소 1개
        printf("%*s", depth * 2, "");
        print_array_range(arr, left, right);
        printf("\n");
        return;
    }

    size_t mid = left + (right - left) / 2;

    // 분할 과정 출력
    printf("%*s", depth * 2, "");
    print_array_range(arr, left, right);
    printf(" → 분할: ");
    print_array_range(arr, left, mid);
    printf(", ");
    print_array_range(arr, mid + 1, right);
    printf("\n");

    // 왼쪽 정렬
    merge_sort_recursive(arr, left, mid, depth + 1);

    // 오른쪽 정렬
    merge_sort_recursive(arr, mid + 1, right, depth + 1);

    // 병합
    merge(arr, left, mid, right);
}

// 병합 정렬 진입점
void merge_sort(int arr[], size_t size) {
    printf("=== 분할 과정 ===\n");
    merge_sort_recursive(arr, 0, size - 1, 0);
    printf("\n");
}

int main(void) {
    int arr[MAX_SIZE];
    const size_t size = MAX_SIZE;

    srand((unsigned int)time(NULL));

    printf("=== 병합 정렬 (Merge Sort) ===\n");
    printf("Divide and Conquer: 분할 → 정렬 → 병합\n\n");

    generate_random_array(arr, size, 100);

    printf("원본 배열: ");
    print_array_simple(arr, size);
    printf("\n\n");

    merge_sort(arr, size);

    printf("정렬된 배열: ");
    print_array_simple(arr, size);
    printf("\n");

    return 0;
}
