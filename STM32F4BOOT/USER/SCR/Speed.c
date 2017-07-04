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
#include "mystd.h"
#include "m_config.h"
#include "Speed.h"
#include "Timer.h"
#include "DigitRP.h"
#include "SysAppConfig.h"
#include "ExINT.h"


#define  US_P_MIN   (1000*1000*60)
/*************************************************************************************************
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
*************************************************************************************************/
void  InitSpeedIO(void)
{
    
    GPIO_InitTypeDef GPIO_InitStructure;
    EXTI_InitTypeDef EXTI_InitStructure;
    //NVIC_InitTypeDef NVIC_InitStructure;
    
    
    //配置PA6为外中断
    //C2PLD_IN_PS0
    //使能GPIOA的AHB1时钟 
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA|RCC_AHB1Periph_GPIOB|RCC_AHB1Periph_GPIOE ,ENABLE);                            

    /* Configure RIGHTB_IN1_NUM/RIGHTB_IN0_NUM (PE.01/PE.0) as alternate function push-pull */
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    
    
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_7;
    GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    //GPIO_InitStructure.GPIO_Mode  = GPIO_PuPd_UP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
          
          
    
    /* Enable SYSCFG clock */
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

    EXTI_ClearITPendingBit(EXTI_Line6);
    EXTI_ClearITPendingBit(EXTI_Line7);

    /* Connect EXTI Line to PC pin */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource6);
    
    /* Connect EXTI Line to PC pin */
    SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOA, EXTI_PinSource7);

    /* Enable the EXTI Line6 Interrupt */
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Line    = EXTI_Line6;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
    
    /* Enable the EXTI Line7 Interrupt */
    EXTI_InitStructure.EXTI_Mode    = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Line    = EXTI_Line7;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);
 

//     /* Configure the Priority Group to 2 bits */
//     NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

//     /* Enable the EXTI9_5_IRQHandler Interrupt */
//     NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
//     NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EXTI9_5_IRQ_PRIO;
//     NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
//     NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
//     NVIC_Init(&NVIC_InitStructure);

       InitExINT9_5_NVIC();

     
    
    
    
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
#if 0
void EXTI9_5_IRQHandler_XXX(void)
{
    static uint32_t   speed0_pre_tm_cnt = 0;
    static uint32_t   speed1_pre_tm_cnt = 0;
    
    uint32_t   angle_set_v;
    uint32_t   cur_tm_cnt;
    uint32_t   us_p_rev;
    

    cur_tm_cnt =  TIM_GetCounter(TIM5);
    
    //回测角度
    if(EXTI_GetITStatus(EXTI_Line6)!= RESET)
    {
        if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6))
        {
            //(cur_tm_cnt <=  speed0_pre_tm_cnt)?(SpeedTestCnt = TIM5_CNT - speed0_pre_tm_cnt  + cur_tm_cnt) 
            
            SpeedTestCnt = (TIM5_CNT + (int)(cur_tm_cnt- speed0_pre_tm_cnt)) % TIM5_CNT;//计数绕回处理
            speed0_pre_tm_cnt = cur_tm_cnt;     
        }
        else
        {
            AngleTmCnt = (TIM5_CNT + (int)(cur_tm_cnt - speed0_pre_tm_cnt)) % TIM5_CNT; //计数绕回处理

                                    
        }
        
        
        EXTI_ClearITPendingBit(EXTI_Line6);
        
    } 
    
    //测速并产生角度脉宽
    if(EXTI_GetITStatus(EXTI_Line7)!= RESET)
    {
        if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7))
        {
            if(cur_tm_cnt <= speed1_pre_tm_cnt)
            {
                us_p_rev = TIM5_CNT - speed1_pre_tm_cnt  + cur_tm_cnt;
                SpeedRPM = US_P_MIN /(us_p_rev);
                
                angle_set_v = (us_p_rev / 360) * AngleSet;                
            }
            else
            {
                us_p_rev = cur_tm_cnt - speed1_pre_tm_cnt;
                SpeedRPM = US_P_MIN /(us_p_rev) ;  
                
                angle_set_v = ((us_p_rev) / 360) * AngleSet;               
            }
          
            if (SpeedRPM > MAX_REV)
            {
                SpeedRPM = 0;   //无效值
            }           
            else if (AngleRun)
            {              
                if (angle_set_v != 0)
                {    
                    TIM_SetAutoreload(TIM2, us_p_rev);
                    TIM_SetCompare2(TIM2,angle_set_v);
                    TIM_OC2FastConfig(TIM2, ENABLE);
                }             
              
            }  
          
          speed1_pre_tm_cnt = cur_tm_cnt;
          
        }
    
        EXTI_ClearITPendingBit(EXTI_Line7); 
        RPM_0_Flg = 0;        
        
    }
    
    //OS_CPU_SR  cpu_sr = 0;

    //OS_ENTER_CRITICAL();
    
    if(EXTI_GetITStatus(EXTI_Line8)!= RESET)
    {
         if (AppData.digit_rp_en)
         {
             DigitRP(LEFT_DRP);
         }	

         EXTI_ClearITPendingBit(EXTI_Line8);
     
    }
    
    
    //OS_EXIT_CRITICAL();
    
    //OSIntExit(); 
    


}
#endif

/*
void EXTI9_5_IRQHandler11(void)
{
    static uint32_t   speed0_pre_tm_cnt = 0;
    static uint32_t   speed1_pre_tm_cnt = 0;
    
    uint32_t   cur_tm_cnt;
    

    cur_tm_cnt =  TIM_GetCounter(TIM5);
    
    if(EXTI_GetITStatus(EXTI_Line6)!= RESET)
    {
        if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6))
        {
            if(cur_tm_cnt <= speed0_pre_tm_cnt)
            {
                SpeedRPM = US_P_MIN /(TIM5_CNT - speed0_pre_tm_cnt  + cur_tm_cnt);
                SpeedTimCount = ((TIM5_CNT - speed0_pre_tm_cnt  + cur_tm_cnt) / 360) * AngleSet;                
            }
            else
            {
                SpeedRPM = US_P_MIN /(cur_tm_cnt - speed0_pre_tm_cnt) ;  
                SpeedTimCount = ((cur_tm_cnt - speed0_pre_tm_cnt) / 360) * AngleSet;               
            }
                   
            TIM_SetCompare2(TIM2,SpeedTimCount);
            speed0_pre_tm_cnt = cur_tm_cnt;
        }
        else
        {
            
             PulseTimCount = cur_tm_cnt - speed0_pre_tm_cnt; 
            
        }
        
        EXTI_ClearITPendingBit(EXTI_Line6);
        
    } 
    
    //测速并产生角度脉宽
    if(EXTI_GetITStatus(EXTI_Line7)!= RESET)
    {
        if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7))
        {
            if(cur_tm_cnt <= speed1_pre_tm_cnt)
            {
                SpeedRPM = US_P_MIN /(TIM5_CNT - speed1_pre_tm_cnt  + cur_tm_cnt);
                SpeedTimCount = ((TIM5_CNT - speed1_pre_tm_cnt  + cur_tm_cnt) / 360) * AngleSet;                
            }
            else
            {
                SpeedRPM = US_P_MIN /(cur_tm_cnt - speed0_pre_tm_cnt) ;  
                SpeedTimCount = ((cur_tm_cnt - speed1_pre_tm_cnt) / 360) * AngleSet;               
            }
                   
            TIM_SetCompare2(TIM2,SpeedTimCount);
            speed1_pre_tm_cnt = cur_tm_cnt;
        }
    
        EXTI_ClearITPendingBit(EXTI_Line7);        
        
    }


}

*/


