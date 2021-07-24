#ifndef PORT_H
#define PORT_H

#include "portmacro.h"
#include "projdefs.h"

extern StackType_t *pxPortInitialiseStack(StackType_t *pxTopOfStack,
                                          TaskFunction_t pxCode,
                                          void *pvParameters);
                                          
extern BaseType_t xPortStartScheduler(void);
#endif