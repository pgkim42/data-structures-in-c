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

// 배열 일부 출력
static void print_array_range(const int arr[], size_t start, size_t end) {
    printf("[");
    for (size_t i = start; i <= end; i++) {
        printf("%d", arr[i]);
        if (i < end) printf(" ");
    }
    printf("]");
}

// 힙 성질 복구: i를 루트로 하는 서브트리를 힙으로 만듦
static void heapify(int arr[], size_t n, size_t i) {
    size_t largest = i;        // 최댓값 인덱스 (초기: 루트)
    size_t left = 2 * i + 1;   // 왼쪽 자식
    size_t right = 2 * i + 2;  // 오른쪽 자식

    // 왼쪽 자식이 루트보다 크면
    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }

    // 오른쪽 자식이 현재 최댓값보다 크면
    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    // 최댓값이 루트가 아니면 교환하고 재귀
    if (largest != i) {
        int temp = arr[i];
        arr[i] = arr[largest];
        arr[largest] = temp;

        heapify(arr, n, largest);
    }
}

// 최대 힙 생성
static void build_max_heap(int arr[], size_t n) {
    // 마지막 비리프 노드부터 힙화
    for (int i = (int)n / 2 - 1; i >= 0; i--) {
        heapify(arr, n, (size_t)i);
    }
}

// 힙 정렬
void heap_sort(int arr[], size_t n) {
    // 최대 힙 생성
    build_max_heap(arr, n);

    printf("=== 최대 힙 생성 ===\n");
    print_array_simple(arr, n);
    printf("\n\n");

    printf("=== 정렬 과정 ===\n");
    // 원소를 하나씩 추출하며 정렬
    for (size_t i = n - 1; i > 0; i--) {
        // 루트(최댓값)와 마지막 원소 교환
        int temp = arr[0];
        arr[0] = arr[i];
        arr[i] = temp;

        printf("%zu. [%d] 추출 → ", n - i, temp);
        print_array_range(arr, 0, i - 1);
        printf(" | ");
        print_array_range(arr, i, n - 1);
        printf("\n");

        // 줄어든 힙에 대해 heapify
        heapify(arr, i, 0);
    }
}

int main(void) {
    int arr[MAX_SIZE];
    const size_t size = MAX_SIZE;

    srand((unsigned int)time(NULL));

    printf("=== 힙 정렬 (Heap Sort) ===\n");
    printf("최대 힙을 구축하여 최댓값 반복 추출\n");
    printf("항상 O(n log n), in-place 정렬\n\n");

    generate_random_array(arr, size, 100);

    printf("원본 배열: ");
    print_array_simple(arr, size);
    printf("\n\n");

    heap_sort(arr, size);

    printf("\n정렬된 배열: ");
    print_array_simple(arr, size);
    printf("\n");

    return 0;
}
