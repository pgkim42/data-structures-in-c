/**
 * Chapter 08: 이진 탐색 트리 (Binary Search Tree, BST)
 *
 * BST 속성: 왼쪽 서브트리 < 루트 < 오른쪽 서브트리
 *
 *          50
 *        /    \
 *      30      70
 *     /  \    /  \
 *   20   40  60   80
 *
 * 중위 순회 결과: 20 30 40 50 60 70 80 (오름차순 정렬)
 *
 * 시간 복잡도 (h = 트리 높이):
 * - 탐색: O(h)  → 평균 O(log n), 최악 O(n)
 * - 삽입: O(h)
 * - 삭제: O(h)
 */

#include <stdio.h>
#include <stdlib.h>

// ==================== 노드 구조체 정의 ====================

typedef struct BSTNode {
    int key;                        // 키 값 (중복 없음)
    struct BSTNode *left;
    struct BSTNode *right;
} BSTNode;


// ==================== 기본 연산 ====================

/**
 * 새로운 노드 생성
 */
BSTNode* create_bst_node(int key) {
    BSTNode *node = (BSTNode *)malloc(sizeof(BSTNode));
    if (node == NULL) {
        fprintf(stderr, "메모리 할당 실패\n");
        exit(1);
    }
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    return node;
}


// ==================== 의사코드: 삽입 ====================

/*
ALGORITHM bst_insert(root, key)
  IF root is NULL THEN
    RETURN create_node(key)
  END IF

  IF key < root.key THEN
    root.left ← bst_insert(root.left, key)    // 왼쪽으로 이동
  ELSE IF key > root.key THEN
    root.right ← bst_insert(root.right, key)  // 오른쪽으로 이동
  END IF
  // key == root.key인 경우 중복은 무시

  RETURN root
*/

/**
 * 키 삽입 (재귀)
 * @param root 현재 서브트리의 루트
 * @param key 삽입할 키
 * @return 삽입后的 서브트리의 루트
 */
BSTNode* bst_insert(BSTNode *root, int key) {
    // 빈 트리이면 새 노드 생성
    if (root == NULL) {
        return create_bst_node(key);
    }

    // 왼쪽 서브트리로
    if (key < root->key) {
        root->left = bst_insert(root->left, key);
    }
    // 오른쪽 서브트리로
    else if (key > root->key) {
        root->right = bst_insert(root->right, key);
    }
    // key == root->key: 중복은 무시

    return root;
}


// ==================== 의사코드: 탐색 ====================

/*
ALGORITHM bst_search(root, key)
  IF root is NULL THEN
    RETURN NULL              // 찾지 못함
  END IF

  IF key == root.key THEN
    RETURN root              // 찾음
  ELSE IF key < root.key THEN
    RETURN bst_search(root.left, key)
  ELSE
    RETURN bst_search(root.right, key)
  END IF
*/

/**
 * 키 탐색
 * @param root 루트 노드
 * @param key 찾을 키
 * @return 찾은 노드 포인터, 없으면 NULL
 */
BSTNode* bst_search(BSTNode *root, int key) {
    if (root == NULL) {
        return NULL;  // 찾지 못함
    }

    if (key == root->key) {
        return root;  // 찾음
    }
    else if (key < root->key) {
        return bst_search(root->left, key);
    }
    else {
        return bst_search(root->right, key);
    }
}


// ==================== 의사코드: 최솟값/최댓값 ====================

/*
ALGORITHM bst_find_min(root)
  IF root is NULL THEN
    RETURN NULL
  END IF
  WHILE root.left is NOT NULL DO
    root ← root.left     // 가장 왼쪽이 최솟값
  END WHILE
  RETURN root
*/

/**
 * 최솟값 찾기 (가장 왼쪽 노드)
 */
BSTNode* bst_find_min(BSTNode *root) {
    if (root == NULL) {
        return NULL;
    }
    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

/**
 * 최댓값 찾기 (가장 오른쪽 노드)
 */
BSTNode* bst_find_max(BSTNode *root) {
    if (root == NULL) {
        return NULL;
    }
    while (root->right != NULL) {
        root = root->right;
    }
    return root;
}


// ==================== 의사코드: 삭제 ====================

/*
ALGORITHM bst_delete(root, key)
  IF root is NULL THEN
    RETURN NULL
  END IF

  IF key < root.key THEN
    root.left ← bst_delete(root.left, key)
  ELSE IF key > root.key THEN
    root.right ← bst_delete(root.right, key)
  ELSE  // key == root.key: 삭제 대상 찾음
    // 케이스 1: 리프 노드
    IF root.left is NULL AND root.right is NULL THEN
      free(root)
      RETURN NULL
    END IF

    // 케이스 2: 자식이 하나
    IF root.left is NULL THEN
      temp ← root.right
      free(root)
      RETURN temp
    END IF
    IF root.right is NULL THEN
      temp ← root.left
      free(root)
      RETURN temp
    END IF

    // 케이스 3: 자식이 둘 다
    // 오른쪽 서브트리의 최솟값(후계자)을 찾음
    successor ← bst_find_min(root.right)
    root.key ← successor.key
    root.right ← bst_delete(root.right, successor.key)
  END IF

  RETURN root
*/

/**
 * 키 삭제
 * 케이스 1: 리프 노드 → 그냥 삭제
 * 케이스 2: 자식 1개  → 자식으로 대체
 * 케이스 3: 자식 2개  → 후계자(오른쪽 최솟값)로 대체
 */
BSTNode* bst_delete(BSTNode *root, int key) {
    if (root == NULL) {
        return NULL;
    }

    // 삭제할 노드 찾기
    if (key < root->key) {
        root->left = bst_delete(root->left, key);
    }
    else if (key > root->key) {
        root->right = bst_delete(root->right, key);
    }
    else {
        // 삭제 대상 찾음 (key == root->key)

        // 케이스 1: 리프 노드
        if (root->left == NULL && root->right == NULL) {
            free(root);
            return NULL;
        }

        // 케이스 2: 자식이 하나
        if (root->left == NULL) {
            BSTNode *temp = root->right;
            free(root);
            return temp;
        }
        if (root->right == NULL) {
            BSTNode *temp = root->left;
            free(root);
            return temp;
        }

        // 케이스 3: 자식이 둘 다
        // 오른쪽 서브트리의 최솟값(후계자) 찾기
        BSTNode *successor = bst_find_min(root->right);
        root->key = successor->key;
        // 후계자를 삭제 (중복 없으므로 안전)
        root->right = bst_delete(root->right, successor->key);
    }

    return root;
}


// ==================== 유틸리티 ====================

/**
 * 중위 순회 (정렬된 출력)
 */
void bst_inorder(BSTNode *root) {
    if (root != NULL) {
        bst_inorder(root->left);
        printf("%d ", root->key);
        bst_inorder(root->right);
    }
}

/**
 * 노드 개수
 */
int bst_get_count(BSTNode *root) {
    if (root == NULL) return 0;
    return 1 + bst_get_count(root->left) + bst_get_count(root->right);
}

/**
 * 트리 높이
 */
int bst_get_height(BSTNode *root) {
    if (root == NULL) return 0;
    int left_h = bst_get_height(root->left);
    int right_h = bst_get_height(root->right);
    return 1 + (left_h > right_h ? left_h : right_h);
}

/**
 * 메모리 해제
 */
void bst_destroy(BSTNode *root) {
    if (root != NULL) {
        bst_destroy(root->left);
        bst_destroy(root->right);
        free(root);
    }
}


// ==================== 테스트 코드 ====================

int main(void) {
    printf("========== 이진 탐색 트리 (Binary Search Tree) ==========\n\n");

    BSTNode *root = NULL;

    // ========== 삽입 테스트 ==========
    printf("========== 삽입 테스트 ==========\n");
    int keys[] = {50, 30, 70, 20, 40, 60, 80};
    int n = sizeof(keys) / sizeof(keys[0]);

    for (int i = 0; i < n; i++) {
        root = bst_insert(root, keys[i]);
        printf("삽입: %d\t→ 중위 순회: ", keys[i]);
        bst_inorder(root);
        printf("\n");
    }
    printf("\n");

    // ========== 탐색 테스트 ==========
    printf("========== 탐색 테스트 ==========\n");
    int search_keys[] = {40, 25, 70, 100};
    for (int i = 0; i < 4; i++) {
        BSTNode *found = bst_search(root, search_keys[i]);
        if (found != NULL) {
            printf("탐색 %d: 찾음!\n", search_keys[i]);
        } else {
            printf("탐색 %d: 없음\n", search_keys[i]);
        }
    }
    printf("\n");

    // ========== 최솟값/최댓값 테스트 ==========
    printf("========== 최솟값/최댓값 ==========\n");
    BSTNode *min_node = bst_find_min(root);
    BSTNode *max_node = bst_find_max(root);
    printf("최솟값: %d\n", min_node ? min_node->key : -1);
    printf("최댓값: %d\n\n", max_node ? max_node->key : -1);

    // ========== 삭제 테스트: 리프 노드 ==========
    printf("========== 삭제 테스트 1: 리프 노드 (20) ==========\n");
    printf("삭제 전: ");
    bst_inorder(root);
    printf("\n");
    root = bst_delete(root, 20);
    printf("삭제 후: ");
    bst_inorder(root);
    printf("\n\n");

    // ========== 삭제 테스트: 자식 1개 ==========
    printf("========== 삭제 테스트 2: 자식 1개 (30) ==========\n");
    printf("삭제 전: ");
    bst_inorder(root);
    printf("\n");
    root = bst_delete(root, 30);
    printf("삭제 후: ");
    bst_inorder(root);
    printf("\n\n");

    // ========== 삭제 테스트: 자식 2개 ==========
    printf("========== 삭제 테스트 3: 자식 2개 (50, 루트) ==========\n");
    printf("삭제 전: ");
    bst_inorder(root);
    printf("\n");
    root = bst_delete(root, 50);
    printf("삭제 후: ");
    bst_inorder(root);
    printf("\n\n");

    // ========== 트리 정보 ==========
    printf("========== 트리 정보 ==========\n");
    printf("노드 개수: %d\n", bst_get_count(root));
    printf("트리 높이: %d\n", bst_get_height(root));

    // 메모리 정리
    bst_destroy(root);

    return 0;
}
