# kiosk.c 완성본 (kitchen.c와 연동 가능 버전)

```c
// kiosk.c

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "kiosk.h"
#include "queue.h"

1. Generate_Random_Customer()
   -> 랜덤 확률로 손님 생성
   -> 손님 정보 저장
   -> remaining_time 에는
      "키오스크 주문 시간" 을 넣음

2. 메인에서 kiosk_queue에 Enqueue

3. Kiosk_Process_Order()
   -> 키오스크 주문 시간 감소
   -> 주문 완료 시 remaining_time을
      "조리시간 + 식사시간" 으로 변경
   -> Kitchen Queue로 넘길 Customer* 반환

4. kitchen.c 에서 remaining_time 감소
   -> 조리 + 식사 끝나면 퇴장

============================================================
중요한 설계 포인트
============================================================

remaining_time 을 단계마다 역할 변경해서 사용함.

[키오스크 단계]
remaining_time = 주문 시간

[주방 단계]
remaining_time = 조리 시간 + 식사 시간

이렇게 해야 구조체를 추가 수정하지 않아도 되고,
김지원님의 kitchen.c 와 충돌 없이 연동 가능함.

============================================================
*/


// ---------------------------------------------------------
// 설정값
// ---------------------------------------------------------

#define CUSTOMER_ENTER_PERCENT 30

// 키오스크 주문 시간
#define ORDER_TIME 3

// 식사 시간
#define EATING_TIME 10


// ---------------------------------------------------------
// 메뉴별 조리시간 반환 함수
// ---------------------------------------------------------
int Get_Cooking_Time(int menu_type)
{
    switch(menu_type)
    {
        case 1:
            return 5;

        case 2:
            return 10;

        case 3:
            return 15;

        default:
            return 5;
    }
}


// ---------------------------------------------------------
// 1. 난수 기반 손님 생성 함수
// ---------------------------------------------------------
Customer* Generate_Random_Customer(int current_time, int current_id)
{
    // 0~99 난수 생성
    int chance = rand() % 100;

    // 특정 확률로 손님 생성
    if(chance < CUSTOMER_ENTER_PERCENT)
    {
        // 동적 메모리 할당
        Customer* new_customer =
            (Customer*)malloc(sizeof(Customer));

        // 메모리 할당 실패 체크
        if(new_customer == NULL)
        {
            return NULL;
        }

        // 기본 정보 저장
        new_customer->customer_id = current_id;

        // 손님 도착 시간
        new_customer->arrival_time = current_time;

        // 아직 퇴장 안 했으므로 -1
        new_customer->exit_time = -1;

        // 메뉴 랜덤 선택 (1~3)
        new_customer->menu_type = rand() % 3 + 1;

        // 총 조리 시간 저장
        new_customer->total_cooking_time =
            Get_Cooking_Time(new_customer->menu_type);

        /*
        ====================================================
        현재 단계는 키오스크 단계
        -> remaining_time 은 주문 시간
        ====================================================
        */
        new_customer->remaining_time = ORDER_TIME;

        // 연결 포인터 초기화
        new_customer->next = NULL;

        printf("[입장] 손님 %d 생성 | 메뉴:%d | 조리:%d초 | 현재틱:%d\n",
               new_customer->customer_id,
               new_customer->menu_type,
               new_customer->total_cooking_time,
               current_time);

        return new_customer;
    }

    // 손님 생성 실패
    return NULL;
}


// ---------------------------------------------------------
// 2. 키오스크 주문 처리 함수
// ---------------------------------------------------------
Customer* Kiosk_Process_Order(Queue* kiosk_queue)
{
    // 키오스크 큐 비어있으면 종료
    if(IsEmpty(kiosk_queue))
    {
        return NULL;
    }

    // 맨 앞 손님 확인
    Customer* current_customer = kiosk_queue->front;

    // 주문 시간 감소
    current_customer->remaining_time--;

    printf("[키오스크] 손님 %d 주문중... 남은시간:%d\n",
           current_customer->customer_id,
           current_customer->remaining_time);


    // 주문 완료 확인
    if(current_customer->remaining_time <= 0)
    {
        // 큐에서 제거
        Customer* finished_customer = Dequeue(kiosk_queue);

        /*
        ====================================================
        여기 매우 중요

        이제 손님은 kitchen.c 로 이동함.

        kitchen.c 는 remaining_time 을 감소시키므로
        여기서 remaining_time 을

        "조리시간 + 식사시간"

        으로 다시 세팅해야 함.
        ====================================================
        */

        finished_customer->remaining_time =
            finished_customer->total_cooking_time + EATING_TIME;


        printf("[주문완료] 손님 %d -> 주방 이동 | 남은시간:%d\n",
               finished_customer->customer_id,
               finished_customer->remaining_time);

        // kitchen.c 로 넘길 손님 반환
        return finished_customer;
    }

    // 아직 주문 중
    return NULL;
}

```

---

# 왜 이렇게 수정했는지 설명

## 기존 코드 문제점

기존 코드에서는:

```c
new_customer->remaining_time =
    new_customer->total_cooking_time;
```

이렇게 되어 있었음.

그러면 키오스크 단계에서 remaining_time이 감소해버려서
조리시간이 먼저 깎이는 문제 발생.

즉:

* 키오스크에서 시간 감소
* 주방에서도 시간 감소

=> 시간 로직이 꼬이게 됨.

---

# 지금 수정한 방식

## 단계별 역할 분리

### 1단계: 키오스크

```c
remaining_time = ORDER_TIME;
```

키오스크 주문만 처리.

---

### 2단계: 주문 완료 직후

```c
remaining_time = total_cooking_time + EATING_TIME;
```

이제 kitchen.c가 감소시킬 실제 시간 설정.

---

# kitchen.c와 연동 시 흐름

```text
손님 생성
↓
Kiosk Queue 입장
↓
Kiosk_Process_Order()
↓
remaining_time 감소
↓
주문 완료
↓
remaining_time = 조리 + 식사 시간
↓
Kitchen Queue 이동
↓
Kitchen_Process_Tick()
↓
remaining_time 감소
↓
0 되면 퇴장
```

---

# 시뮬레이션 예시

예:

* 현재 tick = 20
* 손님 7 생성
* 메뉴 2 선택
* 조리시간 = 10

생성 직후:

```text
remaining_time = 3
```

키오스크 3틱 지나면:

```text
remaining_time = 0
```

주문 완료 후:

```text
remaining_time = 10 + 10
               = 20
```

그 후 kitchen.c에서 20틱 감소.

0 되면:

```c
exit_time = current_tick;
```

계산 가능:

```c
총 체류시간 = exit_time - arrival_time
```

---

# 중요 포인트

이 구조는:

* FIFO
* Priority Queue(SJF)

둘 다 완벽히 연동 가능.

왜냐면:

```c
remaining_time
```

을 kitchen.c에서 우선순위 비교에 그대로 사용하기 때문.

즉:

```c
current->next->remaining_time
```

비교가 정상 작동함.

---

# 메인에서 연결 예시

```c
Customer* finished_order;

finished_order = Kiosk_Process_Order(&kiosk_queue);

if(finished_order != NULL)
{
    Kitchen_Enqueue_FIFO(&kitchen_queue, finished_order);
}
```

Priority 버전:

```c
Kitchen_Enqueue_Priority(&kitchen_queue, finished_order);
```
