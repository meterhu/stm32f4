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
 *                作者: 杨卫华
 *
 *******************************************************************************************************/
//======================================================================================================
#include "stm32f4xx.h"
#include "ucos_ii.h"
#include <stdio.h>
#include <stdarg.h>
#include "mystd.h"  
#include "m_config.h"
#include "INOut.h"
#include "UserLib.h"
#include "DigitRP.h"
#include "tasks.h"
#include "SysAppConfig.h"
#include "main.h"

//-------------------------------------------------------------------------------------------------------
/*
GPIO map and define
  
  name                  IO              DIR
----------            --------        -------
RIGHTB_IN1             P0.23            IN
RIGHTB_IN0             P0.24            IN
LEFTB_IN1              P0.25            IN
LEFTB_IN0              P0.26            IN
*/
//-------------------------------------------------------------------------------------------------------

static uint16_t  OutBitState;
static uint16_t  INBitState;
extern OS_EVENT    *INOUT_MBOX;



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
void  InitInputIO(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    
    //使能GPIOA的AHB1时钟 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE ,ENABLE);                            

    //PE3-6
    //IN0    PE6
    //IN1    PE5
    //IN2    PE4
    //IN3    PE3
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_3|GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_Init(GPIOE, &GPIO_InitStructure);   
     
    //Enable SYSCFG clock 
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    EXTI_ClearITPendingBit(EXTI_Line3);
    EXTI_ClearITPendingBit(EXTI_Line4);

    //Connect EXTI Line to PE pin 
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource3);
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource4);

    //Enable the EXTI Line3-6 Interrupt
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Line    = EXTI_Line3|EXTI_Line4;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);


    
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
void InitINOut(void)
{
     OutBitState = 0;
         


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
void OutH(uint8_t out_bit_num)
{
    if (!(OutBitState & (0x01 << out_bit_num)))
    {
        //开出
        //OUT_WARM_UP();
        OutBitState &= ~(0x1 << (out_bit_num));
        OutBitState |= (0x1 << (out_bit_num));

        
        OSMboxPost(INOUT_MBOX, (void *)OutBitState);

    }

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
void OutL(uint8_t out_bit_num)
{
    if (OutBitState & (0x1 << (out_bit_num)))
    {
        //开出
        //OUT_WARM_UP();
        OutBitState &= ~(0x1 << (out_bit_num));
        //OutBitState |= ~(0x1 << (out_bit_num));

        OSMboxPost(INOUT_MBOX, (void *)OutBitState);

    }

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
void Out(uint8_t out_bit_num)
{
//    if ((OutBitState >> out_bit_num) ^ new_state)
//    {
//        //开出
//        //OUT_WARM_UP();
//        OutBitState &= ~(0x1 << (out_bit_num));
//        //OutBitState |= ~(0x1 << (out_bit_num));
//
//        OSMboxPost(INOUT_MBOX, (void *)OutBitState);
//
//    }

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
void IN(uint8_t in_bit_num, uint8_t new_state)
{
    //if (AppData.io.i16_o16.in_state ^ (0x1 << (in_bit_num)))
    {
        //开出
        //OUT_WARM_UP();
        INBitState &= ~(0x1 << (in_bit_num));
        INBitState |=  (new_state << (in_bit_num));

        OSMboxPost(INOUT_MBOX, (void *)INBitState);

    }

}

