#include <stdio.h>
#include <stdlib.h>
#include "manager.h"
#include "queue.h"

// 매장 내 가용 좌석(토큰). 파일 내부에서만 쓰도록 static 선언
static int available_seats = 0; 
static int total_customers_served = 0; // 서빙 완료된 총 고객 수
static int total_stay_time = 0;        // 모든 고객의 체류 시간 합

void InitManager(int max_seats) {
    available_seats = max_seats;
    printf("매장 오픈 완료! 총 가용 좌석(토큰) 수: %d\n", available_seats);
}

Customer* TryEnterStore(Queue* waiting_queue) {
    // 1. 자리가 있고 & 2. 밖에서 기다리는 사람이 있다면
    if (available_seats > 0 && !IsEmpty(waiting_queue)) {
        available_seats--; // 토큰 1개 차감 (자리 찜)
        Customer* guest = Dequeue(waiting_queue); // 웨이팅 큐에서 빼냄
        
        printf("[입장] 손님 ID: %d 매장 입장. (남은 좌석: %d)\n", guest->customer_id, available_seats);
        
        return guest; // 이 손님을 키오스크 큐로 보내기 위해 반환
    }
    
    return NULL; // 자리가 없거나 손님이 없으면 입장 불가
}

void LeaveStore(Customer* c, int current_time) {
    // 1. 퇴장 시간 기록 및 총 체류 시간 계산
    c->exit_time = current_time;
    int stay_time = c->exit_time - c->arrival_time;
    
    // 2. 통계 업데이트
    total_customers_served++;
    total_stay_time += stay_time;
    
    // 3. 토큰 반납
    available_seats++; 
    
    printf("[퇴장] 손님 ID: %d 식사 완료 및 퇴장. (체류시간: %d초, 남은 좌석: %d)\n", 
           c->customer_id, stay_time, available_seats);
           
    // 4. 매장을 완전히 떠났으므로 손님 데이터 메모리 해제
    free(c);
}
