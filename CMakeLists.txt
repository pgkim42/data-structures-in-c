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

# 챕터별 실행 파일
add_executable(chapter02 chapter02/main.c)

# chapter03: main.c와 structure.c를 함께 링크
add_executable(chapter03 chapter03/main.c
        chapter03/structure.c)

# 개별 예제 실행 파일들 (각각 독립적인 main 함수 보유)
add_executable(matrix1 chapter03/matrix1.c)      # 일반 행렬 전치
add_executable(matrix2 chapter03/matrix2.c)      # 희소 행렬 전치
add_executable(quiz_01 chapter03/quiz_01.c)      # 구조체 퀴즈
add_executable(quiz_02 chapter03/quiz_02.c)      # 다항식 퀴즈
add_executable(polynomial1 chapter03/polynomial1.c)  # 다항식 덧셈 #1
add_executable(polynomial2 chapter03/polynomial2.c)  # 다항식 덧셈 #2
add_executable(swap chapter03/swap.c)   # 매개변수 스왑
add_executable(array1 chapter03/array1.c)   # 배열의 합
add_executable(malloc chapter03/malloc.c)   # 동적 메모리 할당
add_executable(malloc2 chapter03/malloc2.c)   # 동적 메모리 할당

# 수학 라이브러리 링크 (quiz_02에서 sqrt, pow 함수 사용)
target_link_libraries(quiz_02 PRIVATE m)

# 다른 챕터들
add_executable(chapter04 chapter04/main.c)
add_executable(chapter05 chapter05/main.c)

# ============================================================
# CLion 사용법
# ============================================================
# 1. 빌드: Build > Build Project (Ctrl+F9)
# 2. 실행: Run > Run (Shift+F10)
# 3. 특정 파일만 실행: 우측 상단에서 실행 파일 선택 (chapter03, matrix1, 등)
