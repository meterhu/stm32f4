/**
date: 2012-03-28
author: wzh
e-mail: wangzhihai_138@163.com

note: app_hook source file.

<for STM32F4 discovery board.> */

/* includes----------------------------------------------*/
#include "app_hook.h"

/* variables---------------------------------------------*/
uint32_t cpu_usage_current=0;


void vApplicationStackOverflowHook(xTaskHandle *pxTask, signed char *pcTaskName)
{
	(void) pxTask;
	(void) pcTaskName;
	
	taskDISABLE_INTERRUPTS(); //disable interrupt.
	while(1);
}

void vApplicationTickHook(void)
{
	
}

void vApplicationMallocFailedHook(void)
{
	
}

void vApplicationIdleHook(void)
{
	cpu_usage_current++; //compute cpu usage rate.
}


