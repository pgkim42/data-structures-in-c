/**
 * Chapter 08: 수식 트리 (Expression Tree)
 *
 * 수식 트리는 산술 표현식을 트리 형태로 표현한 것입니다.
 *
 *          +           (3 + 4) * (5 - 2) = 21
 *        /   \
 *       *     -
 *      / \   / \
 *     3   4 5   2
 *
 * 구조:
 * - 내부 노드: 연산자 (+, -, *, /)
 * - 리프 노드: 피연산자 (정수)
 *
 * ============================================================================
 * [의사코드 1] 후위 표기식으로부터 수식 트리 생성
 * ============================================================================
 *
 * ALGORITHM make_expression_tree(postfix)
 * INPUT: 후위 표기식 문자열 (예: "3 4 + 5 2 - *")
 * OUTPUT: 수식 트리의 루트 노드
 *
 *  stack ← empty stack                    // 노드를 저장할 스택
 *  tokens ← split(postfix)               // 토큰별 분리
 *
 *  FOR EACH token IN tokens DO
 *    IF token is operand (숫자) THEN
 *      node ← create_node(token)         // 피연산자 노드 생성
 *      push(stack, node)                 // 스택에 push
 *    ELSE IF token is operator (+,-,*,/) THEN
 *      node ← create_node(token)         // 연산자 노드 생성
 *      node.right ← pop(stack)           // 오른쪽 피연산자
 *      node.left ← pop(stack)            // 왼쪽 피연산자
 *      push(stack, node)                 // 결과 노드를 스택에 push
 *    END IF
 *  END FOR
 *
 *  RETURN pop(stack)                     // 스택의 마지막 노드가 루트
 *
 * ============================================================================
 * [의사코드 2] 수식 트리 계산 (후위 순회)
 * ============================================================================
 *
 * ALGORITHM evaluate(tree)
 * INPUT: 수식 트리의 루트 노드
 * OUTPUT: 계산 결과 (정수)
 *
 *  IF tree is NULL THEN
 *    RETURN 0
 *  END IF
 *
 *  IF tree is leaf node (리프 노드) THEN
 *    RETURN tree.data                    // 피연산자 반환
 *  END IF
 *
 *  // 내부 노드인 경우 재귀적으로 계산
 *  left_val ← evaluate(tree.left)        // 왼쪽 서브트리 계산
 *  right_val ← evaluate(tree.right)      // 오른쪽 서브트리 계산
 *
 *  // 연산자에 따른 연산 수행
 *  SWITCH tree.data DO
 *    CASE '+': RETURN left_val + right_val
 *    CASE '-': RETURN left_val - right_val
 *    CASE '*': RETURN left_val * right_val
 *    CASE '/': RETURN left_val / right_val
 *  END SWITCH
 *
 * ============================================================================
 * [의사코드 3] 수식 트리를 다양한 표기법으로 출력
 * ============================================================================
 *
 * ALGORITHM print_infix(tree)           // 중위 표기식 (3 + 4) * (5 - 2)
 *   IF tree is not NULL THEN
 *     IF tree is internal node THEN PRINT "("
 *     print_infix(tree.left)
 *     PRINT tree.data
 *     print_infix(tree.right)
 *     IF tree is internal node THEN PRINT ")"
 *   END IF
 *
 * ALGORITHM print_prefix(tree)           // 전위 표기식 * + 3 4 - 5 2
 *   IF tree is not NULL THEN
 *     PRINT tree.data
 *     print_prefix(tree.left)
 *     print_prefix(tree.right)
 *   END IF
 *
 * ALGORITHM print_postfix(tree)          // 후위 표기식 3 4 + 5 2 - *
 *   IF tree is not NULL THEN
 *     print_postfix(tree.left)
 *     print_postfix(tree.right)
 *     PRINT tree.data
 *   END IF
 *
 * ============================================================================
 * [의사코드 4] 수식 트리 메모리 해제
 * ============================================================================
 *
 * ALGORITHM destroy_expression_tree(tree)
 *   IF tree is not NULL THEN
 *     destroy_expression_tree(tree.left)
 *     destroy_expression_tree(tree.right)
 *     free(tree)
 *   END IF
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// ==================== 노드 구조체 정의 ====================

typedef struct ExprNode {
    char op;                         // 연산자 또는 피연산자 (문자)
    int is_operator;                // 1=연산자, 0=피연산자
    struct ExprNode *left;
    struct ExprNode *right;
} ExprNode;


// ==================== 기본 연산 ====================

/**
 * 새로운 노드 생성
 * @param value 연산자 또는 피연산자 문자
 * @param is_op 연산자이면 1, 피연산자이면 0
 */
ExprNode* create_expr_node(char value, int is_op) {
    ExprNode *node = (ExprNode *)malloc(sizeof(ExprNode));
    if (node == NULL) {
        fprintf(stderr, "메모리 할당 실패\n");
        exit(1);
    }
    node->op = value;
    node->is_operator = is_op;
    node->left = NULL;
    node->right = NULL;
    return node;
}

/**
 * 노드가 리프(피연산자)인지 확인
 */
int is_leaf(ExprNode *node) {
    return node != NULL && !node->is_operator;
}


// ==================== 수식 트리 생성 (후위 표기식 → 트리) ====================

/**
 * 후위 표기식으로부터 수식 트리 생성
 * @param postfix 후위 표기식 문자열 (예: "34+52-*")
 * @return 생성된 트리의 루트 노드
 *
 * 예: "34+52-*" → (3 + 4) * (5 - 2)
 * 후위 표기식에서 공백 없는 연속된 숫자는 한 자리 숫자로 처리
 */
ExprNode* make_expression_tree(const char *postfix) {
    // 스택 정의
    #define MAX_STACK 100
    ExprNode *stack[MAX_STACK];
    int top = -1;

    for (int i = 0; postfix[i] != '\0'; i++) {
        char token = postfix[i];

        // 공백은 건너뜀
        if (token == ' ') {
            continue;
        }

        // 피연산자 (숫자)인 경우
        if (isdigit(token)) {
            ExprNode *node = create_expr_node(token, 0);  // 피연산자
            stack[++top] = node;
        }
        // 연산자인 경우
        else if (token == '+' || token == '-' || token == '*' || token == '/') {
            if (top < 1) {
                fprintf(stderr, "잘못된 수식: 피연산자 부족\n");
                exit(1);
            }

            ExprNode *node = create_expr_node(token, 1);  // 연산자

            // 오른쪽 피연산자 (먼저 pop)
            node->right = stack[top--];

            // 왼쪽 피연산자
            node->left = stack[top--];

            // 결과 노드를 스택에 push
            stack[++top] = node;
        }
    }

    // 스택에 남은 노드가 루트
    if (top != 0) {
        fprintf(stderr, "잘못된 수식\n");
        exit(1);
    }

    return stack[0];
}


// ==================== 수식 트리 계산 ====================

/**
 * 수식 트리 계산 (후위 순회)
 * @param root 루트 노드
 * @return 계산 결과
 */
int evaluate(ExprNode *root) {
    if (root == NULL) {
        return 0;
    }

    // 리프 노드(피연산자)이면 숫자 반환
    if (is_leaf(root)) {
        return root->op - '0';  // 문자 '0'~'9'를 정수 0~9로 변환
    }

    // 재귀적으로 왼쪽/오른쪽 서브트리 계산
    int left_val = evaluate(root->left);
    int right_val = evaluate(root->right);

    // 연산자에 따른 연산 수행
    switch (root->op) {
        case '+': return left_val + right_val;
        case '-': return left_val - right_val;
        case '*': return left_val * right_val;
        case '/': return left_val / right_val;
        default:
            fprintf(stderr, "알 수 없는 연산자: %c\n", root->op);
            return 0;
    }
}


// ==================== 다양한 표기법으로 출력 ====================

/**
 * 중위 표기식 출력 (Infix: (3 + 4) * (5 - 2))
 * 괄호를 포함하여 출력
 */
void print_infix(ExprNode *root) {
    if (root != NULL) {
        // 내부 노드(연산자)이면 괄호 시작
        if (root->is_operator) {
            printf("(");
        }

        print_infix(root->left);
        printf("%c", root->op);
        print_infix(root->right);

        // 내부 노드(연산자)이면 괄호 끝
        if (root->is_operator) {
            printf(")");
        }
    }
}

/**
 * 전위 표기식 출력 (Prefix: * + 3 4 - 5 2)
 */
void print_prefix(ExprNode *root) {
    if (root != NULL) {
        printf("%c ", root->op);
        print_prefix(root->left);
        print_prefix(root->right);
    }
}

/**
 * 후위 표기식 출력 (Postfix: 3 4 + 5 2 - *)
 */
void print_postfix(ExprNode *root) {
    if (root != NULL) {
        print_postfix(root->left);
        print_postfix(root->right);
        printf("%c ", root->op);
    }
}


// ==================== 메모리 해제 ====================

/**
 * 수식 트리 메모리 해제 (후위 순회)
 */
void destroy_expression_tree(ExprNode *root) {
    if (root != NULL) {
        destroy_expression_tree(root->left);
        destroy_expression_tree(root->right);
        free(root);
    }
}


// ==================== 테스트 코드 ====================

int main(void) {
    printf("========== 수식 트리 (Expression Tree) ==========\n\n");

    // 테스트 케이스 1: (3 + 4) * (5 - 2) = 21
    // 후위 표기식: 3 4 + 5 2 - *
    const char *expr1 = "34+52-*";

    printf("테스트 1: (3 + 4) * (5 - 2)\n");
    printf("  후위 표기식: %s\n", expr1);

    ExprNode *tree1 = make_expression_tree(expr1);

    printf("  중위 표기식: ");
    print_infix(tree1);
    printf("\n");

    printf("  전위 표기식: ");
    print_prefix(tree1);
    printf("\n");

    printf("  후위 표기식: ");
    print_postfix(tree1);
    printf("\n");

    printf("  계산 결과: %d\n\n", evaluate(tree1));

    destroy_expression_tree(tree1);

    // 테스트 케이스 2: 1 + 2 * 3 = 7
    // 후위 표기식: 1 2 3 * +
    const char *expr2 = "123*+";

    printf("테스트 2: 1 + 2 * 3\n");
    printf("  후위 표기식: %s\n", expr2);

    ExprNode *tree2 = make_expression_tree(expr2);

    printf("  중위 표기식: ");
    print_infix(tree2);
    printf("\n");

    printf("  전위 표기식: ");
    print_prefix(tree2);
    printf("\n");

    printf("  계산 결과: %d\n\n", evaluate(tree2));

    destroy_expression_tree(tree2);

    // 테스트 케이스 3: (8 - 3) * (2 + 4) / 2 = 15
    // 후위 표기식: 8 3 - 2 4 + * 2 /
    const char *expr3 = "83-24+*2/";

    printf("테스트 3: (8 - 3) * (2 + 4) / 2\n");
    printf("  후위 표기식: %s\n", expr3);

    ExprNode *tree3 = make_expression_tree(expr3);

    printf("  중위 표기식: ");
    print_infix(tree3);
    printf("\n");

    printf("  전위 표기식: ");
    print_prefix(tree3);
    printf("\n");

    printf("  계산 결과: %d\n", evaluate(tree3));

    destroy_expression_tree(tree3);

    return 0;
}
