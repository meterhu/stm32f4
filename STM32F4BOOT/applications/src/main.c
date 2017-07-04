//======================================================================================================
/******************************************************************************************************
 *                                                                                                     
 *            文件名称:              
 *
 *                摘要: 
 *
 *            创建时间: 2012-05-xx  
 *
 *            修改时间: 2012-05-xx
 *            修改原因:
 *
 *            
 *
 *                作者: 
 *
 *******************************************************************************************************/
//======================================================================================================
#include "main1.h"
#include "mystd.h" 
#include "m_config.h"
#include "UserLib.h"
#include "InitHW.h"
#include "UART.h"


//======================================================================================================
/*******************************************************************************************************
 *
 *         函数名称: 
 *
 *         函数功能: 
 *
 *         入口参数: 
 *
 *         出口参数: 无
 *
 *             说明:
 *
 *******************************************************************************************************/
int main1(void)
{	
	uint8_t os_err;
    
    SystemInit();
    
    GPIO_Config();
    InitUART(USART2,115200);
    printf("program is running\n");
	
	__disable_irq(); //禁止所有中断。

	
	OSInit(); //初始化uC/OS-II这个实时内核。

	os_err = OSTaskCreateExt((void (*)(void *)) App_Task0, //创建一个开始任务。
                             (void          * ) 0,
                             (OS_STK        * )&App_Task0Stack[APP_TASK0_STK_SIZE - 1],
                             (uint8_t         ) APP_TASK0_PRIO,
                             (uint16_t        ) APP_TASK0_PRIO,
                             (OS_STK        * )&App_Task0Stack[0],
                             (INT32U          ) APP_TASK0_STK_SIZE,
                             (void          * )0,
                             (uint16_t        )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
	if(os_err == OS_ERR_NONE)
	{
		OSTaskNameSet(APP_TASK0_PRIO, (uint8_t *)"Start Task", &os_err); //给开始任务命名。
	}
	
    OSStart(); //开始UCOS内核。
	
	return 0;
}

/* 最先开始的任务（开始任务）*/
static void App_Task0(void *p_arg) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	(void) p_arg;
	
	OS_CPU_SysTickInit();	//设置SysTick,并允许中断。

#if (OS_TASK_STAT_EN > 0)
    OSStatInit(); //初始任务统计函数。
#endif
		
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE); //open GPIOD's clock.
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_DOWN;
	GPIO_Init(GPIOD, &GPIO_InitStructure); //push-pull_pull-down_50MHz output.
	
	while(1)
	{			
		GPIO_WriteBit(GPIOD, GPIO_Pin_12, Bit_SET);
		OSTimeDly(500);
		GPIO_WriteBit(GPIOD, GPIO_Pin_12, Bit_RESET);
		GPIO_WriteBit(GPIOD, GPIO_Pin_13, Bit_SET);
		OSTimeDly(500);
		GPIO_WriteBit(GPIOD, GPIO_Pin_13, Bit_RESET);
		GPIO_WriteBit(GPIOD, GPIO_Pin_14, Bit_SET);
		OSTimeDly(500);
		GPIO_WriteBit(GPIOD, GPIO_Pin_14, Bit_RESET);
		GPIO_WriteBit(GPIOD, GPIO_Pin_15, Bit_SET);
		OSTimeDly(500); //Clockwise light four LED.			
		GPIO_WriteBit(GPIOD, GPIO_Pin_15, Bit_RESET);
		
	}	
}	

