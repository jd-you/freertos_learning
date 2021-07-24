#include "task.h"
#include "port.h"

TCB_t *pxCurrentTCB;

TCB_t Task1TCB; // TODO
TCB_t Task2TCB; // TODO
List_t pxReadyTasksLists[configMAX_PRIORITIES]; // TODO

static void prvInitialiseNewTask(TaskFunction_t pxTaskCode,
                                 const char *const pcName,
                                 const uint32_t ulStackDepth,
                                 void *const pvParameters,
                                 TaskHandle_t *const pxCreatedTask,
                                 TCB_t *pxNewTCB);

#if (configSUPPOTR_STATIC_ALLOCATION == 1)
TaskHandle_t xTaskCreateStatic(TaskFunction_t pxTaskCode,
                               const char *const pcName,
                               const uint32_t ulStackDepth,
                               void *const pvParameters,
                               StackType_t *const puxStackBuffer,
                               TCB_t *const pxTaskBuffer)
{
    TCB_t *pxNewTCB;
    TaskHandle_t xReturn;

    if ((pxTaskBuffer != NULL) && (puxStackBuffer != NULL))
    {
        pxNewTCB = (TCB_t *)pxTaskBuffer;
        pxNewTCB->pxStack = (StackType_t *)puxStackBuffer;

        prvInitialiseNewTask(pxTaskCode, pcName, ulStackDepth, pvParameters, &xReturn, pxNewTCB);
    }
    else
    {
        xReturn = NULL;
    }
    return xReturn;
}
#endif

static void prvInitialiseNewTask(TaskFunction_t pxTaskCode,
                                 const char *const pcName,
                                 const uint32_t ulStackDepth,
                                 void *const pvParameters,
                                 TaskHandle_t *const pxCreatedTask,
                                 TCB_t *pxNewTCB)
{
    StackType_t *pxTopOfStack;
    UBaseType_t x;
    pxTopOfStack = pxNewTCB->pxStack + (ulStackDepth - (uint32_t)1);
    pxTopOfStack = (StackType_t *)(((uint32_t)pxTopOfStack) & (~((uint32_t)0x0007))); // 这个变量实际是32bit的地址（64bit），所以需要是4（8）的整倍数

    for (x = (UBaseType_t)0; x < (UBaseType_t)configMAX_TASK_NAME_LEN; ++x)
    {
        pxNewTCB->pcTaskName[x] = pcName[x];
        if (pcName[x] == 0x00)
        {
            break;
        }
    }

    pxNewTCB->pcTaskName[configMAX_TASK_NAME_LEN - 1] = '\0';

    vListInitialiseItem(&(pxNewTCB->xStateListItem));
    listSET_LIST_ITEM_OWNER(&(pxNewTCB->xStateListItem), pxNewTCB);

    pxNewTCB->pxTopOfStack = pxPortInitialiseStack(pxTopOfStack, pxTaskCode, pvParameters);

    if ((void *)pxCreatedTask != NULL)
    {
        *pxCreatedTask = (TaskHandle_t)pxNewTCB;
    }
}

void prvInitialiseTaskLists(void)
{
    UBaseType_t uxPriority;

    for (uxPriority = (UBaseType_t)0U;
         uxPriority < (UBaseType_t)configMAX_PRIORITIES;
         ++uxPriority)
    {
        vListInitialise(&(pxReadyTasksLists[uxPriority]));
    }
}

void vTaskStartScheduler(void)
{
    pxCurrentTCB = &Task1TCB;

    if (xPortStartScheduler() != pdFALSE) {

    }
}

void vTaskSwitchContext(void)
{
    if (pxCurrentTCB == &Task1TCB)
    {
        pxCurrentTCB = &Task2TCB;
    } else {
        pxCurrentTCB = &Task1TCB;
    }
}