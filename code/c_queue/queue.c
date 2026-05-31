#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

void InitQueue(Queue* q) {
    q->front = NULL;
    q->rear = NULL;
    q->count = 0;
}

bool IsEmpty(Queue* q) {
    return q->count == 0;
}

void Enqueue(Queue* q, Customer* c) {
    c->next = NULL; // 새로 들어온 손님의 뒤에는 아직 아무도 없음

    if (IsEmpty(q)) {
        // 큐가 비어있을 땐 맨 앞(front)과 맨 뒤(rear)가 모두 이 손님
        q->front = c;
        q->rear = c;
    } else {
        // 기존 맨 뒤 손님의 다음(next)을 새 손님으로 연결하고, rear를 갱신
        q->rear->next = c;
        q->rear = c;
    }
    q->count++;
}

Customer* Dequeue(Queue* q) {
    if (IsEmpty(q)) {
        return NULL; // 뺄 사람이 없으면 NULL 반환
    }

    // 1. 맨 앞의 손님 데이터를 임시 보관
    Customer* target = q->front;
    
    // 2. 맨 앞(front)을 그 다음 사람으로 변경
    q->front = q->front->next;
    
    // 3. 만약 뺀 사람이 마지막 사람이었다면 rear도 NULL로 초기화
    if (q->front == NULL) {
        q->rear = NULL;
    }
    
    q->count--;
    
    // 4. 빼낸 손님 데이터를 다음 단계로 넘겨줌 (절대 여기서 free 하면 안 됨!)
    return target;
}