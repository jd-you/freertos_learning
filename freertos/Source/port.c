#include "port.h"
#include "FreeRTOSConfig.h"
#include "task.h"

#define portINITIAL_XPSR (0x01000000)
#define portSTART_ADDRESS_MASK ((StackType_t)0xfffffffeUL)

#define portNVIC_SYSPRI2_REG (*((volatile uint32_t *)0xe000ed20))
#define portNVIC_PENSV_PRI (((uint32_t)configKERNEL_INTERRUPT_PRIORITY) << 16UL)
#define portNVIC_SYSTICK_PRI (((uint32_t)configKERNEL_INTERRUPT_PRIORITY) << 24L)

static void prvTaskExitError(void)
{
    for (;;)
        ;
}

StackType_t *pxPortInitialiseStack(StackType_t *pxTopOfStack,
                                   TaskFunction_t pxCode,
                                   void *pvParameters)
{
    pxTopOfStack--;
    *pxTopOfStack = portINITIAL_XPSR;
    pxTopOfStack--;
    *pxTopOfStack = ((StackType_t)pxCode) & portSTART_ADDRESS_MASK;
    pxTopOfStack--;
    *pxTopOfStack = (StackType_t)prvTaskExitError;
    pxTopOfStack -= 5;
    *pxTopOfStack = (StackType_t)pvParameters;

    pxTopOfStack -= 8;

    return pxTopOfStack;
}

__asm void prvStartFirstTask(void)
{
    PRESERVE8

    ldr r0, =0xE000ED08
    ldr r0, [r0]
    ldr r0, [r0]

    msr msp, r0

    cpsie i
    cpsie f
    dsb
    isb

    svc 0
    nop
    nop
}

__asm void vPortSVCHandler(void)
{
    extern pxCurrentTCB;

    PRESERVE8

    ldr r3, =pxCurrentTCB;
    ldr r1, [r3]
    ldr r0, [r1]

    ldmia r0!, {r4-r11}
    msr psp, r0
    isb
    mov r0, #0
    msr basepri, r0
    orr r14, #0xd

    bx r14
}

__asm void xPortPendSVHandler(void)
{
    extern pxCurrentTCB;
    extern vTaskSwitchContext;

    PRESERVE8

    mrs r0, psp
    isb

    ldr r3, =pxCurrentTCB;
    ldr r2, [r3]

    stmdb r0!, {r4-r11}
    str r0, [r2]

    stmdb sp!, {r3, r14}
    mov r0, #configMAX_SYSCALL_INTERRUPT_PRIORITY
    msr basepri, r0
    dsb
    isb
    bl vTaskSwitchContext
    mov r0, #0
    msr basepri, r0
    ldmia sp!, {r3, r14}

    ldr r1, [r3]
    ldr r0, [r1]
    ldmia r0!, {r4-r11}
    msr psp, r0
    isb
    bx r14
    nop
}

BaseType_t xPortStartScheduler(void)
{
    portNVIC_SYSPRI2_REG |= portNVIC_PENSV_PRI;
    portNVIC_SYSPRI2_REG |= portNVIC_SYSPRI2_REG;

    prvStartFirstTask();
    return 0;
}

TickType_t xTickCount;
void xTaskIncrementTick( void )
{
    TCB_t *pxTCB = NULL;
    BaseType_t i = 0;
    /* 更新系统时基计数器xTickCount，xTickCount 是一个在port.c 中定义的全局变量 */
    const TickType_t xConstTickCount = xTickCount + 1;
    xTickCount = xConstTickCount;

    /* 扫描就绪列表中所有任务的xTicksToDelay，如果不为0，则减1 */
    for (i=0; i<configMAX_PRIORITIES; i++){
        pxTCB = ( TCB_t * ) listGET_OWNER_OF_HEAD_ENTRY( ( &pxReadyTasksLists[i] ) );
        if (pxTCB->xTicksToDelay > 0) {
            pxTCB->xTicksToDelay --;
        }
    }

    /* 任务切换 */
    portYIELD();
}

void xPortSysTickHandler(void)
{
    xTaskIncrementTick();
}
