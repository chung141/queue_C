#include <stdio.h>
#include <stdlib.h>
#include <time.h>    // 난수(랜덤) 시드 설정용
#include "types.h"
#include "queue.h"
#include "manager.h"
#include "kiosk.h"
#include "kitchen.h"

#define SIMULATION_TIME 3600 // 총 영업시간: 1시간(3600초)

int main() {
    int scenario_mode;
    printf("========== 6조 식당 시뮬레이션 ==========\n");
    printf("실행할 시나리오를 선택하세요.\n");
    printf("1: 시나리오 1 (FIFO 주방)\n");
    printf("2: 시나리오 2 (SJF 우선순위 주방)\n");
    printf("입력: ");
    scanf("%d", &scenario_mode);

    // 1. 공정한 비교를 위해 고정된 시드값 사용 (매번 똑같은 손님 패턴이 오게 함)
    srand(1234); 

    // 2. 큐 초기화
    Queue waiting_queue, kiosk_queue, kitchen_queue;
    InitQueue(&waiting_queue);
    InitQueue(&kiosk_queue);
    InitQueue(&kitchen_queue);

    // 3. 매니저(이충한) 초기화 (좌석 10개, 선택한 시나리오 번호 전달)
    InitManager(10, scenario_mode);

    // 4. ⏰ 시간의 흐름 (메인 시뮬레이션 루프)
    int current_time = 0;
    int current_id = 0; // 손님 번호표

    while (current_time < SIMULATION_TIME) {
        
        // [STEP 1: 손님 유입] - 수마미야 담당
        // 1초마다 랜덤하게 손님이 오는지 확인
        Customer* new_guest = Generate_Random_Customer(current_time, current_id);
        if (new_guest != NULL) {
            current_id++;
            Enqueue(&waiting_queue, new_guest); // 웨이팅 큐에 줄 세우기
        }

        // [STEP 2: 매장 입장 통제] - 이충한 담당
        // 자리가 났으면 웨이팅 큐 -> 매장 안(키오스크 큐)으로 들여보냄
        Customer* entering_guest = TryEnterStore(&waiting_queue);
        if (entering_guest != NULL) {
            Enqueue(&kiosk_queue, entering_guest);
        }

        // [STEP 3: 키오스크 주문] - 수마미야 담당
        // 키오스크에서 주문이 끝난 손님이 나오면 주방 큐로 넘김
        Customer* ordered_guest = Kiosk_Process_Order(&kiosk_queue);
        if (ordered_guest != NULL) {
            // 💡 여기서 시나리오에 따라 주방으로 들어가는(Enqueue) 방식이 갈림!
            if (scenario_mode == 1) {
                Kitchen_Enqueue_FIFO(&kitchen_queue, ordered_guest);
            } else if (scenario_mode == 2) {
                Kitchen_Enqueue_Priority(&kitchen_queue, ordered_guest);
            }
        }

        // [STEP 4: 주방 요리 및 퇴장] - 김지원 & 이충한 담당
        // 주방에서 1초씩 요리를 하고, 다 된 요리(손님)가 나오면 퇴장시킴
        Customer* finished_guest = Kitchen_Process_Tick(&kitchen_queue);
        if (finished_guest != NULL) {
            LeaveStore(finished_guest, current_time); // 식사 완료, 퇴장 및 통계 기록!
        }

        // 시간 1초 증가
        current_time++;
    }

    // 5. 시뮬레이션 종료 및 엑셀(CSV) 저장 마무리
    CloseManager();
    printf("시뮬레이션이 성공적으로 종료되었습니다. CSV 파일을 확인해주세요!\n");

    return 0;
}
