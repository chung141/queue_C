#ifndef KITCHEN_H
#define KITCHEN_H

#include "types.h"
#include "queue.h"

// =========================================================
// [김지원 담당] 주방 큐 및 조리 알고리즘 헤더 파일 (kitchen.h)
// =========================================================

/*
 * 1. [시나리오 1] 기본 FIFO 주방 큐 삽입 
 * - 역할: 키오스크에서 넘어온 주문을 무조건 큐의 맨 뒤에 입력
 * - 이 함수는 queue.c에 있는 Enqueue()를 그대로 호출하면 될 거 같아요
 */
void Kitchen_Enqueue_FIFO(Queue* kitchen_queue, Customer* c);

/*
 * 2. [시나리오 2] 최단 작업 우선(SJF) 우선순위 큐 삽입      (시나리오를 나눌 핵심 부분입니다)
 * - 역할: 들어온 순서와 상관없이 조리 시간(remaining_time)이 짧은 사람이 
 * 먼저 요리되도록 큐의 중간(알맞은 위치)에 끼워 넣고,
 * - 단순 Enqueue가 아니라, Linked List의 next 포인터를 비교하면서 
 * 들어갈 자리를 찾는 정렬 알고리즘이 필요해요
 */
void Kitchen_Enqueue_Priority(Queue* kitchen_queue, Customer* c);

/*
 * 3. 주방 조리 타이머 진행 및 완료 처리 (시나리오 1, 2 공통 사용)
 * - 역할: 매 1초(Tick)마다 호출되며, 주방 큐 맨 앞 요리의 remaining_time을 -1 깎습니다.
 * - 리턴값: 시간이 0이 되어 요리가 완성된 손님의 Customer* 포인터. 
 * (아직 조리 중이거나 큐가 비어있으면 NULL 반환)
 * - 요리가 완성되면 Dequeue()로 빼서 리턴부탁 드립니다.
 * 그러면 제가(이충한) 받아서 손님을 퇴장시킬게요
 */
Customer* Kitchen_Process_Tick(Queue* kitchen_queue);

#endif
