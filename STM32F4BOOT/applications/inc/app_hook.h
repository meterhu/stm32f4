/**
date: 2012-03-28
author: wzh
e-mail: wangzhihai_138@163.com

note: app_hook header file.

<for STM32F4 discovery board.> */

#ifndef APP_HOOK_H
#define APP_HOOK_H

#ifdef __cplusplus
 extern "C" {
#endif
   
/* Includes----------------------------------------------*/
#include "stm32f4xx.h"
#include "freertos.h" 
#include "task.h"
	 
/* Function prototypes ----------------------------------*/   
void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName);
void vApplicationTickHook(void);   
void vApplicationMallocFailedHook(void);   
void vApplicationIdleHook(void);


#ifdef __cplusplus
 }
#endif /* C++ */

#endif /* APP_HOOK_H */


















