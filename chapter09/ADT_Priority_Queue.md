# 우선순위 큐 (Priority Queue) ADT

## 객체
0개 이상의 요소들로 구성된 집합으로, 각 요소는 우선순위(priority)를 가진다

## 연산

| 연산 | 설명 |
|------|------|
| `create(max_size)` | 최대 크기가 `max_size`인 공백 우선순위 큐를 생성한다 |
| `init(q)` | 우선순위 큐를 초기화한다 |
| `is_empty(q)` | `size == 0` 이면 `TRUE`, 아니면 `FALSE` 반환 |
| `is_full(q)` | `size == max_size` 이면 `TRUE`, 아니면 `FALSE` 반환 |
| `insert(q, x)` | 큐가 가득 차면 `pq_full` 오류, 아니면 우선순위 큐에 요소 `x`를 삽입한다 |
| `delete(q)` | 큐가 비어 있으면 `pq_empty` 오류, 아니면 **최대 우선순위** 요소를 제거하여 반환한다 |
| `find(q)` | 큐가 비어 있으면 `pq_empty` 오류, 아니면 **최대 우선순위** 요소를 읽어서 반환한다 (삭제 없음) |

> **참고**: 최대 우선순위(Max Priority Queue)는 우선순위 값이 가장 큰 요소를 반환하고,<br>
> 최소 우선순위(Min Priority Queue)는 우선순위 값이 가장 작은 요소를 반환한다.

### 의사코드

```
is_empty(q) ::=
    if (size == 0) return TRUE
    else return FALSE

is_full(q) ::=
    if (size == max_size) return TRUE
    else return FALSE

insert(q, x) ::=
    if (is_full(q)) pq_full 오류
    else 우선순위 큐에 x를 적절한 위치에 삽입한다

delete(q) ::=
    if (is_empty(q)) pq_empty 오류
    else 우선순위가 가장 높은 요소를 제거하여 반환한다

find(q) ::=
    if (is_empty(q)) pq_empty 오류
    else 우선순위가 가장 높은 요소를 읽어서 반환한다
```

### 시간 복잡도 비교

| 구현 방식 | insert | delete | find |
|----------|--------|--------|------|
| 정렬되지 않은 배열 | O(1) | O(n) | O(n) |
| 정렬된 배열 | O(n) | O(1) | O(1) |
| **힙 (Heap)** | **O(log n)** | **O(log n)** | **O(1)** |
