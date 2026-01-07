#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ARRAY_SIZE 16

// ============================================================
// 배열 출력 함수
// ============================================================

// 배열 간단 출력
static void print_array(const int arr[], size_t size) {
    printf("[");
    for (size_t i = 0; i < size; i++) {
        printf("%2d", arr[i]);
        if (i < size - 1) {
            printf(", ");
        }
    }
    printf("]");
}

// 탐색 과정 시각화 (범위와 중간점 표시)
static void print_search_state(const int arr[], size_t size,
                                size_t low, size_t high, size_t mid,
                                int target, const char* result) {
    printf("  ");
    for (size_t i = 0; i < size; i++) {
        // 탐색 범위 표시
        if (i == low && low == high) {
            printf(" ↓ ");
        } else if (i == low) {
            printf("[ ");
        } else if (i == high) {
            printf(" ]");
        }

        // 중간점 표시
        if (i == mid) {
            printf("▲%2d▲ ", arr[i]);  // mid 위치는 화살표로 강조
        } else if (i >= low && i <= high) {
            printf("%2d  ", arr[i]);    // 탐색 범위 내
        } else {
            printf(" -- ");             // 탐색 범위 밖
        }

        if (i == high && low != high) {
            printf("]");
        }
    }
    printf("\n");
    printf("     범위: [%zu, %zu], mid: %zu, 비교: %d vs %d → %s\n",
           low, high, mid, arr[mid], target, result);
}

// ============================================================
// 이진 탐색 (반복문 버전)
// ============================================================
// 시간 복잡도: O(log n)
// 공간 복잡도: O(1)
// ============================================================
int binary_search(const int arr[], size_t size, int target) {
    size_t low = 0;
    size_t high = size - 1;

    printf("\n  이진 탐색 시작: target = %d\n", target);
    printf("  초기 배열: ");
    print_array(arr, size);
    printf("\n\n");

    while (low <= high) {
        // 오버플로우 방지: (low + high) / 2 대신 low + (high - low) / 2 사용
        size_t mid = low + (high - low) / 2;

        if (arr[mid] == target) {
            print_search_state(arr, size, low, high, mid, target, "발견!");
            return (int)mid;
        }

        if (arr[mid] < target) {
            print_search_state(arr, size, low, high, mid, target, "오른쪽 탐색");
            low = mid + 1;  // 오른쪽 절반 탐색
        } else {
            print_search_state(arr, size, low, high, mid, target, "왼쪽 탐색");
            high = mid - 1;  // 왼쪽 절반 탐색
        }
    }

    printf("  → 탐색 실패: %d을(를) 찾을 수 없음\n", target);
    return -1;
}

// ============================================================
// 이진 탐색 (재귀 버전)
// ============================================================
// 시간 복잡도: O(log n)
// 공간 복잡도: O(log n) - 재귀 호출 스택
// ============================================================
static int binary_search_recursive_impl(const int arr[], int target,
                                         size_t low, size_t high,
                                         bool verbose) {
    if (low > high) {
        return -1;
    }

    size_t mid = low + (high - low) / 2;

    if (arr[mid] == target) {
        if (verbose) {
            printf("     재귀: 범위 [%zu, %zu], mid=%zu, arr[mid]=%d == target → 발견!\n",
                   low, high, mid, arr[mid]);
        }
        return (int)mid;
    }

    if (arr[mid] < target) {
        if (verbose) {
            printf("     재귀: 범위 [%zu, %zu], mid=%zu, arr[mid]=%d < target → 오른쪽\n",
                   low, high, mid, arr[mid]);
        }
        return binary_search_recursive_impl(arr, target, mid + 1, high, verbose);
    } else {
        if (verbose) {
            printf("     재귀: 범위 [%zu, %zu], mid=%zu, arr[mid]=%d > target → 왼쪽\n",
                   low, high, mid, arr[mid]);
        }
        return binary_search_recursive_impl(arr, target, low, mid - 1, verbose);
    }
}

// 재귀 버전 래퍼 함수
int binary_search_recursive(const int arr[], size_t size, int target) {
    printf("\n  재귀 버전 탐색 시작: target = %d\n", target);
    int result = binary_search_recursive_impl(arr, target, 0, size - 1, true);
    if (result == -1) {
        printf("  → 탐색 실패: %d을(를) 찾을 수 없음\n", target);
    }
    return result;
}

// ============================================================
// 정렬된 배열 생성
// ============================================================
static void create_sorted_array(int arr[], size_t size, int start, int step) {
    for (size_t i = 0; i < size; i++) {
        arr[i] = start + (int)(i * step);
    }
}

// ============================================================
// 메인 함수
// ============================================================
int main(void) {
    int arr[ARRAY_SIZE];

    printf("========================================\n");
    printf("    이진 탐색 (Binary Search)\n");
    printf("========================================\n");
    printf("조건: 배열이 이미 정렬되어 있어야 함\n");
    printf("시간 복잡도: O(log n)\n");
    printf("공간 복잡도: O(1) - 반복문 버전\n");
    printf("========================================\n\n");

    // 정렬된 배열 생성 (1, 3, 5, 7, ..., 31)
    create_sorted_array(arr, ARRAY_SIZE, 1, 2);

    printf("테스트 배열: ");
    print_array(arr, ARRAY_SIZE);
    printf("\n");

    // ========================================
    // 테스트 케이스 1: 존재하는 값 (중간)
    // ========================================
    printf("\n────────────────────────────────────────\n");
    printf("테스트 1: 중간에 있는 값 탐색 (target = 15)\n");
    printf("────────────────────────────────────────");
    int idx1 = binary_search(arr, ARRAY_SIZE, 15);
    printf("  결과: 인덱스 %d에서 발견\n", idx1);

    // ========================================
    // 테스트 케이스 2: 존재하지 않는 값
    // ========================================
    printf("\n────────────────────────────────────────\n");
    printf("테스트 2: 존재하지 않는 값 탐색 (target = 20)\n");
    printf("────────────────────────────────────────");
    int idx2 = binary_search(arr, ARRAY_SIZE, 20);
    printf("  결과: %d\n", idx2);

    // ========================================
    // 테스트 케이스 3: 첫 번째 값
    // ========================================
    printf("\n────────────────────────────────────────\n");
    printf("테스트 3: 첫 번째 값 탐색 (target = 1)\n");
    printf("────────────────────────────────────────");
    int idx3 = binary_search(arr, ARRAY_SIZE, 1);
    printf("  결과: 인덱스 %d에서 발견\n", idx3);

    // ========================================
    // 테스트 케이스 4: 마지막 값
    // ========================================
    printf("\n────────────────────────────────────────\n");
    printf("테스트 4: 마지막 값 탐색 (target = 31)\n");
    printf("────────────────────────────────────────");
    int idx4 = binary_search(arr, ARRAY_SIZE, 31);
    printf("  결과: 인덱스 %d에서 발견\n", idx4);

    // ========================================
    // 테스트 케이스 5: 재귀 버전
    // ========================================
    printf("\n────────────────────────────────────────\n");
    printf("테스트 5: 재귀 버전 탐색 (target = 9)\n");
    printf("────────────────────────────────────────");
    int idx5 = binary_search_recursive(arr, ARRAY_SIZE, 9);
    printf("  결과: 인덱스 %d에서 발견\n", idx5);

    printf("\n========================================\n");
    printf("이진 탐색의 장점:\n");
    printf("  - 순차 탐색 O(n)보다 훨씬 빠름 O(log n)\n");
    printf("  - 100만 개 중 20번 안에 찾음 (log₂ 1,000,000 ≈ 20)\n");
    printf("========================================\n");

    return 0;
}
