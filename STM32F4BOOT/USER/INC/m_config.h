//===============================================================================================
/************************************************************************************************
*                                                                                                     
*            �ļ�����:              
*
*                ժҪ: 
*
*            ����ʱ��: 2012-05-20  
*
*            �޸�ʱ��: 2012-05-20 
*            �޸�ԭ��:
*
*            
*
*                ����: ������
*
*************************************************************************************************/
//===============================================================================================

#ifndef __CONFIG_H
#define __CONFIG_H

//=================================================================================================
//*************************************************************************************************
//�궨��
#define      __MY_DEBUG

#define      __TRACE_EN  1
 

#define   DEBUG

#ifdef __MY_DEBUG

///#define  _USE_OS

#define   COMM_UART          0
 
// #include <stdlib.h>
#include <stdio.h>
// #include <stdarg.h>
#endif


#define TMPTR_USING  1

//=================================================================================================
//�ж����ȼ�
//#define __NVIC_PRIO_BITS          5        �����Ѷ���

#define  TIMER0_IRQ_PRIO      3  
#define  TIMER1_IRQ_PRIO      4


#define  TIMER2_IRQ_PRIO      5
#define  TIMER3_IRQ_PRIO      6
#define  TIMER5_IRQ_PRIO      7

#define  TIMER_IRQ_PRIO       TIMER5_IRQ_PRIO
#define  TIMER_IRQ_SUB1       1
#define  TIMER_IRQ_SUB2       2
#define  TIMER_IRQ_SUB3       3
#define  TIMER_IRQ_SUB4       4
#define  TIMER_IRQ_SUB5       5
#define  TIMER_IRQ_SUB6       6


#define  UART0_IRQ_PRIO       8
#define  UART1_IRQ_PRIO       9
#define  UART2_IRQ_PRIO       10
///#define  UART3_IRQ_PRIO       11


#define  I2C_DMA_IRQ_PRIO     11


#define  RTC_ALARM_PRIO       13

#define  I2C_IRQ_PRIO         12

#define   TIM2_CC_IRQ_PRIO    15

#define   EXTI15_10_IRQ_PRIO  16
#define   EXTI_IRQ_PRIO       17
#define   EXTI_IRQ_PRIO_SUB1  1
#define   EXTI_IRQ_PRIO_SUB2  2
#define   EXTI_IRQ_PRIO_SUB3  3
#define   EXTI_IRQ_PRIO_SUB4  4
#define   EXTI_IRQ_PRIO_SUB5  5
#define   EXTI_IRQ_PRIO_SUB6  6


#define   EXTI9_5_IRQ_PRIO    18


#define  TIMER4_IRQ_PRIO      19


#define  DEBUG_UART_CHNNL   USART1

#define  MAX_REV     4000
#define  MIN_REV     40

//=================================================================================================
//ע��ֻ��ʹ������ĸ�ʽ������ͻ�������
extern void  Trace(char *fmt, ...);  //�˺�������� __MY_DEBUG �²��ÿ���Ϊ��
#if  __TRACE_EN == 1
  #define  TRACE  printf
#else
  #define  TRACE  1 ? (void)0 : Trace       
#endif

#ifdef __MY_DEBUG
 //extern   int    AvoidNoUseWarning;
 //#define  AVOID_NO_USE_W(var)   AvoidNoUseWarning = var 
 #define  AVOID_NO_USE_W(var)   var = var

#else
  #define  AVOID_NO_USE_W(var)
#endif


//#define  _USE_OS

//========================================================================================
//
extern uint32_t SystemFrequency;

//=================================================================================================


   



#endif    //__CONFIG_H



