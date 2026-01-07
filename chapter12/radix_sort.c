#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_SIZE 10
#define RADIX 10  // 10진수

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

// 최댓값 찾기
static int get_max(const int arr[], size_t n) {
    int max = arr[0];
    for (size_t i = 1; i < n; i++) {
        if (arr[i] > max) {
            max = arr[i];
        }
    }
    return max;
}

// 특정 자릿수(exp)를 기준으로 계수 정렬
// exp: 1(1의 자리), 10(10의 자리), 100(100의 자리)...
static void counting_sort(int arr[], size_t n, int exp) {
    int *output = (int *)malloc(n * sizeof(int));
    int *count = (int *)calloc(RADIX, sizeof(int));

    if (output == NULL || count == NULL) {
        free(output);
        free(count);
        return;
    }

    // 현재 자릿수의 빈도 계산
    for (size_t i = 0; i < n; i++) {
        int digit = (arr[i] / exp) % RADIX;
        count[digit]++;
    }

    // 누적 빈도 계산 (안정적 정렬을 위해)
    for (int i = 1; i < RADIX; i++) {
        count[i] += count[i - 1];
    }

    // 안정적으로 출력 배열에 배치 (뒤에서부터)
    for (int i = (int)n - 1; i >= 0; i--) {
        int digit = (arr[i] / exp) % RADIX;
        output[count[digit] - 1] = arr[i];
        count[digit]--;
    }

    // 출력 배열을 원본 배열로 복사
    for (size_t i = 0; i < n; i++) {
        arr[i] = output[i];
    }

    // 현재 자릿수 정렬 결과 출력
    printf("%d의 자리: ", exp);
    print_array_simple(arr, n);
    printf("\n");

    free(output);
    free(count);
}

// 기수 정렬 (LSD: Least Significant Digit)
void radix_sort(int arr[], size_t n) {
    // 최댓값 찾기
    int max = get_max(arr, n);

    printf("최댓값: %d (", max);

    // 자릿수 계산
    int digits = 0;
    int temp = max;
    while (temp > 0) {
        temp /= 10;
        digits++;
    }
    printf("%d자리)\n\n", digits);

    printf("=== 자릿수별 정렬 ===\n");

    // 각 자릿수에 대해 계수 정렬 수행
    // 1의 자리 → 10의 자리 → 100의 자리 → ...
    for (int exp = 1; max / exp > 0; exp *= 10) {
        counting_sort(arr, n, exp);
    }
}

int main(void) {
    int arr[MAX_SIZE];
    const size_t size = MAX_SIZE;

    srand((unsigned int)time(NULL));

    printf("=== 기수 정렬 (Radix Sort) ===\n");
    printf("비교 없이 자릿수별 분배 기반 정렬\n");
    printf("시간 복잡도: O(d × n)\n\n");

    generate_random_array(arr, size, 100);

    printf("원본 배열: ");
    print_array_simple(arr, size);
    printf("\n\n");

    radix_sort(arr, size);

    printf("\n정렬된 배열: ");
    print_array_simple(arr, size);
    printf("\n");

    return 0;
}
