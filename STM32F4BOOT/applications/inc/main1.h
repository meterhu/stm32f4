/**
date: 2012-08-14
author: wzh
e-mail: wangzhihai_138@163.com

note: main header file.

<for STM32F4 discovery board.> */

#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
 extern "C" {
#endif
   
/* includes----------------------------------------------*/
#include "stm32f4xx.h"
#include "ucos_ii.h"
	 
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
#define APP_TASK0_STK_SIZE				512
#define APP_TASK0_PRIO					8

/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
static OS_STK		App_Task0Stack[APP_TASK0_STK_SIZE];
	 
/* Private function prototypes -----------------------------------------------*/
static void App_Task0(void *p_arg);


   
#ifdef __cplusplus
 }
#endif /* C++ */

#endif /* MAIN_H */

/* end of file */
