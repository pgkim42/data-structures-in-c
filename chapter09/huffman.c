/**
 * Chapter 09: 허프만 코딩 (Huffman Coding)
 *
 * 최소 힙(Min-Heap)을 이용한 무손실 압축 알고리즘
 *
 * 허프만 코딩 원리:
 * 1. 각 문자의 빈도수 계산
 * 2. 빈도수를 가중치로 Min-Heap에 리프 노드로 삽입
 * 3. 가장 작은 두 노드 추출 → 병합 → 힙에 재삽입
 * 4. 하나의 노드만 남을 때까지 반복
 * 5. 루트에서 각 리프까지의 경로가 허프만 코드
 *
 * 시간 복잡도: O(n + k log k)
 * - n: 입력 문자열 길이
 * - k: 고유 문자 개수
 *
 * 예시: "abbccc"
 *   빈도: a=1, b=2, c=3
 *   코드: c=0, b=10, a=11 (빈도가 높을수록 짧은 코드)
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_HEAP_SIZE 256
#define MAX_CODE_LEN 100
#define ASCII_SIZE 256


// ==================== 데이터 구조 ====================

/**
 * 허프만 트리 노드
 */
typedef struct HuffmanNode {
    char character;      // 문자 (내부 노드는 '\0')
    int frequency;       // 빈도수 (가중치)
    struct HuffmanNode *left;
    struct HuffmanNode *right;
} HuffmanNode;

/**
 * Min-Heap (허프만 노드용)
 */
typedef struct {
    HuffmanNode *heap[MAX_HEAP_SIZE];
    int heap_size;
} HuffmanHeap;

/**
 * 허프만 코드 저장소
 */
typedef struct {
    char code[MAX_CODE_LEN];  // 허프만 코드 문자열
    int length;                // 코드 길이
} HuffmanCode;


// ==================== 유틸리티 함수 ====================

/**
 * 오류 메시지를 출력하고 프로그램을 종료한다
 */
void error(char *message) {
    fprintf(stderr, "%s\n", message);
    exit(1);
}

/**
 * 새로운 허프만 노드를 생성한다
 */
HuffmanNode* create_node(char ch, int freq) {
    HuffmanNode *node = (HuffmanNode *)malloc(sizeof(HuffmanNode));
    if (node == NULL) {
        error("메모리 할당 실패");
    }
    node->character = ch;
    node->frequency = freq;
    node->left = NULL;
    node->right = NULL;
    return node;
}


// ==================== Min-Heap 연산 ====================

/**
 * Min-Heap을 초기화한다
 */
void init_huffman_heap(HuffmanHeap *h) {
    h->heap_size = 0;
}

/**
 * Min-Heap이 공백상태인지 확인한다
 */
int is_huffman_heap_empty(HuffmanHeap *h) {
    return h->heap_size == 0;
}

/**
 * Min-Heap의 상향 이동을 수행한다
 */
void heapify_up(HuffmanHeap *h, int index) {
    while (index > 0) {
        int parent = (index - 1) / 2;
        // 빈도수가 작은 것이 우선 (Min-Heap)
        if (h->heap[index]->frequency < h->heap[parent]->frequency) {
            // 교환
            HuffmanNode *temp = h->heap[index];
            h->heap[index] = h->heap[parent];
            h->heap[parent] = temp;
            index = parent;
        } else {
            break;
        }
    }
}

/**
 * Min-Heap의 하향 이동을 수행한다
 */
void heapify_down_huffman(HuffmanHeap *h, int index) {
    int smallest = index;
    int left = 2 * index + 1;
    int right = 2 * index + 2;

    if (left < h->heap_size &&
        h->heap[left]->frequency < h->heap[smallest]->frequency) {
        smallest = left;
    }
    if (right < h->heap_size &&
        h->heap[right]->frequency < h->heap[smallest]->frequency) {
        smallest = right;
    }

    if (smallest != index) {
        HuffmanNode *temp = h->heap[index];
        h->heap[index] = h->heap[smallest];
        h->heap[smallest] = temp;
        heapify_down_huffman(h, smallest);
    }
}

/**
 * Min-Heap에 노드를 삽입한다
 */
void insert_huffman_heap(HuffmanHeap *h, HuffmanNode *node) {
    if (h->heap_size >= MAX_HEAP_SIZE) {
        error("힙이 포화상태입니다.");
    }
    h->heap[h->heap_size] = node;
    h->heap_size++;
    heapify_up(h, h->heap_size - 1);
}

/**
 * Min-Heap에서 최소 빈도 노드를 추출한다
 */
HuffmanNode* extract_min_huffman(HuffmanHeap *h) {
    if (is_huffman_heap_empty(h)) {
        error("힙이 공백상태입니다.");
    }

    HuffmanNode *min = h->heap[0];
    h->heap[0] = h->heap[h->heap_size - 1];
    h->heap_size--;
    heapify_down_huffman(h, 0);

    return min;
}


// ==================== 의사코드: build_huffman_tree ====================

/*
ALGORITHM build_huffman_tree(text)
  freq[ASCII_SIZE] ← {0}

  // 1단계: 빈도수 계산
  FOR each character ch IN text DO
    freq[ch] ← freq[ch] + 1
  END FOR

  // 2단계: Min-Heap 구성
  heap ← empty Min-Heap
  FOR i ← 0 TO ASCII_SIZE-1 DO
    IF freq[i] > 0 THEN
      node ← create_node((char)i, freq[i])
      insert_heap(heap, node)
    END IF
  END FOR

  // 3단계: 트리 구성
  WHILE heap.size > 1 DO
    left ← extract_min(heap)
    right ← extract_min(heap)

    merged ← create_node('\0', left.frequency + right.frequency)
    merged.left ← left
    merged.right ← right

    insert_heap(heap, merged)
  END WHILE

  IF heap.size = 0 THEN
    RETURN NULL  // 빈 입력
  END IF

  RETURN extract_min(heap)  // 루트
*/

/**
 * 텍스트로부터 허프만 트리를 구성한다
 * @param text 입력 텍스트
 * @return 허프만 트리의 루트 노드
 */
HuffmanNode* build_huffman_tree(char *text) {
    // 빈도수 배열 초기화
    int freq[ASCII_SIZE] = {0};

    // 1단계: 빈도수 계산
    int len = strlen(text);
    for (int i = 0; i < len; i++) {
        freq[(unsigned char)text[i]]++;
    }

    // 2단계: Min-Heap 구성
    HuffmanHeap heap;
    init_huffman_heap(&heap);

    for (int i = 0; i < ASCII_SIZE; i++) {
        if (freq[i] > 0) {
            HuffmanNode *node = create_node((char)i, freq[i]);
            insert_huffman_heap(&heap, node);
        }
    }

    // 빈 입력 처리
    if (is_huffman_heap_empty(&heap)) {
        return NULL;
    }

    // 단일 문자만 있는 경우
    if (heap.heap_size == 1) {
        return extract_min_huffman(&heap);
    }

    // 3단계: 트리 구성
    while (heap.heap_size > 1) {
        HuffmanNode *left = extract_min_huffman(&heap);
        HuffmanNode *right = extract_min_huffman(&heap);

        // 내부 노드 생성 (빈도수 합산)
        HuffmanNode *merged = create_node('\0', left->frequency + right->frequency);
        merged->left = left;
        merged->right = right;

        insert_huffman_heap(&heap, merged);
    }

    return extract_min_huffman(&heap);  // 루트
}


// ==================== 의사코드: generate_codes ====================

/*
ALGORITHM generate_codes(node, path[], depth, codes[])
  IF node is NULL THEN
    RETURN
  END IF

  // 리프 노드면 코드 저장
  IF node.character ≠ '\0' THEN
    path[depth] ← '\0'  // 문자열 종료
    strcpy(codes[node.character].code, path)
    codes[node.character].length ← depth
    RETURN
  END IF

  // 왼쪽: 0 추가 (path에 직접 기록)
  path[depth] ← '0'
  generate_codes(node.left, path, depth + 1, codes)

  // 오른쪽: 1 추가
  path[depth] ← '1'
  generate_codes(node.right, path, depth + 1, codes)
*/

/**
 * 허프만 트리에서 각 문자의 코드를 생성한다
 * @param root 허프만 트리의 루트
 * @param codes 코드 저장소 배열
 */
void generate_codes(HuffmanNode *root, HuffmanCode codes[]) {
    if (root == NULL) {
        return;
    }

    // 재귀적으로 코드 생성을 위한 보조 함수
    void traverse(HuffmanNode *node, char path[], int depth) {
        if (node == NULL) {
            return;
        }

        // 리프 노드면 코드 저장
        if (node->character != '\0') {
            path[depth] = '\0';  // 문자열 종료
            strcpy(codes[(unsigned char)node->character].code, path);
            codes[(unsigned char)node->character].length = depth;
            return;
        }

        // 왼쪽: 0 추가
        path[depth] = '0';
        traverse(node->left, path, depth + 1);

        // 오른쪽: 1 추가
        path[depth] = '1';
        traverse(node->right, path, depth + 1);
    }

    char path[MAX_CODE_LEN];
    traverse(root, path, 0);
}


// ==================== 출력 및 인코딩 함수 ====================

/**
 * 생성된 허프만 코드를 출력한다
 */
void print_codes(HuffmanCode codes[], char *text) {
    printf("\n========== 허프만 코드 ==========\n");
    printf("문자\t빈도\t코드\n");
    printf("----\t----\t----\n");

    // 이미 출력된 문자 확인용
    int printed[ASCII_SIZE] = {0};

    for (int i = 0; text[i] != '\0'; i++) {
        unsigned char ch = (unsigned char)text[i];
        if (!printed[ch] && codes[ch].length > 0) {
            if (ch == ' ') {
                printf("SPC\t");
            } else if (ch == '\n') {
                printf("NL\n");
                continue;
            } else if (ch == '\t') {
                printf("TAB\n");
                continue;
            } else {
                printf("%c\t", ch);
            }
            printf("%d\t%s\n", codes[ch].length, codes[ch].code);
            printed[ch] = 1;
        }
    }
}

/**
 * 텍스트를 허프만 코드로 인코딩하여 출력한다
 */
void encode(char *text, HuffmanCode codes[]) {
    printf("\n========== 인코딩 결과 ==========\n");
    printf("원문: %s\n", text);
    printf("코드: ");

    for (int i = 0; text[i] != '\0'; i++) {
        unsigned char ch = (unsigned char)text[i];
        printf("%s", codes[ch].code);
        if (text[i + 1] != '\0') {
            printf(" ");
        }
    }
    printf("\n");

    // 원래 비트 수 vs 압축 후 비트 수
    int original_bits = strlen(text) * 8;
    int compressed_bits = 0;
    for (int i = 0; text[i] != '\0'; i++) {
        unsigned char ch = (unsigned char)text[i];
        compressed_bits += codes[ch].length;
    }

    printf("\n========== 압축 효율 ==========\n");
    printf("원래 비트 수: %d bits (8 bits × %d 문자)\n", original_bits, (int)strlen(text));
    printf("압축 비트 수: %d bits\n", compressed_bits);

    if (original_bits > 0) {
        double ratio = (double)compressed_bits / original_bits * 100;
        printf("압축율: %.1f%%\n", ratio);
    }
}


// ==================== 메모리 해제 ====================

/**
 * 허프만 트리의 메모리를 해제한다 (후위 순회)
 */
void free_tree(HuffmanNode *root) {
    if (root == NULL) {
        return;
    }
    free_tree(root->left);
    free_tree(root->right);
    free(root);
}


// ==================== 테스트 코드 ====================

int main(void) {
    printf("========== 허프만 코딩 (Huffman Coding) ==========\n");

    // 코드 저장소 초기화
    HuffmanCode codes[ASCII_SIZE];
    for (int i = 0; i < ASCII_SIZE; i++) {
        codes[i].code[0] = '\0';
        codes[i].length = 0;
    }

    // 테스트 케이스 1: 기본
    printf("\n========== 테스트 1: 기본 ==========\n");
    char text1[] = "abbccc";
    printf("입력: %s\n", text1);

    HuffmanNode *root1 = build_huffman_tree(text1);
    if (root1 != NULL) {
        generate_codes(root1, codes);
        print_codes(codes, text1);
        encode(text1, codes);
        free_tree(root1);
    }

    // 코드 초기화
    for (int i = 0; i < ASCII_SIZE; i++) {
        codes[i].code[0] = '\0';
        codes[i].length = 0;
    }

    // 테스트 케이스 2: 영문장
    printf("\n========== 테스트 2: 영문장 ==========\n");
    char text2[] = "hello world";
    printf("입력: %s\n", text2);

    HuffmanNode *root2 = build_huffman_tree(text2);
    if (root2 != NULL) {
        generate_codes(root2, codes);
        print_codes(codes, text2);
        encode(text2, codes);
        free_tree(root2);
    }

    // 코드 초기화
    for (int i = 0; i < ASCII_SIZE; i++) {
        codes[i].code[0] = '\0';
        codes[i].length = 0;
    }

    // 테스트 케이스 3: 빈도 불균형
    printf("\n========== 테스트 3: 빈도 불균형 ==========\n");
    char text3[] = "aaaabbbccd";
    printf("입력: %s\n", text3);

    HuffmanNode *root3 = build_huffman_tree(text3);
    if (root3 != NULL) {
        generate_codes(root3, codes);
        print_codes(codes, text3);
        encode(text3, codes);
        free_tree(root3);
    }

    // 코드 초기화
    for (int i = 0; i < ASCII_SIZE; i++) {
        codes[i].code[0] = '\0';
        codes[i].length = 0;
    }

    // 테스트 케이스 4: 단일 문자
    printf("\n========== 테스트 4: 단일 문자 ==========\n");
    char text4[] = "aaaaa";
    printf("입력: %s\n", text4);

    HuffmanNode *root4 = build_huffman_tree(text4);
    if (root4 != NULL) {
        generate_codes(root4, codes);
        print_codes(codes, text4);
        encode(text4, codes);
        free_tree(root4);
    }

    return 0;
}
