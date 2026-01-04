/**
 * Chapter 08: 이진 트리 (Binary Tree)
 * 포인터 기반 구현
 *
 * 이진 트리: 각 노드가 최대 2개의 자식(왼쪽, 오른쪽)을 가지는 트리
 *
 * 시간 복잡도:
 * - 노드 생성/삭제: O(1)
 * - 순회 (preorder, inorder, postorder): O(n)
 * - 노드 탐색: O(n) 최악 (편향 트리), O(log n) 평균 (균형 트리)
 */

#include <stdio.h>
#include <stdlib.h>

// ==================== TreeNode 구조체 정의 ====================

typedef struct TreeNode {
    int data;                       // 노드에 저장된 데이터
    struct TreeNode *left;          // 왼쪽 자식 노드 포인터
    struct TreeNode *right;         // 오른쪽 자식 노드 포인터
} TreeNode;


// ==================== 기본 연산 ====================

/**
 * 새로운 노드 생성
 * @param data 노드에 저장할 데이터
 * @return 생성된 노드의 포인터
 */
TreeNode* create_node(int data) {
    TreeNode *node = (TreeNode *)malloc(sizeof(TreeNode));
    if (node == NULL) {
        fprintf(stderr, "메모리 할당 실패\n");
        exit(1);
    }
    node->data = data;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/**
 * 트리가 비어있는지 확인
 * @param root 루트 노드
 * @return 비어있으면 1, 아니면 0
 */
int is_empty(TreeNode *root) {
    return root == NULL;
}

/**
 * 왼쪽 자식 노드 설정
 * @param parent 부모 노드
 * @param child 왼쪽 자식으로 설정할 노드
 */
void set_left(TreeNode *parent, TreeNode *child) {
    if (parent == NULL) {
        fprintf(stderr, "부모 노드가 NULL입니다\n");
        return;
    }
    parent->left = child;
}

/**
 * 오른쪽 자식 노드 설정
 * @param parent 부모 노드
 * @param child 오른쪽 자식으로 설정할 노드
 */
void set_right(TreeNode *parent, TreeNode *child) {
    if (parent == NULL) {
        fprintf(stderr, "부모 노드가 NULL입니다\n");
        return;
    }
    parent->right = child;
}

/**
 * 왼쪽 자식 노드 반환
 * @param node 부모 노드
 * @return 왼쪽 자식 노드 포인터
 */
TreeNode* get_left(TreeNode *node) {
    return node != NULL ? node->left : NULL;
}

/**
 * 오른쪽 자식 노드 반환
 * @param node 부모 노드
 * @return 오른쪽 자식 노드 포인터
 */
TreeNode* get_right(TreeNode *node) {
    return node != NULL ? node->right : NULL;
}

/**
 * 노드의 데이터 반환
 * @param node 노드
 * @return 노드의 데이터
 */
int get_data(TreeNode *node) {
    if (node == NULL) {
        fprintf(stderr, "노드가 NULL입니다\n");
        return -1;
    }
    return node->data;
}


// ==================== 트리 순회 (Tree Traversal) ====================

/**
 * 전위 순회 (Preorder Traversal)
 * 순서: 루트 → 왼쪽 서브트리 → 오른쪽 서브트리
 * 용도: 트리 복사, 전위 표기식 생성
 */
void preorder(TreeNode *root) {
    if (root != NULL) {
        printf("%d ", root->data);      // 루트 방문
        preorder(root->left);            // 왼쪽 서브트리
        preorder(root->right);           // 오른쪽 서브트리
    }
}

/**
 * 중위 순회 (Inorder Traversal)
 * 순서: 왼쪽 서브트리 → 루트 → 오른쪽 서브트리
 * 용도: 오름차순 정렬 (BST), 중위 표기식 생성
 */
void inorder(TreeNode *root) {
    if (root != NULL) {
        inorder(root->left);             // 왼쪽 서브트리
        printf("%d ", root->data);       // 루트 방문
        inorder(root->right);            // 오른쪽 서브트리
    }
}

/**
 * 후위 순회 (Postorder Traversal)
 * 순서: 왼쪽 서브트리 → 오른쪽 서브트리 → 루트
 * 용도: 트리 삭제, 후위 표기식 생성, 메모리 해제
 */
void postorder(TreeNode *root) {
    if (root != NULL) {
        postorder(root->left);           // 왼쪽 서브트리
        postorder(root->right);          // 오른쪽 서브트리
        printf("%d ", root->data);       // 루트 방문
    }
}


// ==================== 반복적 순회 (Iterative Traversal) ====================

/**
 * 반복적 전위 순회 (Preorder Traversal - Iterative)
 * 스택을 직접 사용하여 재귀 없이 구현
 * 순서: 루트 → 왼쪽 서브트리 → 오른쪽 서브트리
 *
 * 알고리즘:
 * 1. 루트를 스택에 push
 * 2. 스택이 빌 때까지 반복:
 *    - 노드를 pop하여 방문
 *    - 오른쪽 자식을 push (먼저 push하면 나중에 방문)
 *    - 왼쪽 자식을 push (나중에 push하면 먼저 방문)
 */
void preorder_iterative(TreeNode *root) {
    if (root == NULL) {
        return;
    }

    // 스택 정의 (최대 크기: 노드 개수)
    #define MAX_STACK 100
    TreeNode *stack[MAX_STACK];
    int top = -1;

    // 루트를 스택에 push
    stack[++top] = root;

    while (top >= 0) {
        // 스택에서 pop하여 방문
        TreeNode *current = stack[top--];
        printf("%d ", current->data);

        // 오른쪽 자식 먼저 push (LIFO이므로 나중에 방문됨)
        if (current->right != NULL) {
            stack[++top] = current->right;
        }

        // 왼쪽 자식을 push (먼저 방문됨)
        if (current->left != NULL) {
            stack[++top] = current->left;
        }
    }
}


// ==================== 레벨 순회 (Level-order Traversal) ====================

/**
 * 레벨 순회 (Level-order Traversal / BFS)
 * 큐를 사용하여 같은 레벨의 노드들을 순서대로 방문
 *
 *       1          Level-order: 1 → 2 → 3 → 4 → 5
 *      / \
 *     2   3
 *    / \
 *   4   5
 *
 * 알고리즘:
 * 1. 루트를 큐에 enqueue
 * 2. 큐가 빌 때까지 반복:
 *    - 노드를 dequeue하여 방문
 *    - 왼쪽 자식을 enqueue
 *    - 오른쪽 자식을 enqueue
 */
void level_order(TreeNode *root) {
    if (root == NULL) {
        return;
    }

    // 큐 정의 (최대 크기: 노드 개수)
    #define MAX_QUEUE 100
    TreeNode *queue[MAX_QUEUE];
    int front = 0;
    int rear = 0;

    // 루트를 큐에 enqueue
    queue[rear++] = root;

    while (front < rear) {
        // 큐에서 dequeue하여 방문
        TreeNode *current = queue[front++];
        printf("%d ", current->data);

        // 왼쪽 자식을 enqueue
        if (current->left != NULL) {
            queue[rear++] = current->left;
        }

        // 오른쪽 자식을 enqueue
        if (current->right != NULL) {
            queue[rear++] = current->right;
        }
    }
}


// ==================== 기타 유틸리티 ====================

/**
 * 트리의 노드 개수 계산
 * @param root 루트 노드
 * @return 노드 개수
 */
int get_node_count(TreeNode *root) {
    if (root == NULL) {
        return 0;
    }
    return 1 + get_node_count(root->left) + get_node_count(root->right);
}

/**
 * 트리의 높이 계산
 * @param root 루트 노드
 * @return 트리의 높이 (높이가 0인 리프 노드 기준)
 */
int get_height(TreeNode *root) {
    if (root == NULL) {
        return 0;
    }
    int left_height = get_height(root->left);
    int right_height = get_height(root->right);
    return 1 + (left_height > right_height ? left_height : right_height);
}

/**
 * 리프 노드(자식이 없는 노드) 개수 계산
 * @param root 루트 노드
 * @return 리프 노드 개수
 */
int get_leaf_count(TreeNode *root) {
    if (root == NULL) {
        return 0;
    }
    if (root->left == NULL && root->right == NULL) {
        return 1;  // 리프 노드
    }
    return get_leaf_count(root->left) + get_leaf_count(root->right);
}

/**
 * 트리 메모리 해제 (후위 순회 방식)
 * @param root 루트 노드
 */
void destroy_tree(TreeNode *root) {
    if (root != NULL) {
        destroy_tree(root->left);        // 왼쪽 서브트리 해제
        destroy_tree(root->right);       // 오른쪽 서브트리 해제
        free(root);                      // 루트 해제
    }
}


// ==================== 트리 복사 및 비교 ====================

/**
 * 트리 복사 (Copy Tree)
 * 전위 순회 방식으로 트리를 복사
 * @param root 복사할 트리의 루트
 * @return 복사된 트리의 루트
 */
TreeNode* copy_tree(TreeNode *root) {
    if (root == NULL) {
        return NULL;
    }

    // 새 노드 생성
    TreeNode *new_node = create_node(root->data);

    // 왼쪽 서브트리 복사
    new_node->left = copy_tree(root->left);

    // 오른쪽 서브트리 복사
    new_node->right = copy_tree(root->right);

    return new_node;
}

/**
 * 트리 동일성 검사 (Tree Equality)
 * 두 트리의 구조와 데이터가 모두 같은지 확인
 * @param t1 첫 번째 트리의 루트
 * @param t2 두 번째 트리의 루트
 * @return 같으면 1, 다르면 0
 */
int is_equal(TreeNode *t1, TreeNode *t2) {
    // 둘 다 NULL이면 같음
    if (t1 == NULL && t2 == NULL) {
        return 1;
    }

    // 하나만 NULL이면 다름
    if (t1 == NULL || t2 == NULL) {
        return 0;
    }

    // 데이터가 다르면 다름
    if (t1->data != t2->data) {
        return 0;
    }

    // 왼쪽 서브트리와 오른쪽 서브트리가 모두 같아야 같음
    return is_equal(t1->left, t2->left) && is_equal(t1->right, t2->right);
}


// ==================== 테스트 코드 ====================

int main(void) {
    printf("========== 이진 트리 (Binary Tree) ==========\n\n");

    //       1
    //      / \
    //     2   3
    //    / \
    //   4   5

    // 노드 생성
    TreeNode *n1 = create_node(1);
    TreeNode *n2 = create_node(2);
    TreeNode *n3 = create_node(3);
    TreeNode *n4 = create_node(4);
    TreeNode *n5 = create_node(5);

    // 트리 구성
    set_left(n1, n2);    // 1의 왼쪽 자식 = 2
    set_right(n1, n3);   // 1의 오른쪽 자식 = 3
    set_left(n2, n4);    // 2의 왼쪽 자식 = 4
    set_right(n2, n5);   // 2의 오른쪽 자식 = 5

    TreeNode *root = n1;  // 루트 노드 설정

    // 기본 연산 테스트
    printf("기본 정보:\n");
    printf("  루트 데이터: %d\n", get_data(root));
    printf("  루트의 왼쪽 자식: %d\n", get_data(get_left(root)));
    printf("  루트의 오른쪽 자식: %d\n", get_data(get_right(root)));
    printf("  트리가 비어있나? %s\n", is_empty(root) ? "Yes" : "No");
    printf("  노드 개수: %d\n", get_node_count(root));
    printf("  트리 높이: %d\n", get_height(root));
    printf("  리프 노드 개수: %d\n\n", get_leaf_count(root));

    // 트리 순회 테스트
    printf("트리 순회 결과:\n");
    printf("  전위 순회 (Preorder): ");
    preorder(root);
    printf("\n");

    printf("  중위 순회 (Inorder):  ");
    inorder(root);
    printf("\n");

    printf("  후위 순회 (Postorder):");
    postorder(root);
    printf("\n\n");

    // ========== 반복적 순회 테스트 ==========
    printf("========== 반복적 순회 테스트 ==========\n\n");
    printf("  반복적 전위 순회: ");
    preorder_iterative(root);
    printf("\n\n");

    // ========== 레벨 순회 테스트 ==========
    printf("========== 레벨 순회 테스트 ==========\n\n");
    printf("  레벨 순회 (Level-order): ");
    level_order(root);
    printf("\n\n");

    // ========== 트리 복사 테스트 ==========
    printf("========== 트리 복사 테스트 ==========\n\n");
    TreeNode *copied = copy_tree(root);
    printf("  원본 트리 전위 순회: ");
    preorder(root);
    printf("\n");
    printf("  복사 트리 전위 순회: ");
    preorder(copied);
    printf("\n");
    printf("  두 트리가 같은가? %s\n\n", is_equal(root, copied) ? "Yes" : "No");

    // ========== 트리 동일성 검사 테스트 ==========
    printf("========== 트리 동일성 검사 테스트 ==========\n\n");

    // 같은 구조의 트리
    TreeNode *same_tree = create_node(1);
    set_left(same_tree, create_node(2));
    set_right(same_tree, create_node(3));
    set_left(get_left(same_tree), create_node(4));
    set_right(get_left(same_tree), create_node(5));

    printf("  같은 구조의 트리와 비교: %s\n", is_equal(root, same_tree) ? "Same" : "Different");

    // 다른 구조의 트리
    TreeNode *diff_tree = create_node(1);
    set_left(diff_tree, create_node(2));
    set_right(diff_tree, create_node(3));
    set_left(get_left(diff_tree), create_node(4));
    // 5가 없음

    printf("  다른 구조의 트리와 비교: %s\n", is_equal(root, diff_tree) ? "Same" : "Different");

    // 다른 데이터의 트리
    TreeNode *data_diff_tree = create_node(1);
    set_left(data_diff_tree, create_node(2));
    set_right(data_diff_tree, create_node(3));
    set_left(get_left(data_diff_tree), create_node(4));
    set_right(get_left(data_diff_tree), create_node(99));  // 데이터가 다름

    printf("  다른 데이터의 트리와 비교: %s\n\n", is_equal(root, data_diff_tree) ? "Same" : "Different");

    // 더 복잡한 트리 테스트
    //       15
    //      /  \
    //     4    20
    //    /      \
    //   1       25
    //  / \
    // 2   3

    printf("========== 추가 테스트: 비대칭 트리 ==========\n\n");

    TreeNode *root2 = create_node(15);
    set_left(root2, create_node(4));
    set_right(root2, create_node(20));
    set_left(get_left(root2), create_node(1));
    set_right(get_right(root2), create_node(25));
    set_left(get_left(get_left(root2)), create_node(2));
    set_right(get_left(get_left(root2)), create_node(3));

    printf("트리 정보:\n");
    printf("  노드 개수: %d\n", get_node_count(root2));
    printf("  트리 높이: %d\n", get_height(root2));
    printf("  리프 노드 개수: %d\n\n", get_leaf_count(root2));

    printf("트리 순회 결과:\n");
    printf("  전위 순회: ");
    preorder(root2);
    printf("\n");
    printf("  중위 순회: ");
    inorder(root2);
    printf("\n");
    printf("  후위 순회: ");
    postorder(root2);
    printf("\n");

    // 메모리 정리
    destroy_tree(root);
    destroy_tree(copied);
    destroy_tree(same_tree);
    destroy_tree(diff_tree);
    destroy_tree(data_diff_tree);
    destroy_tree(root2);

    return 0;
}
