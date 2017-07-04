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
#include <stdio.h>
#include <stdarg.h>
#include "stm32f4xx.h"
#include "stm32f4_std_libs.h"
#include "ucos_ii.h"
#include "mystd.h"
#include "m_config.h"
#include "Speed.h"
#include "Timer.h"
#include "DigitRP.h"
#include "SysAppConfig.h"
#include "INOut.h"
#include "ExINT.h"
#include "pwm.h"
#include "RevAdvAngle.h"

uint32_t   SpeedRPM = 0;
volatile uint32_t   PulseTimCount = 0;
volatile uint32_t   AngleTmCnt = 0;
volatile uint32_t   SpeedTmCnt = 0;

volatile  uint32_t  SpeedTestCnt;
uint32_t  AngleSet;
int  AngleRun;
extern volatile  uint32_t  RPM_0_Flg;
extern uint32_t RevClearCnt;

extern OS_EVENT  *TS_SYN;
uint8_t PWMIOEna = 0;
#define  US_P_MIN   (1000*1000*60)

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
void InitExINT0_NVIC(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    //Configure the Priority Group to 2 bits 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    //Enable the EXTIx_IRQHandler Interrupt,LEFT_DRP
    NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EXTI_IRQ_PRIO;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = EXTI_IRQ_PRIO_SUB1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EXTI_IRQ_PRIO;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = EXTI_IRQ_PRIO_SUB1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);
    
    //Enable the EXTIx_IRQHandler Interrupt,RIGHT_DRP
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EXTI_IRQ_PRIO;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = EXTI_IRQ_PRIO_SUB1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);
    
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EXTI_IRQ_PRIO;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = EXTI_IRQ_PRIO_SUB1;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);
 
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
void InitExINT1_NVIC(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    //Configure the Priority Group to 2 bits 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    //Enable the EXTIx_IRQHandler Interrupt
    NVIC_InitStructure.NVIC_IRQChannel = EXTI1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EXTI_IRQ_PRIO;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = EXTI_IRQ_PRIO_SUB2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);
 
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
void InitExINT2_NVIC(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    //Configure the Priority Group to 2 bits 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    //Enable the EXTIx_IRQHandler Interrupt
    NVIC_InitStructure.NVIC_IRQChannel = EXTI2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EXTI_IRQ_PRIO;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = EXTI_IRQ_PRIO_SUB3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);
 
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
void InitExINT3_NVIC(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    //Configure the Priority Group to 2 bits 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    //Enable the EXTIx_IRQHandler Interrupt 
    NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EXTI_IRQ_PRIO;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = EXTI_IRQ_PRIO_SUB4;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);
 
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
void InitExINT4_NVIC(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;

    //Configure the Priority Group to 2 bits 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    //Enable the EXTIx_IRQHandler Interrupt 
    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EXTI_IRQ_PRIO;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = EXTI_IRQ_PRIO_SUB5;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);
 
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
void InitExINT9_5_NVIC(void)
{
    NVIC_InitTypeDef NVIC_InitStructure;
    
    //Configure the Priority Group to 2 bits 
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

    //Enable the EXTIx_IRQHandler Interrupt 
    NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = EXTI9_5_IRQ_PRIO;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  
    NVIC_Init(&NVIC_InitStructure);
    
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
void EXTI9_5_IRQHandler(void)
{
    static uint32_t   speed0_pre_tm_cnt = 0;
    static uint32_t   speed1_pre_tm_cnt = 0;
    
    uint32_t   angle_set_v;
    uint32_t   cur_tm_cnt;
    uint32_t   us_p_rev;
    
    ///OS_CPU_SR  cpu_sr = 0;
    //OS_ENTER_CRITICAL();
    ///OSIntNesting++;
    ///OS_EXIT_CRITICAL();
    
    cur_tm_cnt =  TIM_GetCounter(TIM5);
    
    //�ز�Ƕ�
    if(EXTI_GetITStatus(EXTI_Line6)!= RESET)
    {

        if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6))
        {
            //(cur_tm_cnt <=  speed0_pre_tm_cnt)?(SpeedTestCnt = TIM5_CNT - speed0_pre_tm_cnt  + cur_tm_cnt) 
            
            SpeedTestCnt = cur_tm_cnt- speed0_pre_tm_cnt;//�����ƻش���
            speed0_pre_tm_cnt = cur_tm_cnt; 
            ///printf("Speed=%d\n", SpeedTestCnt);					
        }
        else
        {
            AngleTmCnt = cur_tm_cnt - speed0_pre_tm_cnt; //�����ƻش���
		    //printf("Angle=%d, %d\n", AngleTmCnt, SpeedTestCnt);
                                 
        }
        
        
        EXTI_ClearITPendingBit(EXTI_Line6);
        
    } 
    
    //���ٲ������Ƕ�����
    if(EXTI_GetITStatus(EXTI_Line7)!= RESET)
    {
        if(!GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7)) //�½���
        {
            RevClearCnt = 0;
            
            if(cur_tm_cnt <= speed1_pre_tm_cnt)
            {
                us_p_rev = TIM5_CNT - speed1_pre_tm_cnt  + cur_tm_cnt;
                SpeedRPM = US_P_MIN /(us_p_rev);
                
                angle_set_v = (us_p_rev / 360) * AngleSet;                
            }
            else
            {
                us_p_rev = cur_tm_cnt - speed1_pre_tm_cnt;
                SpeedRPM = US_P_MIN /(us_p_rev);  
                
                angle_set_v = ((us_p_rev) / 360) * AngleSet;               
            }
          
            if (SpeedRPM > MAX_REV)
            {
                SpeedRPM = 0;   //��Чֵ
            }           
            else if (AngleRun)
            {              
                //�ڽ�������У����û�������"Run"ʱ,����AngleRun��λ
                //��"Angle Run"̧��ʱ,AngleRun=FALSE
                //������ĳ������ж�AngleRun��������PWM�������(ע���Ǽ�ʱ��)
                //����Ҫע��ر�PWMʱ�����:
                //1.ת��С��ĳ����С��ֵ��ת��Ϊ0(��Timer5�ж����ж�)
                //2.Angle Ruņ��(��������н�����Ӧ����)
                if ((SpeedRPM > MIN_REV)  && (angle_set_v > 0))
                {   
                    
                    if (!PWMIOEna)
                    {	
                        RestorePWMPin();
						PWMIOEna = !PWMIOEna;
									  
                    }
					else
					{
                        
                        TIM_SetAutoreload(TIM2, us_p_rev);
                        TIM_SetCompare2(TIM2,angle_set_v);
                        TIM_OC2FastConfig(TIM2, ENABLE);
                        TIM_Cmd(TIM2, ENABLE);
					}
                } 
                else  //ת��̫��ʱ���ر�PWM
                { 
                    TIM_Cmd(TIM2, DISABLE);
                    ForcePWMOutLow();
				    PWMIOEna = 0;
                    
                }
                          
              
            }  
            
            //RevCnt(&SpeedRPM); //���ת�����Ϊ4000/min,��ô����Ϣ���15msͶ��һ��
            OilCnt();  //���ͼ���
            speed1_pre_tm_cnt = cur_tm_cnt;
          
        }
    
        EXTI_ClearITPendingBit(EXTI_Line7); 
        RPM_0_Flg = 0;        
        
    }       
    
    /////OSIntExit(); 
    
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
void EXTI0_IRQHandler(void)
{
  
    OS_CPU_SR  cpu_sr = 0;
    OS_ENTER_CRITICAL();
    OSIntNesting++;
    OS_EXIT_CRITICAL();   
    
    if(EXTI_GetITStatus(EXTI_Line0)!= RESET)
    {
        if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_0))
        {
            OSSemPost(TS_SYN);
        }
        
        if(!GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_0)) //���ֵ�λ��
        {
            if (AppData.digit_rp_en)
            {
                //DigitRP(LEFT_DRP);
                LeftDigitRP();
            }	
        }
        
        EXTI_ClearITPendingBit(EXTI_Line0);
        
    }    
      
   
    OSIntExit();
    
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
void EXTI1_IRQHandler(void)
{
  
    OS_CPU_SR  cpu_sr = 0;
    OS_ENTER_CRITICAL();
    OSIntNesting++;
    OS_EXIT_CRITICAL();   
    
    if(EXTI_GetITStatus(EXTI_Line1)!= RESET)
    {
        if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_1))
        {
            
            
        }
        
//         if(!GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_1)) //���ֵ�λ��
//         {
//             if (AppData.digit_rp_en)
//             {
//                 DigitRP(RIGHT_DRP); 
//             }	
//         }
        
        EXTI_ClearITPendingBit(EXTI_Line1);
    }    
    
    

    OSIntExit();
    
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
void EXTI2_IRQHandler(void)
{
  
    OS_CPU_SR  cpu_sr = 0;
  
    
    OS_ENTER_CRITICAL();    
    
    if(EXTI_GetITStatus(EXTI_Line2)!= RESET)  
    {
         if(!GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_2)) //���ֵ�λ��,�½���
         {
            if (AppData.digit_rp_en)
            {
                //DigitRP(RIGHT_DRP);
                RightDigitRP();
            }	
         }
         
         EXTI_ClearITPendingBit(EXTI_Line2);
        
     
    }
    
    
    OS_EXIT_CRITICAL();
    OSIntExit();
    
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
void EXTI3_IRQHandler(void)
{
  
    OS_CPU_SR  cpu_sr = 0;
  
    
    OS_ENTER_CRITICAL();    
    
    if(EXTI_GetITStatus(EXTI_Line3)!= RESET)
    {
        if(!GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3))
        {
            IN(3, 0);
            
        }
        else if(GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_3)) 
        {
             IN(3, 1);
            
        }
        
//         if(!GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_3)) //���ֵ�λ��,�½���
//         {
//             if (AppData.digit_rp_en)
//             {
//                 DigitRP(LEFT_DRP);
//             }	
//         }
         
         
        EXTI_ClearITPendingBit(EXTI_Line3);
    }    
    
    
    OS_EXIT_CRITICAL();
    OSIntExit();
    
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
void EXTI4_IRQHandler(void)
{
  
    OS_CPU_SR  cpu_sr = 0;
  
    
    OS_ENTER_CRITICAL();    
    
    if(EXTI_GetITStatus(EXTI_Line4)!= RESET)
    {
        if(!GPIO_ReadInputDataBit(GPIOE, GPIO_Pin_4))
        {
            
            
        }
        
        EXTI_ClearITPendingBit(EXTI_Line4);
    }    
    
    
    OS_EXIT_CRITICAL();
    OSIntExit();
    
}



