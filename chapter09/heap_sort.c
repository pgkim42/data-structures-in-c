/**
 * Chapter 09: 힙 정렬 (Heap Sort)
 *
 * Max-Heap을 이용한 O(n log n) 정렬 알고리즘
 *
 * 정렬 과정:
 * 1. 배열 → Max-Heap 변환 (build_max_heap): O(n)
 * 2. 루트(최댓값) ↔ 마지막 요소 교환
 * 3. 힙 크기를 1 줄이고 루트에 대해 heapify 수행: O(log n)
 * 4. 2-3을 n-1번 반복
 *
 * 시간 복잡도: O(n log n)
 * 공간 복잡도: O(1) - 제자리 정렬(in-place)
 *
 * 장점:
 * - 최악의 경우에도 O(n log n) 보장
 * - 추가 메모리 불필요
 *
 * 단점:
 * - 불안정 정렬(unstable sort)
 * - 실제로는 퀵 정렬보다 느린 경우가 많음
 */

#include <stdio.h>


// ==================== 보조 함수 ====================

/**
 * 두 값을 교환한다
 */
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * 배열을 출력한다
 */
void print_array(int arr[], int n) {
    printf("[");
    for (int i = 0; i < n; i++) {
        printf("%d", arr[i]);
        if (i < n - 1) {
            printf(", ");
        }
    }
    printf("]");
}


// ==================== 의사코드: heapify ====================

/*
ALGORITHM heapify(arr, n, i)
  // i: heapify를 시작할 노드 인덱스
  // n: 현재 힙의 크기
  largest ← i
  left ← 2*i + 1
  right ← 2*i + 2

  // 왼쪽 자식이 더 크면
  IF (left < n AND arr[left] > arr[largest]) THEN
    largest ← left
  END IF

  // 오른쪽 자식이 더 크면
  IF (right < n AND arr[right] > arr[largest]) THEN
    largest ← right
  END IF

  // largest가 루트가 아니면 교환 후 재귀
  IF (largest ≠ i) THEN
    swap(arr[i], arr[largest])
    heapify(arr, n, largest)  // 재귀 호출
  END IF
*/

/**
 * 힙 속성을 유지한다 (하향 이동, down-heap)
 * @param arr 배열
 * @param n 현재 힙 크기
 * @param i heapify를 시작할 노드 인덱스
 */
void heapify(int arr[], int n, int i) {
    int largest = i;           // 가장 큰 값의 인덱스
    int left = 2 * i + 1;      // 왼쪽 자식
    int right = 2 * i + 2;     // 오른쪽 자식

    // 왼쪽 자식이 현재 노드보다 크면
    if (left < n && arr[left] > arr[largest]) {
        largest = left;
    }

    // 오른쪽 자식이 현재 largest보다 크면
    if (right < n && arr[right] > arr[largest]) {
        largest = right;
    }

    // largest가 루트가 아니면 교환 후 재귀
    if (largest != i) {
        swap(&arr[i], &arr[largest]);
        heapify(arr, n, largest);  // 교환된 자식 위치에서 다시 heapify
    }
}


// ==================== 의사코드: build_max_heap ====================

/*
ALGORITHM build_max_heap(arr, n)
  // 마지막 내부 노드부터 루트까지 역순으로 heapify
  // 마지막 내부 노드 인덱스: (n/2) - 1

  FOR i ← (n/2)-1 DOWNTO 0 DO
    heapify(arr, n, i)
  END FOR
*/

/**
 * 배열을 Max-Heap으로 변환한다
 * @param arr 배열
 * @param n 배열 크기
 */
void build_max_heap(int arr[], int n) {
    // 마지막 내부 노드 인덱스: (n/2) - 1
    // 리프 노드는 이미 힙 속성을 만족하므로 제외
    for (int i = (n / 2) - 1; i >= 0; i--) {
        heapify(arr, n, i);
    }
}


// ==================== 의사코드: heap_sort ====================

/*
ALGORITHM heap_sort(arr, n)
  // 1단계: Max-Heap 구성
  build_max_heap(arr, n)

  // 2단계: 요소 추출 및 정렬
  FOR i ← n-1 DOWNTO 1 DO
    swap(arr[0], arr[i])  // 루트(최댓값)를 마지막으로 이동
    heapify(arr, i, 0)    // 축소된 힙에서 heapify
  END FOR
*/

/**
 * 힙 정렬 수행
 * @param arr 정렬할 배열
 * @param n 배열 크기
 */
void heap_sort(int arr[], int n) {
    // 1단계: 배열 → Max-Heap 변환
    build_max_heap(arr, n);

    // 2단계: 최댓값을 끝으로 보내며 정렬
    for (int i = n - 1; i > 0; i--) {
        swap(&arr[0], &arr[i]);  // 루트(최댓값)를 i 위치로
        heapify(arr, i, 0);       // 힙 크기를 i로 축소 후 heapify
    }
}


// ==================== 테스트 코드 ====================

int main(void) {
    printf("========== 힙 정렬 (Heap Sort) ==========\n\n");

    // 테스트 케이스 1: 기본
    printf("========== 테스트 1: 기본 ==========\n");
    int arr1[] = {4, 10, 3, 5, 1};
    int n1 = sizeof(arr1) / sizeof(arr1[0]);

    printf("정렬 전: ");
    print_array(arr1, n1);
    printf("\n");

    heap_sort(arr1, n1);

    printf("정렬 후: ");
    print_array(arr1, n1);
    printf("\n\n");

    // 테스트 케이스 2: 역순
    printf("========== 테스트 2: 역순 ==========\n");
    int arr2[] = {9, 7, 5, 3, 1};
    int n2 = sizeof(arr2) / sizeof(arr2[0]);

    printf("정렬 전: ");
    print_array(arr2, n2);
    printf("\n");

    heap_sort(arr2, n2);

    printf("정렬 후: ");
    print_array(arr2, n2);
    printf("\n\n");

    // 테스트 케이스 3: 중복 포함
    printf("========== 테스트 3: 중복 포함 ==========\n");
    int arr3[] = {5, 2, 8, 2, 9, 1, 5};
    int n3 = sizeof(arr3) / sizeof(arr3[0]);

    printf("정렬 전: ");
    print_array(arr3, n3);
    printf("\n");

    heap_sort(arr3, n3);

    printf("정렬 후: ");
    print_array(arr3, n3);
    printf("\n\n");

    // 테스트 케이스 4: 이미 정렬됨
    printf("========== 테스트 4: 이미 정렬됨 ==========\n");
    int arr4[] = {1, 2, 3, 4, 5};
    int n4 = sizeof(arr4) / sizeof(arr4[0]);

    printf("정렬 전: ");
    print_array(arr4, n4);
    printf("\n");

    heap_sort(arr4, n4);

    printf("정렬 후: ");
    print_array(arr4, n4);
    printf("\n\n");

    // 테스트 케이스 5: 단일 요소
    printf("========== 테스트 5: 단일 요소 ==========\n");
    int arr5[] = {42};
    int n5 = sizeof(arr5) / sizeof(arr5[0]);

    printf("정렬 전: ");
    print_array(arr5, n5);
    printf("\n");

    heap_sort(arr5, n5);

    printf("정렬 후: ");
    print_array(arr5, n5);
    printf("\n\n");

    // 테스트 케이스 6: 두 요소
    printf("========== 테스트 6: 두 요소 ==========\n");
    int arr6[] = {5, 2};
    int n6 = sizeof(arr6) / sizeof(arr6[0]);

    printf("정렬 전: ");
    print_array(arr6, n6);
    printf("\n");

    heap_sort(arr6, n6);

    printf("정렬 후: ");
    print_array(arr6, n6);
    printf("\n");

    return 0;
}
