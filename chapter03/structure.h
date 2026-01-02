#ifndef STRUCTURE_H
#define STRUCTURE_H

/* 학생 구조체 정의 */
typedef struct {
    char name[10];
    int age;
    double gpa;
} student;

/* 구조체 출력 함수 */
void print_student(const student *s);

#endif
