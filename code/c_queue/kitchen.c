#include "types.h"
#include "queue.h"

// [시나리오 1] 무조건 맨 뒤에 넣는 기본 함수
void Kitchen_Enqueue_FIFO(Queue* q, Customer* c);

// [시나리오 2] 조리시간(remaining_time)을 비교해서 중간에 끼워 넣는 함수
void Kitchen_Enqueue_Priority(Queue* q, Customer* c);

// [공통 사용] 요리 타이머를 깎고 0초면 빼내는 함수 (시나리오 1, 2 공통)
Customer* Kitchen_Process_Tick(Queue* q);
