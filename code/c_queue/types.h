#ifndef TYPES_H
#define TYPES_H

#include <stdbool.h>

//손님 구조체와 큐 구조체 헤더 파일입니다~ 각자 편한 변수명으로 바꿔서 사용하셔도 됩니다! 대신 주석달고 문자로 알려주세요(변수명은 일치해야 합니다.)

//"수마미야님이 만들 Kiosk_Dequeue 함수는 리턴 값으로 무조건 Customer* 포인터를 뱉어내게 만들어주세요. 그래야 김지원님이 그걸 받아서 씁니다."
//"김지원님이 만들 Kitchen_Enqueue 함수는 파라미터로 Customer*를 받게 세팅해 주세요."

// 1. 손님 구조체 (시스템을 돌아다닐 핵심 데이터)
typedef struct Customer {
    int customer_id;         // 손님 고유 번호 (1번, 2번...)

    //  [통계용 시간 데이터] - 수마미야님과 제가 사용할 변수입니다.
    int arrival_time;        // 웨이팅 큐에 도착한 시간 (입장시간)
    int exit_time;           // 식사를 마치고 퇴장한 시간 (퇴장시간)

    //  [조리 및 시나리오 2 통제용 데이터] - 김지원님이 사용할 변수입니다.
    int menu_type;           // 주문한 메뉴 번호
    int total_cooking_time;  // 총 조리 소요 시간 (우선순위 큐 정렬 기준!)
    int remaining_time;      // 남은 시간 (주방이나 좌석에서 1초마다 깎일 타이머 변수)

    //  [자료구조 연결용]
    struct Customer* next;   // 다음 손님을 가리키는 포인터
} Customer;

// 2. 큐 구조체 (대기열 상태)
typedef struct Queue {
    Customer* front;         // 큐의 맨 앞 (Dequeue 대상)
    Customer* rear;          // 큐의 맨 뒤 (Enqueue 대상)
    int count;               // 현재 큐에 쌓인 데이터(사람) 수
} Queue;

#endif