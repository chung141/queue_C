
#include <stdio.h>
#include <stdlib.h>
#include "kitchen.h"

// ==========================================
// 시나리오 1: 선입선출 (FIFO)
// ==========================================
void Kitchen_Enqueue_FIFO(Queue* kitchen_queue, Customer* c) {
    Enqueue(kitchen_queue, c); // 우리가 만든 기본 큐 엔진 재사용
}

// ==========================================
// 시나리오 2: 최단 작업 우선 (Priority - SJF)
// ==========================================
void Kitchen_Enqueue_Priority(Queue* kitchen_queue, Customer* c) {
    c->next = NULL;

    // 1. 주방 큐가 아예 비어있는 경우
    if (IsEmpty(kitchen_queue)) {
        kitchen_queue->front = c;
        kitchen_queue->rear = c;
    }
    // 2. 새로 들어온 요리가 지금 맨 앞 요리보다 시간이 짧아서 맨 앞에 서야 할 때
    else if (c->remaining_time < kitchen_queue->front->remaining_time) {
        c->next = kitchen_queue->front;
        kitchen_queue->front = c;
    }
    // 3. 중간이나 맨 뒤에 들어가야 할 때 (내 자리 찾기)
    else {
        Customer* current = kitchen_queue->front;
        
        // 내 조리시간보다 더 오래 걸리는 요리를 찾을 때까지 뒤로 이동
        while (current->next != NULL && current->next->remaining_time <= c->remaining_time) {
            current = current->next;
        }
        
        // 자리 찾았으면 끼워 넣기
        c->next = current->next;
        current->next = c;

        // 만약 맨 마지막에 삽입되었다면 rear 갱신
        if (c->next == NULL) {
            kitchen_queue->rear = c;
        }
    }
    
    // 큐 대기 인원 증가!
    kitchen_queue->count++; 
}

// ==========================================
// 주방 타이머 및 완료 처리
// ==========================================
Customer* Kitchen_Process_Tick(Queue* kitchen_queue) {
    if (IsEmpty(kitchen_queue)) {
        return NULL; // 주방에 요리가 없으면 아무것도 안 함
    }

    // 주방 맨 앞에서 요리 중인 손님 확인
    Customer* current = kitchen_queue->front;

    // 타이머 1초 깎기
    current->remaining_time--;

    // 0초가 되어 요리가 다 끝났다면?
    if (current->remaining_time <= 0) {
        // 큐에서 빼내서 리턴 (이충한 팀장이 받아서 퇴장시킬 예정)
        return Dequeue(kitchen_queue);
    }

    return NULL; // 아직 요리 중
}
