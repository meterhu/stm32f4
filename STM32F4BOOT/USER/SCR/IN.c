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
 *******************************************************************************************************/
//======================================================================================================
#include "stm32f4_std_libs.h"
#include "In.h"

 /***************************************************************************
*��������INx_Init(void) 
*������void
*����ֵ:void
*���ܣ�EXTI��ʼ��
*****************************************************************************/
 void  INx_Init(void)
 {	   
   IN_Configuration();
   IN_NVIC_Config();
 }

/***************************************************************************
*��������IN_Configuration(void)
*������void
*����ֵ:void
*���ܣ�EXTI����
*****************************************************************************/
void IN_Configuration(void)
{
    
  GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	/* GPIOA enable */
  RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE , ENABLE);
	/* Enable SYSCFG clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
    
	/* Configure (PE.0) as input floating */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_Init(GPIOE, &GPIO_InitStructure);
   
	EXTI_ClearITPendingBit(EXTI_Line1);
	EXTI_ClearITPendingBit(EXTI_Line2);
	EXTI_ClearITPendingBit(EXTI_Line3);
	EXTI_ClearITPendingBit(EXTI_Line4);

  /* Connect EXTI Line0 to PA0 pin */
  SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource1);
	
	/* Enable the EXTI Line1 Interrupt */
  EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Line = EXTI_Line1;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

}

/**************************************************************************
*
*��������: EXTI_NVIC_Config(void)
*������void
*����ֵ:void
*���ܣ�EXTI��������
*****************************************************************************/
 void IN_NVIC_Config(void)
 {
  NVIC_InitTypeDef NVIC_InitStructure;
  
#ifdef DEBUG_IN_RAM 
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x00);
#else
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x00);
#endif 

 	 /* Configure the Priority Group to 2 bits */
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
  
  /* Enable the EXTI1 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable the EXTI2 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure);

    /* Enable the EXTI3 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure);
  
  /* Enable the EXTI4 Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
  NVIC_Init(&NVIC_InitStructure);
 
 }


/****************************************************************************
*
*��������: EXTI1_IRQHandler(void)
*������void
*����ֵ:void
*���ܣ�EXTI�жϷ�����
*****************************************************************************/
void EXTI1_IRQHandler(void)
{
    if(EXTI_GetITStatus(EXTI_Line0)!= RESET)
	 {	
//		Tim0 = TIM_GetCounter(TIM3);//��ȡʱ��
//        //TIM_SetCounter(TIM4, 0x0);
//		if(Tim0>Tim3)
//	     {
//	  	    Exti_0_Num++;	        
//	     }
////	    Exti_0_Num++;
////	    EXTIsecIntFlg=1;	    
////	    EXTITempNum0=Exti_0_Num;
//
//
//		if(Exti_0_Num>200 || Exti_0_Num<0)
//	     {
//			   	Exti_0_Num=0;
//				TIM_SetAutoreload(TIM3, 0x00);  
//		  }
//		  else 
//		  {
//			     TIM_SetAutoreload(TIM3, Exti_0_Num*100);
//		   }

	    EXTI_ClearITPendingBit(EXTI_Line1);
	 }

	 
 	 
}







