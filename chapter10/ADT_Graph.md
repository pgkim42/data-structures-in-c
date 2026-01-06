# 그래프 (Graph) ADT

## 객체

정점(Vertex)의 집합과 간선(Edge)의 집합으로 구성된 비선형 자료구조

- **정점(V)**: 데이터를 저장하는 노드
- **간선(E)**: 정점 간의 관계를 표현하는 연결선

## 그래프의 종류

| 구분 | 설명 |
|------|------|
| **무방향 그래프** | 간선에 방향이 없음 `(u, v) = (v, u)` |
| **방향 그래프** | 간선에 방향이 있음 `<u, v> ≠ <v, u>` |
| **가중치 그래프** | 간선에 비용( weight)이 부여됨 |

## 연산

| 연산 | 설명 |
|------|------|
| `create_graph()` | 공백 그래프를 생성한다 |
| `init(g)` | 그래프를 초기화한다 |
| `is_empty(g)` | 그래프에 정점이 없으면 `TRUE`, 아니면 `FALSE` 반환 |
| `insert_vertex(g, v)` | 그래프에 정점 `v`를 삽입한다 |
| `insert_edge(g, u, v)` | 정점 `u`와 `v` 사이에 간선을 삽입한다 |
| `delete_vertex(g, v)` | 정점 `v`와 연결된 모든 간선을 삭제한 후 정점 `v`를 삭제한다 |
| `delete_edge(g, u, v)` | 정점 `u`와 `v` 사이의 간선을 삭제한다 |
| `adjacent(g, v)` | 정점 `v`에 인접한 모든 정점들을 반환한다 |
| `destroy_graph(g)` | 그래프의 모든 메모리를 해제한다 |

## 의사코드

```
is_empty(g) ::=
    if (g의 정점 수 == 0) return TRUE
    else return FALSE

insert_vertex(g, v) ::=
    if (v가 이미 존재) 오류
    else g에 정점 v를 추가한다

insert_edge(g, u, v) ::=
    if (간선 (u,v)가 이미 존재) 오류
    else if (u 또는 v가 존재하지 않음) 오류
    else 정점 u와 v 사이에 간선을 추가한다

delete_vertex(g, v) ::=
    if (v가 존재하지 않음) 오류
    else
        v에 인접한 모든 정점들에 대해 간선을 삭제한다
        g에서 정점 v를 삭제한다

delete_edge(g, u, v) ::=
    if (간선 (u,v)가 존재하지 않음) 오류
    else 정점 u와 v 사이의 간선을 삭제한다

adjacent(g, v) ::=
    if (v가 존재하지 않음) 오류
    else 정점 v에 직접 연결된 모든 정점들의 집합을 반환한다
```
