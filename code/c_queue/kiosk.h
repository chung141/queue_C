#ifndef KIOSK_H
#define KIOSK_H

#include "types.h"
#include "queue.h"

// =========================================================
// [수마미야 담당] 키오스크 및 손님 생성 헤더 파일 (kiosk.h)
// =========================================================

/*
 * 1. 난수 기반 손님 생성 함수
 * - 역할: 특정 확률(예: 30%)로 손님 데이터를 malloc으로 생성해서 반환
 * - 매개변수: 
 * - current_time: 현재 시뮬레이션 시간 (손님의 arrival_time에 넣기 위함)
 * - current_id: 현재까지 온 손님 수 (ID 부여용)
 * - 리턴값: 새로 생성된 Customer* 포인터 (손님이 안 오는 틱에는 NULL 반환)
 * - rand()를 써서 손님의 menu_type(1~3)을 랜덤으로 정하고, 
 * 메뉴에 따라 total_cooking_time과 remaining_time을 세팅해주세요!
 */
Customer* Generate_Random_Customer(int current_time, int current_id);

/*
 * 2. 키오스크 주문 처리 함수
 * - 역할: 키오스크 큐의 맨 앞 손님의 주문 시간(예: 10초)을 다뤘는지 체크
 * - 매개변수: 
 * - kiosk_queue: 현재 손님들이 서 있는 키오스크 큐
 * - 리턴값: 주문을 마친 손님의 Customer* 포인터 (아직 주문 중이면 NULL 반환)
 * - 주문이 끝난 손님을 queue.c에 있는 Dequeue()를 이용해 빼낸 뒤 
 * 리턴해주면, 메인에서 받아서 주방으로 넘길게요

 ++
 * 손님 생성할 때 타이머 변수(remaining_time)에 주방 요리 시간만 넣지 말고, **'요리 시간 + 손님이 밥 먹는 시간(예: 10분)'**을 합쳐서 세팅해주세요!!!
 */
Customer* Kiosk_Process_Order(Queue* kiosk_queue);

#endif
