/*
 * maze.c: 스택을 이용한 미로찾기 (Maze Finding with Backtracking)
 *
 * 스택을 활용하여 미로의 경로를 탐색
 * - 이동한 위치를 스택에 push하여 경로 저장
 * - 막다른 길에서 pop하여 이전 위치로 되돌아감 (백트래킹)
 * - 출구에 도달하면 스택에 저장된 경로가 정답
 */

#include <stdio.h>
#include <stdlib.h>

#define MAX_STACK_SIZE 100
#define MAX_SIZE 20

/* 미로 상수 */
#define PATH 0      /* 길 */
#define WALL 1      /* 벽 */
#define VISITED 2   /* 방문 완료 */

/* 위치 정보를 저장하는 구조체 */
typedef struct {
    int row;    /* 행 인덱스 */
    int col;    /* 열 인덱스 */
} Location;

typedef Location element;

/* 스택 구조체 */
typedef struct {
    element data[MAX_STACK_SIZE];
    int top;
} StackType;

/* 방향 배열 (오른쪽 → 아래 → 왼쪽 → 위: 시계방향) */
int dr[4] = {0, 1, 0, -1};   /* 행 변화량 */
int dc[4] = {1, 0, -1, 0};   /* 열 변화량 */

/* ============================================================
 * 스택 기본 함수
 * ============================================================ */

/* 스택 초기화 */
void init_stack(StackType *s) {
    s->top = -1;
}

/* 공백 상태 검출 */
int is_empty(StackType *s) {
    return s->top == -1;
}

/* 포화 상태 검출 */
int is_full(StackType *s) {
    return s->top == MAX_STACK_SIZE - 1;
}

/* 삽입 */
void push(StackType *s, element item) {
    if (is_full(s)) {
        fprintf(stderr, "스택 포화 상태\n");
        return;
    }
    s->data[++s->top] = item;
}

/* 삭제 */
element pop(StackType *s) {
    if (is_empty(s)) {
        fprintf(stderr, "스택 공백 상태\n");
        exit(1);
    }
    return s->data[s->top--];
}

/* 조회 */
element peek(StackType *s) {
    if (is_empty(s)) {
        fprintf(stderr, "스택 공백 상태\n");
        exit(1);
    }
    return s->data[s->top];
}

/* ============================================================
 * 미로 관련 함수
 * ============================================================ */

/*
 * 미로 출력
 */
void print_maze(int size, int *maze) {
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            printf("%d ", maze[i * size + j]);
        }
        printf("\n");
    }
}

/*
 * 두 위치가 같은지 확인
 */
int is_same_location(Location a, Location b) {
    return a.row == b.row && a.col == b.col;
}

/*
 * 유효한 이동인지 확인
 * - 미로 범위 내
 * - 벽이 아님
 * - 아직 방문하지 않음
 */
int is_valid_move(int row, int col, int size, int *maze, int visited[MAX_SIZE][MAX_SIZE]) {
    /* 범위 검사 */
    if (row < 0 || row >= size || col < 0 || col >= size) {
        return 0;
    }
    /* 벽 또는 이미 방문 */
    if (maze[row * size + col] == WALL || visited[row][col]) {
        return 0;
    }
    return 1;
}

/*
 * 미로찾기 (스택 + 백트래킹)
 *
 * 반환값: 1 = 성공, 0 = 실패
 */
int find_path(int size, int *maze, StackType *path) {
    int visited[MAX_SIZE][MAX_SIZE] = {0};  /* 방문 표시 배열 */

    /* 시작점 push */
    Location start = {0, 0};
    Location exit = {size - 1, size - 1};

    /* 시작점이 벽이면 실패 */
    if (maze[start.row * size + start.col] == WALL) {
        return 0;
    }

    push(path, start);
    visited[start.row][start.col] = 1;

    while (!is_empty(path)) {
        Location current = peek(path);

        /* 출구 도달! */
        if (is_same_location(current, exit)) {
            return 1;
        }

        /* 4방향 탐색 (시계방향) */
        int found = 0;
        for (int dir = 0; dir < 4; dir++) {
            int next_row = current.row + dr[dir];
            int next_col = current.col + dc[dir];

            if (is_valid_move(next_row, next_col, size, maze, visited)) {
                /* 이동 가능 */
                Location next = {next_row, next_col};
                push(path, next);
                visited[next_row][next_col] = 1;
                found = 1;
                break;
            }
        }

        /* 모든 방향이 막혀있으면 백트래킹 */
        if (!found) {
            Location popped = pop(path);
            printf("[백트래킹] (%d,%d)에서 되돌아감\n", popped.row, popped.col);
        }
    }

    /* 스택이 비면 실패 */
    return 0;
}

/*
 * 경로 출력 (스택 내용을 순서대로)
 */
void print_path(StackType *path) {
    /* 스택 내용을 배열로 복사 (스택은 LIFO라 순서가 거꾸로) */
    element temp[MAX_STACK_SIZE];
    int count = 0;

    while (!is_empty(path)) {
        temp[count++] = pop(path);
    }

    /* 역순으로 출력 */
    printf("찾은 경로: ");
    for (int i = count - 1; i >= 0; i--) {
        printf("(%d,%d)", temp[i].row, temp[i].col);
        if (i > 0) {
            printf(" -> ");
        }
        /* 스택에 다시 push (복원) */
        push(path, temp[i]);
    }
    printf("\n");
    printf("이동 횟수: %d\n", count - 1);  /* 시작점 제외 */
}

/*
 * 경로를 미로에 표시하여 출력
 */
void mark_path_on_maze(int size, int *maze, StackType *path) {
    /* 경로를 표시할 임시 배열 복사 */
    int display[MAX_SIZE][MAX_SIZE];
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            display[i][j] = maze[i * size + j];
        }
    }

    /* 스택에 있는 모든 위치 표시 */
    for (int i = 0; i <= path->top; i++) {
        display[path->data[i].row][path->data[i].col] = -1;  /* -1은 경로 표시 */
    }

    /* 출력 (-1을 *로, 나머지는 숫자 그대로) */
    printf("\n경로를 표시한 미로 (*: 경로):\n");
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (display[i][j] == -1) {
                printf("* ");
            } else {
                printf("%d ", display[i][j]);
            }
        }
        printf("\n");
    }
}

/* ============================================================
 * 메인 함수: 테스트 케이스
 * ============================================================ */
int main(void) {
    /* 테스트 케이스 1: 정상 경로 존재 */
    int maze1[5][5] = {
        {0, 0, 1, 1, 1},
        {1, 0, 1, 1, 1},
        {1, 0, 0, 0, 1},
        {1, 1, 1, 0, 1},
        {1, 1, 1, 0, 0}
    };
    int size1 = 5;

    /* 테스트 케이스 2: 경로 없음 */
    int maze2[4][4] = {
        {0, 1, 1, 1},
        {1, 1, 0, 1},
        {1, 0, 1, 1},
        {1, 1, 1, 0}
    };
    int size2 = 4;

    printf("===== 미로찾기 =====\n\n");

    /* 테스트 1 */
    printf("[테스트 1]\n");
    printf("미로 크기: %dx%d\n\n", size1, size1);

    printf("원본 미로:\n");
    print_maze(size1, (int *)maze1);
    printf("\n");

    StackType path1;
    init_stack(&path1);

    if (find_path(size1, (int *)maze1, &path1)) {
        printf("탐색 성공!\n");
        print_path(&path1);
        mark_path_on_maze(size1, (int *)maze1, &path1);
    } else {
        printf("탐색 실패: 출구에 도달할 수 없는 미로입니다.\n");
    }

    /* 테스트 2 */
    printf("\n\n[테스트 2]\n");
    printf("미로 크기: %dx%d\n\n", size2, size2);

    printf("원본 미로:\n");
    print_maze(size2, (int *)maze2);
    printf("\n");

    StackType path2;
    init_stack(&path2);

    if (find_path(size2, (int *)maze2, &path2)) {
        printf("탐색 성공!\n");
        print_path(&path2);
        mark_path_on_maze(size2, (int *)maze2, &path2);
    } else {
        printf("탐색 실패: 출구에 도달할 수 없는 미로입니다.\n");
    }

    return 0;
}
