# 덱 (Deque) ADT

## 객체
0개 이상의 요소들로 구성된 선형 리스트 (양쪽 끝에서 삽입/삭제 가능)

## 연산

| 연산 | 설명 |
|------|------|
| `create(max_size)` | 최대 크기가 `max_size`인 공백 덱을 생성한다 |
| `init(dq)` | 덱을 초기화한다 |
| `is_empty(dq)` | `size == 0` 이면 `TRUE`, 아니면 `FALSE` 반환 |
| `is_full(dq)` | `size == max_size` 이면 `TRUE`, 아니면 `FALSE` 반환 |
| `add_front(dq, e)` | 덱이 가득 차면 `deque_full` 오류, 아니면 전단(front)에 원소 `e`를 삽입한다 |
| `add_rear(dq, e)` | 덱이 가득 차면 `deque_full` 오류, 아니면 후단(rear)에 원소 `e`를 삽입한다 |
| `delete_front(dq)` | 덱이 비어 있으면 `deque_empty` 오류, 아니면 전단 원소를 제거하여 반환한다 |
| `delete_rear(dq)` | 덱이 비어 있으면 `deque_empty` 오류, 아니면 후단 원소를 제거하여 반환한다 |
| `get_front(dq)` | 덱이 비어 있으면 `deque_empty` 오류, 아니면 전단 원소를 읽어서 반환한다 (삭제 없음) |
| `get_rear(dq)` | 덱이 비어 있으면 `deque_empty` 오류, 아니면 후단 원소를 읽어서 반환한다 (삭제 없음) |

### 의사코드

```
is_empty(dq) ::=
    if (size == 0) return TRUE
    else return FALSE

is_full(dq) ::=
    if (size == max_size) return TRUE
    else return FALSE

add_front(dq, e) ::=
    if (is_full(dq)) deque_full 오류
    else dq의 전단에 e를 삽입한다

add_rear(dq, e) ::=
    if (is_full(dq)) deque_full 오류
    else dq의 후단에 e를 삽입한다

delete_front(dq) ::=
    if (is_empty(dq)) deque_empty 오류
    else dq의 전단 원소를 제거하여 반환한다

delete_rear(dq) ::=
    if (is_empty(dq)) deque_empty 오류
    else dq의 후단 원소를 제거하여 반환한다

get_front(dq) ::=
    if (is_empty(dq)) deque_empty 오류
    else dq의 전단 원소를 읽어서 반환한다

get_rear(dq) ::=
    if (is_empty(dq)) deque_empty 오류
    else dq의 후단 원소를 읽어서 반환한다
```