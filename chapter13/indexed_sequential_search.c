#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define DATA_SIZE 50      // 전체 데이터 크기
#define INDEX_SIZE 5      // 색인 테이블 크기
#define BLOCK_SIZE 10     // 각 블록(구간)의 크기 (= DATA_SIZE / INDEX_SIZE)

// ============================================================
// 색인 엔트리 구조체
// ============================================================
typedef struct {
    int key;        // 해당 구간의 최댓값 (색인 키)
    size_t start;   // 데이터 배열에서의 시작 위치
} IndexEntry;

// ============================================================
// 배열 출력 함수
// ============================================================

// 데이터 배열 출력 (구간 구분 포함)
static void print_data_array(const int data[], size_t size, size_t block_size) {
    printf("데이터 배열:\n  ");
    for (size_t i = 0; i < size; i++) {
        printf("%3d", data[i]);
        if ((i + 1) % block_size == 0 && i < size - 1) {
            printf(" |");  // 구간 구분자
        } else {
            printf(",");
        }
    }
    printf("\n\n");
}

// 색인 테이블 출력
static void print_index_table(const IndexEntry index_table[], size_t index_size) {
    printf("색인 테이블:\n");
    printf("  ┌─────┬───────────┬─────────┐\n");
    printf("  │ 인덱스 │   키(Key)  │  시작   │\n");
    printf("  ├─────┼───────────┼─────────┤\n");
    for (size_t i = 0; i < index_size; i++) {
        printf("  │  [%zu] │    %3d    │   %2zu    │\n",
               i, index_table[i].key, index_table[i].start);
    }
    printf("  └─────┴───────────┴─────────┘\n\n");
}

// 탐색 과정 시각화
static void print_search_progress(const int data[], size_t size,
                                   size_t block_size,
                                   size_t search_start, size_t search_end,
                                   int target, const char* message) {
    printf("  %s\n", message);
    printf("    ");
    for (size_t i = 0; i < size; i++) {
        // 탐색 범위 강조
        if (i >= search_start && i <= search_end) {
            printf(" [%2d]", data[i]);
        } else {
            printf("  %2d ", data[i]);
        }

        if ((i + 1) % block_size == 0 && i < size - 1) {
            printf(" |");
        }
    }
    printf("\n");
}

// ============================================================
// 색인 테이블 생성
// ============================================================
// 정렬된 데이터 배열에서 일정한 간격으로 색인 테이블 생성
// 각 색인 엔트리는 해당 구간의 최댓값과 시작 위치를 저장
// ============================================================
static void create_index_table(const int data[], size_t data_size,
                                IndexEntry index_table[], size_t index_size) {
    size_t block_size = data_size / index_size;

    for (size_t i = 0; i < index_size; i++) {
        size_t start = i * block_size;
        size_t end = start + block_size - 1;

        // 마지막 블록은 데이터 끝까지
        if (i == index_size - 1) {
            end = data_size - 1;
        }

        index_table[i].key = data[end];      // 구간의 최댓값
        index_table[i].start = start;        // 구간의 시작 위치
    }
}

// ============================================================
// 색인 순차 탐색 (Indexed Sequential Search)
// ============================================================
// 시간 복잡도: O(n/m + m) = O(n) where m = 색인 테이블 크기
//   - 색인 테이블 탐색: O(m)
//   - 구간 내 순차 탐색: O(n/m)
// 공간 복잡도: O(m) - 색인 테이블 추가 공간
//
// [탐색 단계]
// 1단계: 색인 테이블을 순차 탐색하여 target이 속할 구간 찾기
// 2단계: 찾은 구간의 시작 위치부터 순차 탐색 수행
// 3단계: target을 찾으면 인덱스 반환, 못 찾으면 -1 반환
// ============================================================
int indexed_sequential_search(const int data[], size_t data_size,
                               const IndexEntry index_table[], size_t index_size,
                               int target) {
    printf("\n  탐색 시작: target = %d\n", target);
    printf("  ──────────────────────────────────────\n");

    // --------------------------------------------------------
    // 1단계: 색인 테이블 탐색 (target이 속할 구간 찾기)
    // --------------------------------------------------------
    size_t block_start = 0;
    size_t block_end = data_size - 1;
    size_t found_index = 0;

    printf("\n  [1단계] 색인 테이블 탐색:\n");
    for (size_t i = 0; i < index_size; i++) {
        printf("    index[%zu]: key=%d, start=%zu ", i, index_table[i].key, index_table[i].start);

        if (target <= index_table[i].key) {
            // target이 이 구간에 있거나 이전 구간에 있음
            block_start = index_table[i].start;
            block_end = (i == index_size - 1) ? data_size - 1 : index_table[i].start + BLOCK_SIZE - 1;
            found_index = i;
            printf("→ 선택! (target %d ≤ key %d)\n", target, index_table[i].key);
            break;
        } else {
            printf("→ 통과 (target %d > key %d)\n", target, index_table[i].key);
            if (i == index_size - 1) {
                // 마지막 색인도 초과하면 못 찾음
                printf("    → target이 모든 구간의 최댓값보다 큽니다.\n");
                return -1;
            }
        }
    }

    // --------------------------------------------------------
    // 2단계: 구간 내 순차 탐색
    // --------------------------------------------------------
    printf("\n  [2단계] 구간 [%zu] 순차 탐색: 인덱스 %zu ~ %zu\n",
           found_index, block_start, block_end);

    print_search_progress(data, data_size, BLOCK_SIZE,
                         block_start, block_end, target, "  탐색 범위:");

    for (size_t i = block_start; i <= block_end && i < data_size; i++) {
        printf("    data[%zu] = %d ", i, data[i]);

        if (data[i] == target) {
            printf("→ 발견! ✓\n");
            return (int)i;
        } else if (data[i] > target) {
            printf("→ 초과! 더 이상 찾을 필요 없음 (정렬됨)\n");
            break;
        }
        printf("→ 계속...\n");
    }

    printf("    → 탐색 실패: %d을(를) 찾을 수 없음\n", target);
    return -1;
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
    int data[DATA_SIZE];
    IndexEntry index_table[INDEX_SIZE];

    printf("========================================\n");
    printf("   색인 순차 탐색 (Indexed Sequential Search)\n");
    printf("========================================\n");
    printf("조건: 배열이 이미 정렬되어 있어야 함\n");
    printf("시간 복잡도: O(n/m + m)\n");
    printf("공간 복잡도: O(m) - 색인 테이블\n");
    printf("========================================\n\n");

    // 정렬된 배열 생성 (2, 4, 6, 8, ..., 100)
    create_sorted_array(data, DATA_SIZE, 2, 2);

    // 색인 테이블 생성
    create_index_table(data, DATA_SIZE, index_table, INDEX_SIZE);

    // 데이터 배열 출력
    print_data_array(data, DATA_SIZE, BLOCK_SIZE);

    // 색인 테이블 출력
    print_index_table(index_table, INDEX_SIZE);

    // ========================================
    // 테스트 케이스 1: 중간 구간에 있는 값
    // ========================================
    printf("────────────────────────────────────────\n");
    printf("테스트 1: 중간 구간에 있는 값 (target = 46)\n");
    printf("────────────────────────────────────────");
    int result1 = indexed_sequential_search(data, DATA_SIZE, index_table, INDEX_SIZE, 46);
    printf("  결과: 인덱스 %d에서 발견\n\n", result1);

    // ========================================
    // 테스트 케이스 2: 첫 번째 구간
    // ========================================
    printf("────────────────────────────────────────\n");
    printf("테스트 2: 첫 번째 구간 (target = 14)\n");
    printf("────────────────────────────────────────");
    int result2 = indexed_sequential_search(data, DATA_SIZE, index_table, INDEX_SIZE, 14);
    printf("  결과: 인덱스 %d에서 발견\n\n", result2);

    // ========================================
    // 테스트 케이스 3: 마지막 구간
    // ========================================
    printf("────────────────────────────────────────\n");
    printf("테스트 3: 마지막 구간 (target = 96)\n");
    printf("────────────────────────────────────────");
    int result3 = indexed_sequential_search(data, DATA_SIZE, index_table, INDEX_SIZE, 96);
    printf("  결과: 인덱스 %d에서 발견\n\n", result3);

    // ========================================
    // 테스트 케이스 4: 존재하지 않는 값
    // ========================================
    printf("────────────────────────────────────────\n");
    printf("테스트 4: 존재하지 않는 값 (target = 55)\n");
    printf("────────────────────────────────────────");
    int result4 = indexed_sequential_search(data, DATA_SIZE, index_table, INDEX_SIZE, 55);
    printf("  결과: %d\n\n", result4);

    // ========================================
    // 테스트 케이스 5: 모든 구간의 최댓값보다 큰 값
    // ========================================
    printf("────────────────────────────────────────\n");
    printf("테스트 5: 범위 밖 값 (target = 200)\n");
    printf("────────────────────────────────────────");
    int result5 = indexed_sequential_search(data, DATA_SIZE, index_table, INDEX_SIZE, 200);
    printf("  결과: %d\n\n", result5);

    // ========================================
    // 성능 비교 (시뮬레이션)
    // ========================================
    printf("========================================\n");
    printf("성능 비교 (평균 비교 횟수)\n");
    printf("========================================\n");
    printf("데이터 크기: %d, 색인 테이블 크기: %d\n\n", DATA_SIZE, INDEX_SIZE);

    printf("┌─────────────────────┬──────────────────┐\n");
    printf("│    알고리즘          │  평균 비교 횟수   │\n");
    printf("├─────────────────────┼──────────────────┤\n");
    printf("│ 순차 탐색            │   %.1f 회         │\n", (double)DATA_SIZE / 2.0);
    printf("│ 색인 순차 탐색       │   %.1f 회         │\n",
           (double)INDEX_SIZE / 2.0 + (double)BLOCK_SIZE / 2.0);
    printf("│ 이진 탐색            │   %.1f 회         │\n",
           log2((double)DATA_SIZE));
    printf("└─────────────────────┴──────────────────┘\n");

    printf("\n※ 색인 순차 탐색은:\n");
    printf("   - 색인 테이블 탐색: 평균 %.1f 회\n", (double)INDEX_SIZE / 2.0);
    printf("   - 구간 내 탐색: 평균 %.1f 회\n", (double)BLOCK_SIZE / 2.0);
    printf("   - 합계: %.1f 회\n\n", (double)INDEX_SIZE / 2.0 + (double)BLOCK_SIZE / 2.0);

    printf("========================================\n");
    printf("색인 순차 탐색의 장점:\n");
    printf("  - 대용량 데이터에서 순차 탐색보다 빠름\n");
    printf("  - 디스크 파일처럼 임의 접근 비용이 높은 환경에서 효율적\n");
    printf("  - 색인 테이블만 메모리에 유지하고 데이터는 디스크에서\n");
    printf("========================================\n");

    return 0;
}
