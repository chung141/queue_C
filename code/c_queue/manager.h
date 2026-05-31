#ifndef MANAGER_H
#define MANAGER_H

#include "types.h"

// 매장 관리 시스템 초기화 (총 좌석 수 세팅)
void InitManager(int max_seats);

// 웨이팅 큐에서 손님을 매장으로 입장 시도 (토큰 발급)
Customer* TryEnterStore(Queue* waiting_queue);

// 식사를 마친 손님 퇴장 처리 (토큰 회수 및 데이터 통계 기록)
void LeaveStore(Customer* c, int current_time);

#endif