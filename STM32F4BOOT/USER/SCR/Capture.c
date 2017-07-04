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
#include <stdio.h>
#include <stdarg.h>
#include "stm32f4xx.h"
#include "ucos_ii.h"
#include "mystd.h"
#include "m_config.h"
#include "InitHW.h"
#include "UART.h"

static __IO uint16_t IC3ReadValue1 = 0, IC3ReadValue2 = 0;
static __IO uint16_t CaptureNumber = 0;
static __IO uint32_t Capture = 0;
static __IO uint32_t TIM1Freq = 0;


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
void Capture_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    /* TIM1 clock enable */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);

    /* GPIOA clock enable */
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

    /* TIM1 channel 2 pin (PE.11) configuration */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_11;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    /* Connect TIM pins to AF2 */
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_TIM1);

    /* Enable the TIM1 global Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = TIM1_CC_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
  
}


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
void InitCapturePin(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;


    /* TIM1 channel 2 pin (PE.11) configuration */
    GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
    GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
    GPIO_Init(GPIOB, &GPIO_InitStructure);

    /* Connect TIM pins to AF2 */
    GPIO_PinAFConfig(GPIOB, GPIO_PinSource10, GPIO_AF_TIM2);


  
}

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
void InitCapture(void)
{
    TIM_ICInitTypeDef  TIM_ICInitStructure;
    
    
    Capture_Config();

    /* TIM1 configuration: Input Capture mode ---------------------
     The external signal is connected to TIM1 CH2 pin (PE.11)  
     The Rising edge is used as active edge,
     The TIM1 CCR2 is used to compute the frequency value 
    ------------------------------------------------------------ */

    TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
    TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
    TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
    TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
    TIM_ICInitStructure.TIM_ICFilter = 0x0;

    TIM_ICInit(TIM1, &TIM_ICInitStructure);

    /* TIM enable counter */
    TIM_Cmd(TIM1, ENABLE);

    /* Enable the CC2 Interrupt Request */
    TIM_ITConfig(TIM1, TIM_IT_CC2, ENABLE);

     
}

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
void TIM1_CC_IRQHandler(void)
{ 
    if(TIM_GetITStatus(TIM1, TIM_IT_CC2) == SET) 
    {
        /* Clear TIM1 Capture compare interrupt pending bit */
        TIM_ClearITPendingBit(TIM1, TIM_IT_CC2);
        if(CaptureNumber == 0)
        {
            /* Get the Input Capture value */
            IC3ReadValue1 = TIM_GetCapture2(TIM1);
            CaptureNumber = 1;
        }
        else if(CaptureNumber == 1)
        {
            /* Get the Input Capture value */
            IC3ReadValue2 = TIM_GetCapture2(TIM1); 

            /* Capture computation */
            if (IC3ReadValue2 > IC3ReadValue1)
            {
                Capture = (IC3ReadValue2 - IC3ReadValue1); 
            }
            else if (IC3ReadValue2 < IC3ReadValue1)
            {
                Capture = ((0xFFFF - IC3ReadValue1) + IC3ReadValue2); 
            }
            else
            {
                Capture = 0;
            }
            
            /* Frequency computation */ 
            TIM1Freq = (uint32_t) SystemCoreClock / Capture;
            CaptureNumber = 0;
            
        }
        
    }
}

