#include <stdio.h>
#include "structure.h"

/* 구조체 초기화 및 출력 함수 */
void print_student(const student *s) {
    printf("이름: %s, 나이: %d, 학점: %.1f\n", s->name, s->age, s->gpa);
}
