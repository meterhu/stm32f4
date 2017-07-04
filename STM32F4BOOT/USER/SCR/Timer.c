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
 *                ����: ������
 *
 *******************************************************************************************************/
//======================================================================================================
#include "stm32f4xx.h"
#include "stm32f4_std_libs.h"
#include "mystd.h"
#include "m_config.h"
#include "UserLib.h"
//#include "ucos_ii.h"
#include "Timer.h"
#include "RevAdvAngle.h"

     
uint32_t  Timer1Cnt = 0;


uint16_t CCR1_Val = 333;
uint16_t CCR2_Val = 2100;
uint16_t CCR3_Val = 166;
uint16_t CCR4_Val = 83;
uint16_t PrescalerValue = 0;

volatile uint32_t  Timer2Cnt = 0;
volatile uint32_t  Timer4Cnt = 0;
volatile uint32_t  Timer5Cnt = 0;

uint32_t SysTimeTickCnt = 0;
uint32_t FirmwareUpdateCnt = 0;

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
/*************************************************************************************************
*
*         ��������: TIM_Configuration
*
*         ��������: 
*
*         ��ڲ���: 
*
*         ���ڲ���: ��
*
*             ˵��:
*
*************************************************************************************************/
void TIM_Configuration11(void)
{
    /* ---------------------------------------------------------------
    TIM2��3��4��ʱ��Դ�� APB1 ���� PCLK1  ( APB1 ��Ӧ PCLK1 )
    PCLK1 = APB1 = HCLK/2 = SYSCLK/2 = 36MHZ (36,000,000 HZ)
    
    ע�⣺������һ��Ƚ����⡣
    ��� APB��Ƶϵ����Ҳ���Ǻ���RCC_PCLK1Config(RCC_HCLK_Div2);��Ϊ1��
    ��PCLK1*1������PCLK1*2��
    �˴�PCLK1*2;��Ϊ72MHZ
    
    TIM2 Configuration: Output Compare Timing Mode:
    TIM2CLK = 72 MHz, Prescaler(TIM_Prescaler) = 2     
    
    TIM2 counter clock = TIM2CLK/(TIM_Prescaler+1) =24MHz
    TIM2 update rate = TIM2 counter clock / TIM_Period = 1000 Hz
    --------------------------------------------------------------- */
    
    
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    
    /*����1����1΢��*/
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 60000;
    TIM_TimeBaseStructure.TIM_Prescaler = 71;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    /* TIM IT enable */
    //TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
    
    /* TIM2 enable counter */
    TIM_Cmd(TIM2, ENABLE);
    
    
    /*1000,����1����1΢��*/
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period = 1000;
    TIM_TimeBaseStructure.TIM_Prescaler = 71;
    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
    
    TIM_ITConfig(TIM1, TIM_IT_Update, ENABLE);
    /* TIM1 enable counter */
    TIM_Cmd(TIM1, ENABLE);
    
    
}



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
void TIM1_UP_IRQHandler(void)
{
    ///if (TIM_GetITStatus(TIM1, TIM_IT_Update) != RESET)
    {
        /* Read data from SPI flash and set it as TIM4 Ch3 Pulse */
    
        Timer1Cnt++;
        /* Clear TIM2 update interrupt */
        TIM_ClearITPendingBit(TIM1, TIM_IT_Update);
    }

}

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
void  InitTimer2(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    //TIM_ICInitTypeDef  TIM_ICInitStructure;
    RCC_ClocksTypeDef rcc_clk;
    /*
    TIM3 Configuration: Output Compare Timing Mode:
    
    In this example TIM3 input clock (TIM3CLK) is set to 2 * APB1 clock (PCLK1), 
    since APB1 prescaler is different from 1.   
      TIM3CLK = 2 * PCLK1  
      PCLK1 = HCLK / 4 
      => TIM3CLK = HCLK / 2 = SystemCoreClock /2
          
    To get TIM3 counter clock at 50 MHz, the prescaler is computed as follows:
       Prescaler = (TIM3CLK / TIM3 counter clock) - 1
       Prescaler = ((SystemCoreClock /2) /50 MHz) - 1
    */
     /* ---------------------------------------------------------------
    TIM2��3��4��ʱ��Դ�� APB1 ���� PCLK1  ( APB1 ��Ӧ PCLK1 )
    PCLK1 = APB1 = HCLK/2 = SYSCLK/2 = 36MHZ (36,000,000 HZ)
    
    ע�⣺������һ��Ƚ����⡣
    ��� APB��Ƶϵ����Ҳ���Ǻ���RCC_PCLK1Config(RCC_HCLK_Div2);��Ϊ1��
    ��PCLK1*1������PCLK1*2��
    �˴�PCLK1*2;��Ϊ72MHZ
    
    TIM2 Configuration: Output Compare Timing Mode:
    TIM2CLK = 72 MHz, Prescaler(TIM_Prescaler) = 2     
    
    TIM2 counter clock = TIM2CLK/(TIM_Prescaler+1) =24MHz
    TIM2 update rate = TIM2 counter clock / TIM_Period = 1000 Hz
    --------------------------------------------------------------- */

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
    
    //TIM3CLK = 2 * PCLK1    PCLK1 = HCLK / 4 => TIM3CLK = HCLK / 2 = SystemCoreClock /2
    //��TIM3CLK 84MHz(168Mhz/2)
    RCC_GetClocksFreq(&rcc_clk);
    if ((rcc_clk.SYSCLK_Frequency / rcc_clk.PCLK1_Frequency) == 1)
    {
        TIM_TimeBaseStructure.TIM_Prescaler = (rcc_clk.PCLK1_Frequency / __MILLION); 
    }
    else
    {
        TIM_TimeBaseStructure.TIM_Prescaler = (rcc_clk.PCLK1_Frequency / __MILLION) * 2; 
    }
    
    //if (PCLK1 == HCLK / 4) TIM_TimeBaseStructure.TIM_Prescaler = 83; 

    //����1����1΢��
    TIM_TimeBaseStructure.TIM_Period = 10000; //1us(��Ƶ��Ϊ1Mhz,����������)  1000 * 1000 * 5;  //

    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    /* Enable the TIM2 gloabal Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIMER2_IRQ_PRIO;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    /* TIM IT enable */
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
        
    /* TIM2 enable counter */
    TIM_Cmd(TIM2, ENABLE);
    


}


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
void InitTimer3(void)
{
  
     /* -----------------------------------------------------------------------
    TIM3 Configuration: Output Compare Timing Mode:
    
    In this example TIM3 input clock (TIM3CLK) is set to 2 * APB1 clock (PCLK1), 
    since APB1 prescaler is different from 1.   
      TIM3CLK = 2 * PCLK1  
      PCLK1 = HCLK / 4 
      => TIM3CLK = HCLK / 2 = SystemCoreClock /2
          
    To get TIM3 counter clock at 50 MHz, the prescaler is computed as follows:
       Prescaler = (TIM3CLK / TIM3 counter clock) - 1
       Prescaler = ((SystemCoreClock /2) /50 MHz) - 1
                                              
    CC1 update rate = TIM3 counter clock / CCR1_Val = 9.154 Hz
    ==> Toggling frequency = 4.57 Hz
    
    C2 update rate = TIM3 counter clock / CCR2_Val = 18.31 Hz
    ==> Toggling frequency = 9.15 Hz
    
    CC3 update rate = TIM3 counter clock / CCR3_Val = 36.62 Hz
    ==> Toggling frequency = 18.31 Hz
    
    CC4 update rate = TIM3 counter clock / CCR4_Val = 73.25 Hz
    ==> Toggling frequency = 36.62 Hz

    Note: 
     SystemCoreClock variable holds HCLK frequency and is defined in system_stm32f4xx.c file.
     Each time the core clock (HCLK) changes, user had to call SystemCoreClockUpdate()
     function to update SystemCoreClock variable value. Otherwise, any configuration
     based on this variable will be incorrect.    
  ----------------------------------------------------------------------- */ 
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_OCInitTypeDef  TIM_OCInitStructure;
    RCC_ClocksTypeDef rcc_clk;
    NVIC_InitTypeDef NVIC_InitStructure;

    ///uint16_t PrescalerValue = 0;    
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);


    RCC_GetClocksFreq(&rcc_clk);   
    /* Time base configuration */
    TIM_TimeBaseStructure.TIM_Period        = 2000; //10us
    if ((rcc_clk.SYSCLK_Frequency / rcc_clk.PCLK1_Frequency) == 1)
    {
        TIM_TimeBaseStructure.TIM_Prescaler = (rcc_clk.PCLK1_Frequency / __MILLION); 
    }
    else
    {
        TIM_TimeBaseStructure.TIM_Prescaler = (rcc_clk.PCLK1_Frequency / __MILLION) * 2; 
    }
    
    TIM_TimeBaseStructure.TIM_Prescaler *= 10;

    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    
        /* Enable the TIM2 gloabal Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIMER3_IRQ_PRIO;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
//     //     //PA0---PWM0
    /* PWM1 Mode configuration: Channel1 */
    TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
    TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
    TIM_OCInitStructure.TIM_Pulse = 300;
    TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;

    TIM_OC4Init(TIM3, &TIM_OCInitStructure);

    TIM_OC4PreloadConfig(TIM3, TIM_OCPreload_Enable);

    TIM_ARRPreloadConfig(TIM3, ENABLE); 
    
    /* TIM Interrupts enable */
    ///TIM_ITConfig(TIM3, TIM_IT_CC1 | TIM_IT_CC2 | TIM_IT_CC3 | TIM_IT_CC4, ENABLE);
    //TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);
    /* TIM3 enable counter */
    TIM_Cmd(TIM3, ENABLE);
    
    

}

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
void  InitTimer4(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_ClocksTypeDef rcc_clk;

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
    
    //TIM3CLK = 2 * PCLK1    PCLK1 = HCLK / 4 => TIM3CLK = HCLK / 2 = SystemCoreClock /2
    //��TIM3CLK 84MHz(168Mhz/2)
    RCC_GetClocksFreq(&rcc_clk);
    
    if ((rcc_clk.SYSCLK_Frequency / rcc_clk.PCLK1_Frequency) == 1)
    {
        TIM_TimeBaseStructure.TIM_Prescaler = (rcc_clk.PCLK1_Frequency / __MILLION); 
    }
    else
    {
        TIM_TimeBaseStructure.TIM_Prescaler = (rcc_clk.PCLK1_Frequency / __MILLION) * 2; 
    }
    //if (PCLK1 == HCLK / 4) TIM_TimeBaseStructure.TIM_Prescaler = 83; 

    //����1����1΢��
    TIM_TimeBaseStructure.TIM_Period = 20000; //1us(��Ƶ��Ϊ1Mhz,����������)

    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);
    
    /* Enable the TIM2 gloabal Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIMER_IRQ_SUB1;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIMER_IRQ_SUB3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
        
     /* TIM IT enable */
     TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);
        
    /* TIM2 enable counter */
    TIM_Cmd(TIM4, ENABLE);

}

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
void  InitTimer5(void)
{
    ///NVIC_InitTypeDef NVIC_InitStructure;
    //TIM_ICInitTypeDef  TIM_ICInitStructure;
    RCC_ClocksTypeDef rcc_clk;
    ///TIM_OCInitTypeDef  TIM_OCInitStructure;
    /*
    TIM3 Configuration: Output Compare Timing Mode:
    
    In this example TIM3 input clock (TIM3CLK) is set to 2 * APB1 clock (PCLK1), 
    since APB1 prescaler is different from 1.   
      TIM3CLK = 2 * PCLK1  
      PCLK1 = HCLK / 4 
      => TIM3CLK = HCLK / 2 = SystemCoreClock /2
          
    To get TIM3 counter clock at 50 MHz, the prescaler is computed as follows:
       Prescaler = (TIM3CLK / TIM3 counter clock) - 1
       Prescaler = ((SystemCoreClock /2) /50 MHz) - 1
    */
     /* ---------------------------------------------------------------
    TIM2��3��4��ʱ��Դ�� APB1 ���� PCLK1  ( APB1 ��Ӧ PCLK1 )
    PCLK1 = APB1 = HCLK/2 = SYSCLK/2 = 36MHZ (36,000,000 HZ)
    
    ע�⣺������һ��Ƚ����⡣
    ��� APB��Ƶϵ����Ҳ���Ǻ���RCC_PCLK1Config(RCC_HCLK_Div2);��Ϊ1��
    ��PCLK1*1������PCLK1*2��
    �˴�PCLK1*2;��Ϊ72MHZ
    
    TIM2 Configuration: Output Compare Timing Mode:
    TIM2CLK = 72 MHz, Prescaler(TIM_Prescaler) = 2     
    
    TIM2 counter clock = TIM2CLK/(TIM_Prescaler+1) =24MHz
    TIM2 update rate = TIM2 counter clock / TIM_Period = 1000 Hz
    --------------------------------------------------------------- */

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
    
    //TIM3CLK = 2 * PCLK1    PCLK1 = HCLK / 4 => TIM3CLK = HCLK / 2 = SystemCoreClock /2
    //��TIM3CLK 84MHz(168Mhz/2)
    RCC_GetClocksFreq(&rcc_clk);
    if ((rcc_clk.SYSCLK_Frequency / rcc_clk.PCLK1_Frequency) == 1)
    {
        TIM_TimeBaseStructure.TIM_Prescaler = (rcc_clk.PCLK1_Frequency / __MILLION); 
    }
    else
    {
        TIM_TimeBaseStructure.TIM_Prescaler = (rcc_clk.PCLK1_Frequency / __MILLION) * 2; 
    }
    
    //if (PCLK1 == HCLK / 4) TIM_TimeBaseStructure.TIM_Prescaler = 83; 

    //����1����1΢��
    ////TIM_TimeBaseStructure.TIM_Period = 2000; //1us(��Ƶ��Ϊ1Mhz,����������)
    
    //����1����1΢��
    TIM_TimeBaseStructure.TIM_Period = 1000 * 1000 * 60; //1us(��Ƶ��Ϊ1Mhz,����������)

    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure);
    
//     /* Enable the TIM2 gloabal Interrupt */
//     NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
//     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIMER_IRQ_PRIO;
//     NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIMER_IRQ_SUB1;
//     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//     NVIC_Init(&NVIC_InitStructure);
//     
//   
//     
//     /* TIM IT enable */
//     TIM_ITConfig(TIM5, TIM_IT_Update, ENABLE);
        
    /* TIM2 enable counter */
    TIM_Cmd(TIM5, ENABLE);

}

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
void  InitTimer7(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    RCC_ClocksTypeDef rcc_clk;

    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure; 

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM7, ENABLE);
    
    //TIM3CLK = 2 * PCLK1    PCLK1 = HCLK / 4 => TIM3CLK = HCLK / 2 = SystemCoreClock /2
    //��TIM3CLK 84MHz(168Mhz/2)
    RCC_GetClocksFreq(&rcc_clk);
    if ((rcc_clk.SYSCLK_Frequency / rcc_clk.PCLK1_Frequency) == 1)
    {
        TIM_TimeBaseStructure.TIM_Prescaler = (rcc_clk.PCLK1_Frequency / __MILLION); 
    }
    else
    {
        TIM_TimeBaseStructure.TIM_Prescaler = (rcc_clk.PCLK1_Frequency / __MILLION) * 2; 
    } 
    
    //if (PCLK1 == HCLK / 4) TIM_TimeBaseStructure.TIM_Prescaler = 83; 

    //����1����1΢��
    TIM_TimeBaseStructure.TIM_Period = 10000; //10ms(��Ƶ��Ϊ1Mhz,����������)

    TIM_TimeBaseStructure.TIM_ClockDivision = 0;
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
    
    TIM_TimeBaseInit(TIM7, &TIM_TimeBaseStructure);
    
    /* Enable the TIM2 gloabal Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM7_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = TIMER_IRQ_PRIO;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = TIMER_IRQ_SUB2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
        
    /* TIM IT enable */
    TIM_ITConfig(TIM7, TIM_IT_Update, ENABLE);
        
    /* TIM2 enable counter */
    TIM_Cmd(TIM7, ENABLE);

}



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
void TIM2_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        Timer2Cnt++;
        FirmwareUpdateCnt++;
        TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
    }
}



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
void TIM4_IRQHandler(void)
{
    
    if (TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
    {
        TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
    }
    
}

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
void TIM5_IRQHandler(void)
{    
    if (TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
    {

        Timer5Cnt++;
        TIM_ClearITPendingBit(TIM5, TIM_IT_Update);
    }
    

}


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
void TIM7_IRQHandler(void)
{    
    if (TIM_GetITStatus(TIM7, TIM_IT_Update) != RESET)
    {
        SysTimeTickCnt++;
        
        TIM_ClearITPendingBit(TIM7, TIM_IT_Update);
    }
       

}



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
void TIM3_IRQHandler(void)
{
    if (TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
    {
        Timer2Cnt++;
        
        TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
    }
    
}







