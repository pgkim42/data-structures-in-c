/*
 * malloc2.c: 구조체 동적 메모리 할당
 *
 * 구조체 포인터에 malloc()으로 메모리를 할당하고,
 * 화살표 연산자(->)로 멤버에 접근한다.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 학생 구조체 정의 */
typedef struct {
    char name[10];
    int age;
    double gpa;
} student;

int main(void) {
    student *s;

    /* 구조체 크기만큼 메모리 할당 */
    s = (student *)malloc(sizeof(student));

    if (s == NULL) {
        fprintf(stderr, "메모리 할당 실패\n");
        exit(1);
    }

    /* 화살표 연산자로 구조체 멤버 접근 */
    strcpy(s->name, "John");
    s->age = 20;
    s->gpa = 3.8;

    printf("===== 구조체 동적 할당 =====\n\n");
    printf("이름: %s\n", s->name);
    printf("나이: %d\n", s->age);
    printf("학점: %.1f\n", s->gpa);

    /* 메모리 반납 */
    free(s);
    s = NULL;

    return 0;
}
