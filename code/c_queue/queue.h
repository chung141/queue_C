#ifndef QUEUE_H
#define QUEUE_H

#include "types.h"
#include <stdbool.h> // true, false 사용

// 큐 초기화 함수
void InitQueue(Queue* q);

// 큐가 비어있는지 확인
bool IsEmpty(Queue* q);

// 큐에 손님 데이터 넣기 (맨 뒤에 삽입)
void Enqueue(Queue* q, Customer* c);

// 큐에서 손님 데이터 빼기 (맨 앞에서 추출)
Customer* Dequeue(Queue* q);

#endif