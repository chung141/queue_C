// kiosk.c

#include <stdio.h>
#include <stdlib.h>
#include "kiosk.h"
#include "queue.h"

#define CUSTOMER_ENTER_PERCENT 30
#define ORDER_TIME 3
#define EATING_TIME 10

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

Customer* Generate_Random_Customer(int current_time, int current_id)
{
    int chance = rand() % 100;

    if(chance < CUSTOMER_ENTER_PERCENT)
    {
        Customer* new_customer =
            (Customer*)malloc(sizeof(Customer));

        if(new_customer == NULL)
        {
            return NULL;
        }

        new_customer->customer_id = current_id;
        new_customer->arrival_time = current_time;
        new_customer->exit_time = -1;

        new_customer->menu_type = rand() % 3 + 1;

        new_customer->total_cooking_time =
            Get_Cooking_Time(new_customer->menu_type);

        new_customer->remaining_time = ORDER_TIME;

        new_customer->next = NULL;

        return new_customer;
    }

    return NULL;
}

Customer* Kiosk_Process_Order(Queue* kiosk_queue)
{
    if(IsEmpty(kiosk_queue))
    {
        return NULL;
    }

    Customer* current_customer = kiosk_queue->front;

    current_customer->remaining_time--;

    if(current_customer->remaining_time <= 0)
    {
        Customer* finished_customer =
            Dequeue(kiosk_queue);

        finished_customer->remaining_time =
            finished_customer->total_cooking_time
            + EATING_TIME;

        return finished_customer;
    }

    return NULL;
}
