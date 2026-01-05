/**
 * Chapter 09: LPT (Longest Processing Time First) 스케줄링
 *
 * 다중 머신 스케줄링 문제의 근사 해법
 *
 * 문제: m개의 머신에 n개의 작업을 할당하여 전체 완료 시간(makespan)을 최소화
 *
 * LPT 알고리즘:
 * 1. 작업을 처리 시간이 긴 순서대로 정렬 (내림차순)
 * 2. 가장 적게 일하는 머신에 작업 할당
 * 3. 모든 작업이 할당될 때까지 반복
 *
 * 시간 복잡도:
 * - Max-Heap 구성: O(n)
 * - 작업 할당: n × O(log m) = O(n log m)
 * - 전체: O(n log m)
 *
 * 성능 보장: LPT는 최적해의 4/3 - 1/(3m) 배 이내의 해를 보장
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_JOBS 100
#define MAX_MACHINES 10

// 작업 구조체
typedef struct {
    int id;          // 작업 ID
    int processing_time;  // 처리 시간
} Job;

// 머신 구조체
typedef struct {
    int id;          // 머신 ID
    int total_time;  // 현재 총 작업 시간
    int job_count;   // 할당된 작업 수
    int jobs[MAX_JOBS];  // 할당된 작업 ID 리스트
} Machine;


// ==================== Max-Heap (작업용) ====================

/**
 * Max-Heap에서 두 작업을 교환한다
 */
void swap_jobs(Job *a, Job *b) {
    Job temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * 작업 힙에서 상향 이동을 수행한다
 */
void heapify_up(Job heap[], int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap[index].processing_time > heap[parent].processing_time) {
            swap_jobs(&heap[index], &heap[parent]);
            index = parent;
        } else {
            break;
        }
    }
}

/**
 * 작업 힙에서 하향 이동을 수행한다 (Max-Heap)
 */
void heapify_down_jobs(Job heap[], int size, int index) {
    int largest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < size && heap[left].processing_time > heap[largest].processing_time) {
        largest = left;
    }
    if (right < size && heap[right].processing_time > heap[largest].processing_time) {
        largest = right;
    }

    if (largest != index) {
        swap_jobs(&heap[index], &heap[largest]);
        heapify_down_jobs(heap, size, largest);
    }
}

/**
 * 작업을 Max-Heap에 삽입한다
 */
void insert_job(Job heap[], int *size, Job job) {
    heap[*size] = job;
    (*size)++;
    heapify_up(heap, *size - 1);
}

/**
 * Max-Heap에서 최댓값(가장 긴 처리 시간)을 추출한다
 */
Job extract_max_job(Job heap[], int *size) {
    if (*size == 0) {
        Job empty = {-1, 0};
        return empty;
    }

    Job max = heap[0];
    heap[0] = heap[*size - 1];
    (*size)--;
    heapify_down_jobs(heap, *size, 0);

    return max;
}


// ==================== Min-Heap (머신용) ====================

/**
 * Min-Heap에서 두 머신을 교환한다
 */
void swap_machines(Machine *a, Machine *b) {
    Machine temp = *a;
    *a = *b;
    *b = temp;
}

/**
 * 머신 힙에서 하향 이동을 수행한다 (Min-Heap)
 */
void heapify_down_machines(Machine heap[], int size, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    // total_time이 작은 것이 우선 (Min-Heap)
    if (left < size && heap[left].total_time < heap[smallest].total_time) {
        smallest = left;
    }
    if (right < size && heap[right].total_time < heap[smallest].total_time) {
        smallest = right;
    }

    if (smallest != index) {
        swap_machines(&heap[index], &heap[smallest]);
        heapify_down_machines(heap, size, smallest);
    }
}

/**
 * Min-Heap에서 최솟값(가장 적게 일하는 머신)을 추출한다
 */
Machine extract_min_machine(Machine heap[], int *size) {
    Machine min = heap[0];
    heap[0] = heap[*size - 1];
    (*size)--;
    heapify_down_machines(heap, *size, 0);
    return min;
}

/**
 * 머신을 Min-Heap에 다시 삽입한다 (작업 할당 후)
 */
void reinsert_machine(Machine heap[], int *size, Machine machine) {
    heap[*size] = machine;
    (*size)++;

    // 상향 이동
    int index = *size - 1;
    while (index > 0) {
        int parent = (index - 1) / 2;
        if (heap[index].total_time < heap[parent].total_time) {
            swap_machines(&heap[index], &heap[parent]);
            index = parent;
        } else {
            break;
        }
    }
}


// ==================== LPT 알고리즘 ====================

/*
ALGORITHM LPT(jobs[], n, m)
  // jobs: 작업 배열, n: 작업 수, m: 머신 수
  // 1단계: 작업을 처리 시간 내림차순으로 정렬 (Max-Heap 사용)
  job_heap ← 빈 Max-Heap
  FOR each job IN jobs DO
    insert_job(job_heap, job)
  END FOR

  // 2단계: 머신 초기화 (Min-Heap으로 관리)
  machine_heap ← 빈 Min-Heap
  FOR i ← 0 TO m-1 DO
    machine.id ← i
    machine.total_time ← 0
    machine.job_count ← 0
    insert_machine(machine_heap, machine)
  END FOR

  // 3단계: 작업 할당
  WHILE job_heap이 비어있지 않을 때
    job ← extract_max_job(job_heap)  // 가장 긴 작업
    machine ← extract_min_machine(machine_heap)  // 가장 한가한 머신

    // 작업 할당
    machine.total_time ← machine.total_time + job.processing_time
    machine.jobs[machine.job_count] ← job.id
    machine.job_count ← machine.job_count + 1

    reinsert_machine(machine_heap, machine)
  END WHILE

  // 4단계: makespan 계산 (가장 늦게 끝나는 머신의 시간)
  RETURN max(machine.total_time for all machines)
*/

/**
 * LPT 스케줄링을 수행한다
 * @param jobs 작업 배열
 * @param n 작업 수
 * @param m 머신 수
 * @param machines 결과 머신 배열
 * @return makespan (전체 완료 시간)
 */
int lpt_schedule(Job jobs[], int n, int m, Machine machines[]) {
    // 1단계: 작업을 Max-Heap에 저장 (처리 시간 내림차순)
    Job job_heap[MAX_JOBS];
    int job_heap_size = 0;

    for (int i = 0; i < n; i++) {
        insert_job(job_heap, &job_heap_size, jobs[i]);
    }

    // 2단계: 머신 Min-Heap 초기화
    Machine machine_heap[MAX_MACHINES];
    int machine_heap_size = 0;

    for (int i = 0; i < m; i++) {
        machine_heap[i].id = i;
        machine_heap[i].total_time = 0;
        machine_heap[i].job_count = 0;
        machine_heap_size++;
    }

    // Min-Heap 구성
    for (int i = (machine_heap_size / 2) - 1; i >= 0; i--) {
        heapify_down_machines(machine_heap, machine_heap_size, i);
    }

    // 3단계: 작업 할당
    printf("\n========== 작업 할당 과정 ==========\n");
    while (job_heap_size > 0) {
        // 가장 긴 작업 추출
        Job job = extract_max_job(job_heap, &job_heap_size);

        // 가장 한가한 머신 추출
        Machine machine = extract_min_machine(machine_heap, &machine_heap_size);

        printf("작업 J%d(%d) → 머신 M%d (이전 시간: %d)",
               job.id, job.processing_time, machine.id, machine.total_time);

        // 작업 할당
        machine.total_time += job.processing_time;
        machine.jobs[machine.job_count] = job.id;
        machine.job_count++;

        printf(" → 새 시간: %d\n", machine.total_time);

        // 머신을 다시 힙에 삽입
        reinsert_machine(machine_heap, &machine_heap_size, machine);
    }

    // 결과를 machines 배열에 복사
    for (int i = 0; i < m; i++) {
        machines[i] = machine_heap[i];
    }

    // 4단계: makespan 계산
    int makespan = 0;
    for (int i = 0; i < m; i++) {
        if (machine_heap[i].total_time > makespan) {
            makespan = machine_heap[i].total_time;
        }
    }

    return makespan;
}


// ==================== 테스트 코드 ====================

/**
 * 스케줄링 결과를 출력한다
 */
void print_schedule(Machine machines[], int m) {
    printf("\n========== 스케줄링 결과 ==========\n");
    int makespan = 0;

    for (int i = 0; i < m; i++) {
        printf("머신 M%d (총 %d): ", machines[i].id, machines[i].total_time);
        for (int j = 0; j < machines[i].job_count; j++) {
            printf("J%d", machines[i].jobs[j]);
            if (j < machines[i].job_count - 1) {
                printf(" → ");
            }
        }
        printf("\n");

        if (machines[i].total_time > makespan) {
            makespan = machines[i].total_time;
        }
    }

    printf("\nMakespan (전체 완료 시간): %d\n", makespan);
}

int main(void) {
    printf("========== LPT (Longest Processing Time First) 스케줄링 ==========\n");

    // 테스트 케이스 1
    printf("\n========== 테스트 1 ==========\n");
    Job jobs1[] = {
        {1, 2}, {2, 4}, {3, 3}, {4, 5}, {5, 1}, {6, 6}, {7, 2}
    };
    int n1 = sizeof(jobs1) / sizeof(jobs1[0]);
    int m1 = 3;  // 3개의 머신

    printf("작업: ");
    for (int i = 0; i < n1; i++) {
        printf("J%d(%d) ", jobs1[i].id, jobs1[i].processing_time);
    }
    printf("\n머신 수: %d\n", m1);

    Machine machines1[MAX_MACHINES];
    int makespan1 = lpt_schedule(jobs1, n1, m1, machines1);
    print_schedule(machines1, m1);

    // 테스트 케이스 2
    printf("\n========== 테스트 2 ==========\n");
    Job jobs2[] = {
        {1, 8}, {2, 7}, {3, 6}, {4, 5}, {5, 4}, {6, 3}, {7, 2}, {8, 1}
    };
    int n2 = sizeof(jobs2) / sizeof(jobs2[0]);
    int m2 = 3;

    printf("작업: ");
    for (int i = 0; i < n2; i++) {
        printf("J%d(%d) ", jobs2[i].id, jobs2[i].processing_time);
    }
    printf("\n머신 수: %d\n", m2);

    Machine machines2[MAX_MACHINES];
    int makespan2 = lpt_schedule(jobs2, n2, m2, machines2);
    print_schedule(machines2, m2);

    // 테스트 케이스 3
    printf("\n========== 테스트 3 ==========\n");
    Job jobs3[] = {
        {1, 5}, {2, 10}, {3, 3}, {4, 7}, {5, 8}, {6, 4}
    };
    int n3 = sizeof(jobs3) / sizeof(jobs3[0]);
    int m3 = 2;

    printf("작업: ");
    for (int i = 0; i < n3; i++) {
        printf("J%d(%d) ", jobs3[i].id, jobs3[i].processing_time);
    }
    printf("\n머신 수: %d\n", m3);

    Machine machines3[MAX_MACHINES];
    int makespan3 = lpt_schedule(jobs3, n3, m3, machines3);
    print_schedule(machines3, m3);

    return 0;
}
