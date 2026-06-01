// kiosk.c

#include <stdio.h>
#include <stdlib.h>
#include "kiosk.h"
#include "queue.h"

// =========================================================
// [수마미야 담당]
// 키오스크 및 손님 생성 구현 파일
// =========================================================


// ---------------------------------------------------------
// 1. 난수 기반 손님 생성 함수
// ---------------------------------------------------------
Customer* Generate_Random_Customer(int current_time, int current_id)
{
    // 0~99 랜덤 숫자 생성
    int chance = rand() % 100;

    // 30% 확률로 손님 생성
    if (chance < 30)
    {
        // 손님 메모리 동적 할당
        Customer* new_customer = (Customer*)malloc(sizeof(Customer));

        // 손님 정보 저장
        new_customer->customer_id = current_id;
        new_customer->arrival_time = current_time;
        new_customer->exit_time = -1;

        // 메뉴 랜덤 선택 (1~3)
        new_customer->menu_type = rand() % 3 + 1;

        // 메뉴별 조리 시간 설정
        if (new_customer->menu_type == 1)
        {
            // 메뉴 1 : 5초
            new_customer->total_cooking_time = 5;
        }
        else if (new_customer->menu_type == 2)
        {
            // 메뉴 2 : 10초
            new_customer->total_cooking_time = 10;
        }
        else
        {
            // 메뉴 3 : 15초
            new_customer->total_cooking_time = 15;
        }

        // 남은 시간 초기화
        new_customer->remaining_time =
            new_customer->total_cooking_time;

        // 다음 손님 포인터 초기화
        new_customer->next = NULL;

        // 생성된 손님 반환
        return new_customer;
    }

    // 손님이 오지 않은 경우
    return NULL;
}


// ---------------------------------------------------------
// 2. 키오스크 주문 처리 함수
// ---------------------------------------------------------
Customer* Kiosk_Process_Order(Queue* kiosk_queue)
{
    // 큐가 비어있으면 NULL 반환
    if (IsEmpty(kiosk_queue))
    {
        return NULL;
    }

    // 맨 앞 손님 확인
    Customer* current_customer = kiosk_queue->front;

    // 주문 시간 감소
    current_customer->remaining_time--;

    // 주문 완료 여부 확인
    if (current_customer->remaining_time <= 0)
    {
        // 큐에서 제거 후 반환
        return Dequeue(kiosk_queue);
    }

    // 아직 주문 중이면 NULL 반환
    return NULL;
}
