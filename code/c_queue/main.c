#include <stdio.h>
#include <stdlib.h>
#include "types.h"
#include "queue.h"
#include "manager.h"

int main() {
    printf("========== [1단계 테스트] 큐 & 매니저 연동 테스트 ==========\n\n");

    // 1. 매장 및 큐 초기화 (테스트를 위해 좌석을 딱 2개만 줍니다)
    InitManager(2); 
    
    Queue waitingQueue;
    InitQueue(&waitingQueue);

    // 2. 가상의 손님 3명 생성 (malloc 사용)
    Customer* c1 = (Customer*)malloc(sizeof(Customer));
    c1->customer_id = 1; c1->arrival_time = 0; // 0초에 도착

    Customer* c2 = (Customer*)malloc(sizeof(Customer));
    c2->customer_id = 2; c2->arrival_time = 0; // 0초에 도착

    Customer* c3 = (Customer*)malloc(sizeof(Customer));
    c3->customer_id = 3; c3->arrival_time = 2; // 2초에 도착

    // 3. 웨이팅 큐에 손님들 줄 세우기 (Enqueue 테스트)
    printf("\n[이벤트] 손님 3명이 매장 밖 웨이팅 큐에 줄을 섭니다.\n");
    Enqueue(&waitingQueue, c1);
    Enqueue(&waitingQueue, c2);
    Enqueue(&waitingQueue, c3);
    printf("현재 웨이팅 큐 대기 인원: %d명\n\n", waitingQueue.count);

    // 4. 매장 입장 시도 (TryEnterStore 테스트)
    printf("--- [시간: 5초] 매장 문 개방 및 입장 시도 ---\n");
    Customer* inside_c1 = TryEnterStore(&waitingQueue); // 좌석 1개 소모 (남은 좌석 1)
    Customer* inside_c2 = TryEnterStore(&waitingQueue); // 좌석 1개 소모 (남은 좌석 0)
    Customer* inside_c3 = TryEnterStore(&waitingQueue); // 남은 좌석이 없어서 NULL 반환되어야 함

    if (inside_c3 == NULL) {
        printf("손님 3번은 좌석이 꽉 차서 입장하지 못하고 웨이팅 큐에서 대기합니다. (남은 대기: %d명)\n\n", waitingQueue.count);
    }

    // 5. 1번 손님 식사 완료 및 퇴장 (LeaveStore 테스트)
    printf("--- [시간: 15초] 1번 손님 식사 완료 ---\n");
    LeaveStore(inside_c1, 15); // 15초에 퇴장 (토큰 +1 반납)

    // 6. 자리 났으니 3번 손님 다시 입장 시도
    printf("\n--- [시간: 16초] 3번 손님 다시 입장 시도 ---\n");
    inside_c3 = TryEnterStore(&waitingQueue); // 방금 자리가 났으므로 입장 성공해야 함

    // 7. 남은 손님들 퇴장 (메모리 누수 방지)
    printf("\n--- [시간: 30초] 영업 종료, 남은 손님 모두 퇴장 ---\n");
    LeaveStore(inside_c2, 30);
    if (inside_c3 != NULL) LeaveStore(inside_c3, 30);

    printf("\n========== 테스트가 성공적으로 종료되었습니다. ==========\n");

    return 0;
}
