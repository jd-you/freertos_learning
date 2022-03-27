#ifndef PORTMACRO_H
#define PORTMACRO_H

#include "stdint.h"
#include "stddef.h"
#include "FreeRTOSConfig.h"

#define portCHAR char
#define portFLOAT float
#define portDOUBLE double
#define portLONG long
#define portSHORT short // 16bit for Cortex-M
#define portSTACK_TYPE uint32_t
#define portBASE_TYPE long // 32bit for Cortex-M

typedef portSTACK_TYPE StackType_t;
typedef long BaseType_t;
typedef unsigned long UBaseType_t;

#if (configUSE_16_BIT_TICKS == 1)
typedef uint16_t TickType_t;
#define portMAX_DELAY ((TickType_t)0xffff)
#else
typedef uint32_t TickType_t;
#define portMAX_DELAY ((TickType_t)0xffffffff)
#endif

#define portNVIC_INT_CTRL_REG (*((volatile uint32_t *)0xe000ed04))
#define portVECTACTIVE_MASK (0xff)
#define portNVIC_PENDSVSET_BIT (1UL << 28UL)
#define portSY_FULL_READ_WRITE (15)

// 触发一次pendSV中断
#define portYIELD()                                     \
    {                                                   \
        portNVIC_INT_CTRL_REG = portNVIC_PENDSVSET_BIT; \
        __dsb(portSY_FULL_READ_WRITE);                  \
        __isb(portSY_FULL_READ_WRITE);                  \
    }


// 关中断 （无返回值，无法在中断中使用）
#define portDISABLE_INTERRUPTS() vPortRaiseBASEPRI()
// 开中断 （无法在中断中使用）
#define portENABLE_INTERRUPTS() vPortSetBASEPRI(0)

// 关中断 （有返回值，可以在中断中使用）
#define portSET_INTERRUPT_MASK_FROM_ISR() ulPortRaiseBASEPRI()
// 开中断
#define portCLEAR_INTERRUPT_MASK_FROM_ISR(x) vPortSetBASEPRI(x)




static inline void vPortRaiseBASEPRI(void)
{
    uint32_t ulNewBASEPRI = configMAX_SYSCALL_INTERRUPT_PRIORITY;
    __asm
    {
        msr basepri, ulNewBASEPRI
        dsb
        isb
    }
}

static inline UBaseType_t ulPortRaiseBASEPRI(void)
{
    uint32_t retBASEPRI, ulNewBASEPRI = configMAX_SYSCALL_INTERRUPT_PRIORITY;
    __asm
    {
        mrs retBASEPRI, basepri
        msr basepri, ulNewBASEPRI
        dsb
        isb
    }
    return retBASEPRI;
}

static inline void vPortSetBASEPRI(UBaseType_t ulBASEPRI)
{
    __asm
    {
        msr basepri, ulBASEPRI
        dsb
        isb
    }
}

    
#endif
