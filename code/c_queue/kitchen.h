#ifndef KITCHEN_H
#define KITCHEN_H

#include "types.h"
#include "queue.h"
#include "kitchen.h"

// c : 새로 들어온 손님
void Kitchen_Enqueue_FIFO(Queue* kitchen_queue, Customer* c) // 시나리오1[선입선출] 함수
{
    Enqueue(kitchen_queue, c);
}
==================================================================================
void Kitchen_Enqueue_Priority(Queue* kitchen_queue, Customer* c) // 시나리오2[최단 작업 우선] 함수
{
    Node* newNode = (Node*)malloc(sizeof(Node));

    newNode->data = c;
    newNode->next = NULL;

    // 큐가 비어있는 경우
    if (kitchen_queue->front == NULL)
    {
        kitchen_queue->front = newNode;
        kitchen_queue->rear = newNode;
        return;
    }

    // 맨 앞에 들어가야 하는 경우
    if (c->remaining_time <
        kitchen_queue->front->data->remaining_time)
    {
        newNode->next = kitchen_queue->front;
        kitchen_queue->front = newNode;
        return;
    }

    Node* current = kitchen_queue->front;

    while (current->next != NULL &&
           current->next->data->remaining_time <=
           c->remaining_time)
    {
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;

    // 마지막에 삽입된 경우 rear 갱신
    if (newNode->next == NULL)
    {
        kitchen_queue->rear = newNode;
    }
}
==================================================================================
Customer* Kitchen_Process_Tick(Queue* kitchen_queue) // 주방 타이머 진행/완료 처리 함수
{
    if (kitchen_queue->front == NULL)
    {
        return NULL;
    }

    Customer* current =
        kitchen_queue->front->data;

    current->remaining_time--;

    if (current->remaining_time <= 0)
    {
        return Dequeue(kitchen_queue);
    }

    return NULL;
}

#endif
