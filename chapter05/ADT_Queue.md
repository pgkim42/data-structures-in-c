# 큐 (Queue) ADT

## 객체
0개 이상의 요소들로 구성된 선형 리스트

## 연산

| 연산 | 설명 |
|------|------|
| `create(max_size)` | 최대 크기가 `max_size`인 공백 큐를 생성한다 |
| `init(q)` | 큐를 초기화한다 |
| `is_empty(q)` | `size == 0` 이면 `TRUE`, 아니면 `FALSE` 반환 |
| `is_full(q)` | `size == max_size` 이면 `TRUE`, 아니면 `FALSE` 반환 |
| `enqueue(q, e)` | 큐가 가득 차면 `queue_full` 오류, 아니면 큐의 후단(rear)에 원소 `e`를 삽입한다 |
| `dequeue(q)` | 큐가 비어 있으면 `queue_empty` 오류, 아니면 전단(front) 원소를 제거하여 반환한다 |
| `peek(q)` | 큐가 비어 있으면 `queue_empty` 오류, 아니면 전단 원소를 읽어서 반환한다 (삭제 없음) |

### 의사코드

```
is_empty(q) ::=
    if (size == 0) return TRUE
    else return FALSE

is_full(q) ::=
    if (size == max_size) return TRUE
    else return FALSE

enqueue(q, e) ::=
    if (is_full(q)) queue_full 오류
    else q의 후단에 e를 삽입한다

dequeue(q) ::=
    if (is_empty(q)) queue_empty 오류
    else q의 전단 원소를 제거하여 반환한다

peek(q) ::=
    if (is_empty(q)) queue_empty 오류
    else q의 전단 원소를 읽어서 반환한다
```