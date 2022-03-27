#ifndef TASK_H
#define TASK_H

#include "portmacro.h"
#include "list.h"
#include "FreeRTOSConfig.h"
#include "projdefs.h"

#define  taskYIELD() portYIELD()

struct taskTaskControlBlock
{
    volatile StackType_t *pxTopOfStack;
    ListItem_t xStateListItem;
    StackType_t *pxStack;
    char pcTaskName[configMAX_TASK_NAME_LEN];
    TickType_t xTicksToDelay;
};

typedef struct taskTaskControlBlock TCB_t;

typedef void * TaskHandle_t;

#if (configSUPPOTR_STATIC_ALLOCATION == 1)
TaskHandle_t xTaskCreateStatic(TaskFunction_t pxTaskCode,
                               const char *const pcName,
                               const uint32_t ulStackDepth,
                               void *const pvParameters,
                               StackType_t *const puxStackBuffer,
                               TCB_t *const pxTaskBuffer);
#endif

extern List_t pxReadyTasksLists[configMAX_PRIORITIES];

extern TCB_t Task1TCB;
extern TCB_t Task2TCB;
extern TCB_t TaskIdleTCB;

extern void vTaskStartScheduler(void);
extern void prvInitialiseTaskLists(void);
extern void vTaskDelay(const TickType_t xTicksToDelay);

#endif
