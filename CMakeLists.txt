# CMake 최소 버전 요구
cmake_minimum_required(VERSION 4.0)

# 프로젝트 이름과 사용 언어 (C)
project(untitled C)

# C 표준 버전 설정 (C11)
set(CMAKE_C_STANDARD 11)

# ============================================================
# 실행 파일 정의
# add_executable(실행파일이름 소스파일들...)
# 각 파일이 독립적으로 main() 함수를 가짐
# ============================================================

# ------------------------------------------------------------
# Chapter 02: 순환 (Recursion)
# ------------------------------------------------------------
add_executable(chapter02 chapter02/main.c)

# ------------------------------------------------------------
# Chapter 03: 배열, 구조체, 포인터
# ------------------------------------------------------------
add_executable(chapter03 chapter03/main.c
        chapter03/structure.c)

# 구조체와 포인터
add_executable(array1      chapter03/array1.c)      # 배열과 포인터
add_executable(swap        chapter03/swap.c)        # 포인터 swap
add_executable(malloc      chapter03/malloc.c)      # 동적 메모리 #1
add_executable(malloc2     chapter03/malloc2.c)     # 동적 메모리 #2

# 다항식
add_executable(polynomial1 chapter03/polynomial1.c) # 다항식 (배열 방식)
add_executable(polynomial2 chapter03/polynomial2.c) # 다항식 (희소 방식)

# 행렬
add_executable(matrix1     chapter03/matrix1.c)     # 행렬 전치 (일반)
add_executable(matrix2     chapter03/matrix2.c)     # 행렬 전치 (희소)

# 퀴즈
add_executable(quiz_01     chapter03/quiz_01.c)     # 구조체 퀴즈
add_executable(quiz_02     chapter03/quiz_02.c)     # 다항식 퀴즈
target_link_libraries(quiz_02 PRIVATE m)            # 수학 라이브러리

# ------------------------------------------------------------
# Chapter 04: 스택 (Stack)
# ------------------------------------------------------------
add_executable(chapter04           chapter04/main.c)
add_executable(stack_integer       chapter04/stack_integer.c)       # 정수 스택
add_executable(stack_struct_array  chapter04/stack_struct_array.c)  # 구조체 스택
add_executable(stack_array         chapter04/stack_array.c)         # 일반적인 배열 스택
add_executable(stack_dynamic_array chapter04/stack_dynamic_array.c) # 동적 배열 스택 (확장 가능)
add_executable(bracket_checker     chapter04/bracket_checker.c)     # 괄호 짝 검사
add_executable(postfix_eval        chapter04/postfix_eval.c)        # 후위 표기 계산
add_executable(infix_to_postfix    chapter04/infix_to_postfix.c)    # 중위→후위 변환
add_executable(maze                chapter04/maze.c)                # 미로찾기

# ------------------------------------------------------------
# Chapter 05: 큐 (Queue)
# ------------------------------------------------------------
add_executable(chapter05         chapter05/main.c)
add_executable(linear_queue      chapter05/linear_queue.c)           # 선형 큐
add_executable(circular_queue    chapter05/circular_queue.c)          # 원형 큐
add_executable(circular_queue_sim chapter05/circular_queue_sim.c)     # 원형 큐 시뮬레이션
add_executable(deque               chapter05/deque.c)                   # 덱 (Double-Ended Queue)
add_executable(bank_simulation     chapter05/bank_simulation.c)         # 은행 서비스 시뮬레이션

# ============================================================
# CLion 사용법
# ============================================================
# 1. 빌드: Build > Build Project (Ctrl+F9)
# 2. 실행: Run > Run (Shift+F10)
# 3. 특정 파일만 실행: 우측 상단에서 실행 파일 선택
#    (chapter02, chapter03, stack_integer, 등)
