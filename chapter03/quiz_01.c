#include <stdio.h>
#include <math.h>

/* 01. 2차원 좌표를 나타내는 구조체 Point 정의 */
typedef struct {
    int x;
    int y;
} Point;

/* 04. 두 점 사이의 거리를 계산하는 함수 */
double get_distance(Point p1, Point p2) {
    int dx = p2.x - p1.x;
    int dy = p2.y - p1.y;
    return sqrt((double)(dx * dx + dy * dy));
}

int main(void) {
    printf("===== Quiz 01 =====\n");

    /* 02. 구조체 변수 p1, p2 정의 */
    /* 03. (1, 2)와 (9, 8)로 초기화 */
    Point p1 = {1, 2};
    Point p2 = {9, 8};

    printf("p1: (%d, %d)\n", p1.x, p1.y);
    printf("p2: (%d, %d)\n", p2.x, p2.y);
    printf("거리: %.2f\n", get_distance(p1, p2));

    return 0;
}
