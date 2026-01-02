#include <stdio.h>
#include "structure.h"

int main(void) {
    student a = {"kim", 20, 4.3};
    student b = {"park", 21, 4.2};

    printf("===== 구조체 테스트 =====\n");
    print_student(&a);
    print_student(&b);

    return 0;
}
