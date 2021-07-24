#include "task.h"

portCHAR flag1;
portCHAR flag2;

void delay(uint32_t count);
void Task1_Entry(void *p_arg);
void Task2_Entry(void *p_arg);

TaskHandle_t Task1_Handle;
#define TASK1_STACK_SIZE 128
StackType_t Task1Stack[TASK1_STACK_SIZE];
TaskHandle_t Task2_Handle;
#define TASK2_STACK_SIZE 128
StackType_t Task2Stack[TASK2_STACK_SIZE];

int main(void)
{
    prvInitialiseTaskLists();

    Task1_Handle = xTaskCreateStatic(Task1_Entry, "Task1", TASK1_STACK_SIZE, NULL, Task1Stack, &Task1TCB);
    vListInsertEnd(&pxReadyTasksLists[1], &(Task1TCB.xStateListItem));

    Task2_Handle = xTaskCreateStatic(Task2_Entry, "Task2", TASK2_STACK_SIZE, NULL, Task2Stack, &Task2TCB);
    vListInsertEnd(&pxReadyTasksLists[2], &(Task2TCB.xStateListItem));

    vTaskStartScheduler();

    while (1)
        ;
}

void delay(uint32_t count)
{
    while (count-- > 0);
}

void Task1_Entry(void *p_arg)
{
    for (;;)
    {
        flag1 = 1;
        delay(100);
        flag1 = 0;
        delay(100);

        taskYIELD();
    }
}

void Task2_Entry(void *p_arg)
{
    for (;;)
    {
        flag2 = 1;
        delay(100);
        flag2 = 0;
        delay(100);

        taskYIELD();
    }
}
