#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

#define ARRAY_SIZE 20

// ============================================================
// 배열 출력 함수
// ============================================================

// 배열 간단 출력
static void print_array(const int arr[], size_t size) {
    printf("[");
    for (size_t i = 0; i < size; i++) {
        printf("%3d", arr[i]);
        if (i < size - 1) {
            printf(", ");
        }
    }
    printf("]");
}

// 탐색 과정 시각화 (범위와 보간 위치 표시)
static void print_search_state(const int arr[], size_t size,
                                size_t low, size_t high, size_t pos,
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

        // 보간 위치 강조
        if (i == pos) {
            printf("◆%3d◆ ", arr[i]);
        } else if (i >= low && i <= high) {
            printf("%3d  ", arr[i]);
        } else {
            printf("  -- ");
        }

        if (i == high && low != high) {
            printf("]");
        }
    }
    printf("\n");
    printf("     범위: [%zu, %zu], pos: %zu, 비교: %d vs %d → %s\n",
           low, high, pos, arr[pos], target, result);
}

// 보간 공식 계산 과정 출력
static void print_interpolation_formula(const int arr[], size_t low, size_t high,
                                         int target, size_t pos) {
    printf("     ┌─────────────────────────────────────────────┐\n");
    printf("     │ 보간 공식:                                   │\n");
    printf("     │   target - arr[low]                         │\n");
    printf("     │ pos = low + ───────────────── × (high - low)│\n");
    printf("     │           arr[high] - arr[low]              │\n");
    printf("     ├─────────────────────────────────────────────┤\n");
    printf("     │   %d - %d                              │\n", target, arr[low]);
    printf("     │ pos = %zu + ───────────── × (%zu - %zu)      │\n",
           low, arr[high] - arr[low], high, low);
    printf("     │           %d                              │\n", arr[high] - arr[low]);
    printf("     ├─────────────────────────────────────────────┤\n");
    printf("     │ pos = %zu + %d × %zu = %zu                    │\n",
           low, target - arr[low], (high - low) / (arr[high] - arr[low]), pos);
    printf("     └─────────────────────────────────────────────┘\n");
}

// ============================================================
// 보간 탐색 (Interpolation Search)
// ============================================================
// 시간 복잡도 (평균): O(log log n) - 균등 분포 시
// 시간 복잡도 (최악): O(n) - 불균형 분포 시
// 공간 복잡도: O(1)
// ============================================================
// 정렬된 배열에서 값의 비례 관계를 이용해 탐색 위치를 예측
// 전화번호부에서 "A"는 앞, "S"는 뒤에 있을 것이라 예측하는 것과 같은 원리
// ============================================================
int interpolation_search(const int arr[], size_t size, int target) {
    size_t low = 0;
    size_t high = size - 1;

    printf("\n  보간 탐색 시작: target = %d\n", target);
    printf("  초기 배열: ");
    print_array(arr, size);
    printf("\n\n");

    while (low <= high && target >= arr[low] && target <= arr[high]) {
        // low == high인 경우: 남은 요소 하나만 확인
        if (low == high) {
            if (arr[low] == target) {
                printf("  ┌───────────────────────────────────────┐\n");
                printf("  │ 남은 요소 하나 확인: arr[%zu] = %d │\n", low, arr[low]);
                printf("  └───────────────────────────────────────┘\n");
                return (int)low;
            }
            return -1;
        }

        // --------------------------------------------------------
        // 보간 공식으로 위치 계산
        // pos = low + ((target - arr[low]) × (high - low)) / (arr[high] - arr[low])
        // --------------------------------------------------------
        // 분모가 0이 되는 것을 방지하기 위해 arr[high] != arr[low] 확인
        if (arr[high] == arr[low]) {
            // 모든 값이 같은 경우
            printf("  → 모든 값이 %d로 동일합니다.\n", arr[low]);
            return arr[low] == target ? (int)low : -1;
        }

        // 보간 위치 계산 (double로 계산 후 반올림하여 정수로 변환)
        double ratio = (double)(target - arr[low]) / (arr[high] - arr[low]);
        size_t pos = low + (size_t)(ratio * (high - low));

        print_interpolation_formula(arr, low, high, target, pos);

        if (arr[pos] == target) {
            print_search_state(arr, size, low, high, pos, target, "발견! ✓");
            return (int)pos;
        }

        if (arr[pos] < target) {
            print_search_state(arr, size, low, high, pos, target, "오른쪽 탐색");
            low = pos + 1;
        } else {
            print_search_state(arr, size, low, high, pos, target, "왼쪽 탐색");
            if (pos == 0) {
                // underflow 방지
                return -1;
            }
            high = pos - 1;
        }
    }

    printf("  → 탐색 실패: %d을(를) 찾을 수 없음\n", target);
    return -1;
}

// ============================================================
// 보간 탐색 (재귀 버전)
// ============================================================
static int interpolation_search_recursive_impl(const int arr[], int target,
                                               size_t low, size_t high,
                                               bool verbose) {
    // 기저 조건: 범위를 벗어나거나 target이 범위 내에 없음
    if (low > high || target < arr[low] || target > arr[high]) {
        return -1;
    }

    // low == high인 경우
    if (low == high) {
        if (arr[low] == target) {
            if (verbose) {
                printf("     재귀: low==high=%zu, arr[%zu]=%d == target → 발견!\n",
                       low, low, arr[low]);
            }
            return (int)low;
        }
        return -1;
    }

    // 모든 값이 같은 경우
    if (arr[high] == arr[low]) {
        return arr[low] == target ? (int)low : -1;
    }

    // 보간 위치 계산
    double ratio = (double)(target - arr[low]) / (arr[high] - arr[low]);
    size_t pos = low + (size_t)(ratio * (high - low));

    if (verbose) {
        printf("     재귀: low=%zu, high=%zu, pos=%zu, arr[pos]=%d\n",
               low, high, pos, arr[pos]);
    }

    if (arr[pos] == target) {
        if (verbose) {
            printf("          → arr[%zu]=%d == target → 발견!\n", pos, arr[pos]);
        }
        return (int)pos;
    }

    if (arr[pos] < target) {
        if (verbose) {
            printf("          → arr[%zu]=%d < target → 오른쪽\n", pos, arr[pos]);
        }
        return interpolation_search_recursive_impl(arr, target, pos + 1, high, verbose);
    } else {
        if (verbose) {
            printf("          → arr[%zu]=%d > target → 왼쪽\n", pos, arr[pos]);
        }
        if (pos == 0) return -1;
        return interpolation_search_recursive_impl(arr, target, low, pos - 1, verbose);
    }
}

// 재귀 버전 래퍼 함수
int interpolation_search_recursive(const int arr[], size_t size, int target) {
    printf("\n  재귀 버전 탐색 시작: target = %d\n", target);
    int result = interpolation_search_recursive_impl(arr, target, 0, size - 1, true);
    if (result == -1) {
        printf("  → 탐색 실패: %d을(를) 찾을 수 없음\n", target);
    }
    return result;
}

// ============================================================
// 정렬된 배열 생성 (균등 분포)
// ============================================================
static void create_uniform_array(int arr[], size_t size, int start, int step) {
    for (size_t i = 0; i < size; i++) {
        arr[i] = start + (int)(i * step);
    }
}

// ============================================================
// 불균형 배열 생성 (비교용)
// ============================================================
static void create_skewed_array(int arr[], size_t size) {
    // 앞부분에는 작은 값들, 뒤에 하나의 매우 큰 값
    for (size_t i = 0; i < size - 1; i++) {
        arr[i] = (int)(i + 1);  // 1, 2, 3, ...
    }
    arr[size - 1] = 10000;  // 마지막에 매우 큰 값
}

// ============================================================
// 메인 함수
// ============================================================
int main(void) {
    int arr_uniform[ARRAY_SIZE];
    int arr_skewed[ARRAY_SIZE];

    printf("========================================\n");
    printf("   보간 탐색 (Interpolation Search)\n");
    printf("========================================\n");
    printf("조건: 배열이 이미 정렬되어 있어야 함\n");
    printf("시간 복잡도: O(log log n) 평균, O(n) 최악\n");
    printf("공간 복잡도: O(1)\n");
    printf("========================================\n\n");

    // ====================================================================
    // Part 1: 균등 분포 배열 - 보간 탐색이 유리한 경우
    // ====================================================================
    printf("╔════════════════════════════════════════════════════╗\n");
    printf("║  Part 1: 균등 분포 (보간 탐색 유리)                 ║\n");
    printf("╚════════════════════════════════════════════════════╝\n\n");

    create_uniform_array(arr_uniform, ARRAY_SIZE, 10, 5);  // 10, 15, 20, ..., 105

    printf("테스트 배열 (균등 분포): ");
    print_array(arr_uniform, ARRAY_SIZE);
    printf("\n");

    // 테스트 1: 중간 값
    printf("\n────────────────────────────────────────\n");
    printf("테스트 1: 중간 값 (target = 55)\n");
    printf("────────────────────────────────────────");
    int result1 = interpolation_search(arr_uniform, ARRAY_SIZE, 55);
    printf("  결과: 인덱스 %d에서 발견\n", result1);

    // 테스트 2: 앞쪽 값
    printf("\n────────────────────────────────────────\n");
    printf("테스트 2: 앞쪽 값 (target = 20)\n");
    printf("────────────────────────────────────────");
    int result2 = interpolation_search(arr_uniform, ARRAY_SIZE, 20);
    printf("  결과: 인덱스 %d에서 발견\n", result2);

    // 테스트 3: 뒤쪽 값
    printf("\n────────────────────────────────────────\n");
    printf("테스트 3: 뒤쪽 값 (target = 95)\n");
    printf("────────────────────────────────────────");
    int result3 = interpolation_search(arr_uniform, ARRAY_SIZE, 95);
    printf("  결과: 인덱스 %d에서 발견\n", result3);

    // 테스트 4: 존재하지 않는 값
    printf("\n────────────────────────────────────────\n");
    printf("테스트 4: 존재하지 않는 값 (target = 53)\n");
    printf("────────────────────────────────────────");
    int result4 = interpolation_search(arr_uniform, ARRAY_SIZE, 53);
    printf("  결과: %d\n", result4);

    // ====================================================================
    // Part 2: 불균형 분포 배열 - 이진 탐색이 유리한 경우
    // ====================================================================
    printf("\n\n╔════════════════════════════════════════════════════╗\n");
    printf("║  Part 2: 불균형 분포 (이진 탐색 유리)               ║\n");
    printf("╚════════════════════════════════════════════════════╝\n\n");

    create_skewed_array(arr_skewed, ARRAY_SIZE);

    printf("테스트 배열 (불균형): ");
    print_array(arr_skewed, ARRAY_SIZE);
    printf("\n");

    printf("\n────────────────────────────────────────\n");
    printf("테스트 5: 불균형 배열 (target = 5)\n");
    printf("────────────────────────────────────────");
    int result5 = interpolation_search(arr_skewed, ARRAY_SIZE, 5);
    printf("  결과: 인덱스 %d에서 발견\n", result5);

    // ====================================================================
    // Part 3: 재귀 버전 테스트
    // ====================================================================
    printf("\n\n╔════════════════════════════════════════════════════╗\n");
    printf("║  Part 3: 재귀 버전 테스트                          ║\n");
    printf("╚════════════════════════════════════════════════════╝\n\n");

    printf("────────────────────────────────────────\n");
    printf("테스트 6: 재귀 버전 (target = 60)\n");
    printf("────────────────────────────────────────");
    int result6 = interpolation_search_recursive(arr_uniform, ARRAY_SIZE, 60);
    printf("  결과: 인덱스 %d에서 발견\n", result6);

    // ====================================================================
    // Part 4: 성능 비교
    // ====================================================================
    printf("\n\n╔════════════════════════════════════════════════════╗\n");
    printf("║  성능 비교 (균등 분포 %d개 데이터)                 ║\n", ARRAY_SIZE);
    printf("╚════════════════════════════════════════════════════╝\n\n");

    printf("┌─────────────────┬──────────────────┬───────────────┐\n");
    printf("│   알고리즘       │  평균 비교 횟수   │  시간 복잡도   │\n");
    printf("├─────────────────┼──────────────────┼───────────────┤\n");
    printf("│ 순차 탐색       │    %.1f 회        │    O(n)       │\n", (double)ARRAY_SIZE / 2.0);
    printf("│ 이진 탐색       │    %.1f 회        │  O(log n)     │\n", log2((double)ARRAY_SIZE));
    printf("│ 보간 탐색       │    ~1-2 회        │ O(log log n)  │\n");
    printf("└─────────────────┴──────────────────┴───────────────┘\n");

    printf("\n※ 보간 탐색은 균등 분포에서:\n");
    printf("   - 첫 시도에서 정확한 위치에 도달할 확률이 높음\n");
    printf("   - 이진 탐색보다 적은 비교 횟수로 탐색 완료\n\n");

    printf("========================================\n");
    printf("보간 탐색의 특징:\n");
    printf("  ✓ 균등 분포 데이터에서 이진 탐색보다 빠름\n");
    printf("  ✓ O(log log n)은 O(log n)보다 빠름\n");
    printf("  ✗ 불균형 데이터에서는 오히려 느려질 수 있음\n");
    printf("  ✗ 실무 데이터는 대부분 불균형하므로 주의 필요\n");
    printf("========================================\n");

    return 0;
}
