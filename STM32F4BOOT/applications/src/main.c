//======================================================================================================
/******************************************************************************************************
 *                                                                                                     
 *            �ļ�����:              
 *
 *                ժҪ: 
 *
 *            ����ʱ��: 2012-05-xx  
 *
 *            �޸�ʱ��: 2012-05-xx
 *            �޸�ԭ��:
 *
 *            
 *
 *                ����: 
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
 *         ��������: 
 *
 *         ��������: 
 *
 *         ��ڲ���: 
 *
 *         ���ڲ���: ��
 *
 *             ˵��:
 *
 *******************************************************************************************************/
int main1(void)
{	
	uint8_t os_err;
    
    SystemInit();
    
    GPIO_Config();
    InitUART(USART2,115200);
    printf("program is running\n");
	
	__disable_irq(); //��ֹ�����жϡ�

	
	OSInit(); //��ʼ��uC/OS-II���ʵʱ�ںˡ�

	os_err = OSTaskCreateExt((void (*)(void *)) App_Task0, //����һ����ʼ����
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
		OSTaskNameSet(APP_TASK0_PRIO, (uint8_t *)"Start Task", &os_err); //����ʼ����������
	}
	
    OSStart(); //��ʼUCOS�ںˡ�
	
	return 0;
}

/* ���ȿ�ʼ�����񣨿�ʼ����*/
static void App_Task0(void *p_arg) 
{
	GPIO_InitTypeDef GPIO_InitStructure;
	(void) p_arg;
	
	OS_CPU_SysTickInit();	//����SysTick,�������жϡ�

#if (OS_TASK_STAT_EN > 0)
    OSStatInit(); //��ʼ����ͳ�ƺ�����
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

