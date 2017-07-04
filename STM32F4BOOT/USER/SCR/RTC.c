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
#include "stm32f4xx.h"
#include "stm32f4_std_libs.h"
//#include "ucos_ii.h"
#include "mystd.h"
#include "m_config.h"
#include "RTC.h"

#define RTC_CLOCK_SOURCE_LSE   /* LSE used as RTC source clock */
/* #define RTC_CLOCK_SOURCE_LSI */ /* LSI used as RTC source clock. The RTC Clock
                                      may varies due to LSI frequency dispersion. */ 

extern volatile uint32_t  Timer2Cnt;
extern volatile uint32_t  Timer4Cnt;
extern volatile uint32_t  Timer5Cnt;
__IO uint32_t AsynchPrediv = 0, SynchPrediv = 0;

#define FIRST_DATA          0x32F2
//volatile uint32_t  test1;

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
void InitRTC(void)
{
     ///RTC_InitStructure;
    //test1 = RTC_ReadBackupRegister(RTC_BKP_DR3);
    if (RTC_ReadBackupRegister(RTC_BKP_DR0) != 0x32F2)
    {
        /* RTC configuration  */
        RTC_Config();
        
        RTC_WriteBackupRegister(RTC_BKP_DR0, FIRST_DATA);

        /* Configure the RTC data register and RTC prescaler */
//         RTC_InitStructure.RTC_AsynchPrediv = AsynchPrediv;
//         RTC_InitStructure.RTC_SynchPrediv  = SynchPrediv;
//         RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;

        /* Configure the time register */
        /////RTC_TimeRegulate(); //设置时间
    }
    else//已设置时间
    {   
        /* Enable the PWR clock */
        RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

        /* Allow access to RTC */
        PWR_BackupAccessCmd(ENABLE);
        RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_BKPSRAM, ENABLE);
        /* Wait for RTC APB registers synchronisation */
        RTC_WaitForSynchro();

        /* Clear the RTC Alarm Flag */
        RTC_ClearFlag(RTC_FLAG_ALRAF);
        
        //RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_BKPSRAM, ENABLE);
        
        //RTC_WriteBackupRegister(RTC_BKP_DR3, 0xAA55);

        /* Clear the EXTI Line 17 Pending bit (Connected internally to RTC Alarm) */
        //EXTI_ClearITPendingBit(EXTI_Line17);

    }
    
    ////RTC_AlarmConfig();
   
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
void RTC_Config1(void)
{
    /* Enable the PWR clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Allow access to RTC */
    PWR_BackupAccessCmd(ENABLE);

    #if defined (RTC_CLOCK_SOURCE_LSI)  /* LSI used as RTC source clock*/
    /* The RTC Clock may varies due to LSI frequency dispersion. */
    /* Enable the LSI OSC */ 
    RCC_LSICmd(ENABLE);

    /* Wait till LSI is ready */  
    while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
    {
    }

    /* Select the RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);

    SynchPrediv = 0xFF;
    AsynchPrediv = 0x7F;

    #elif defined (RTC_CLOCK_SOURCE_LSE) /* LSE used as RTC source clock */
    /* Enable the LSE OSC */
    RCC_LSEConfig(RCC_LSE_ON);

    /* Wait till LSE is ready */  
    while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {
    }

    /* Select the RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    SynchPrediv = 0xFF;
    AsynchPrediv = 0x7F;

  #else
    #error Please select the RTC Clock source inside the main.c file
  #endif /* RTC_CLOCK_SOURCE_LSI */

    /* Enable the RTC Clock */
    RCC_RTCCLKCmd(ENABLE);

    /* Wait for RTC APB registers synchronisation */
    RTC_WaitForSynchro();

    /* Enable The TimeStamp */
    RTC_TimeStampCmd(RTC_TimeStampEdge_Falling, ENABLE);    
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
void RTC_Config(void)
{
    RTC_InitTypeDef RTC_InitStructure;
   /// RTC_TimeTypeDef  RTC_TimeStruct;

    /* Enable the PWR clock */
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);

    /* Allow access to RTC */
    PWR_BackupAccessCmd(ENABLE);

    /* Reset RTC Domain */
    RCC_BackupResetCmd(ENABLE);
    RCC_BackupResetCmd(DISABLE);

    /* Enable the LSE OSC */
    RCC_LSEConfig(RCC_LSE_ON);

    /* Wait till LSE is ready */  
    while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
    {
    }

    /* Select the RTC Clock Source */
    RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);

    /* Configure the RTC data register and RTC prescaler */
    RTC_InitStructure.RTC_AsynchPrediv = 0x7F;
    RTC_InitStructure.RTC_SynchPrediv  = 0xFF;
    RTC_InitStructure.RTC_HourFormat   = RTC_HourFormat_24;
    RTC_Init(&RTC_InitStructure);
    
    
//     /* Set the time to 00h 00mn 00s AM */
//     RTC_TimeStruct.RTC_H12     = RTC_H12_AM;
//     RTC_TimeStruct.RTC_Hours   = 0x00;
//     RTC_TimeStruct.RTC_Minutes = 0x00;
//     RTC_TimeStruct.RTC_Seconds = 0x00;  
//     RTC_SetTime(RTC_Format_BCD, &RTC_TimeStruct);
  
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
u8 RTC_Set(RTC_DateTypeDef *RTC_Date,RTC_TimeTypeDef *RTC_Time)
{
    if (RTC_Date->RTC_WeekDay == 0)
    {
        RTC_Date->RTC_WeekDay = 1; 
    }
    
	if((RTC_SetTime(RTC_Format_BIN, RTC_Time) & (RTC_SetDate(RTC_Format_BIN,RTC_Date)))!=ERROR)
	{
        RTC_WriteBackupRegister(RTC_BKP_DR0,0x32F2); 
	}
    
    return 0;

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
void RTC_TimeStampShow(void)
{
    char  str[100];
    RTC_TimeTypeDef  RTC_TimeStampStructure;
    RTC_DateTypeDef  RTC_TimeStampDateStructure;

    /* Get the current TimeStamp */
    RTC_GetTimeStamp(RTC_Format_BIN, &RTC_TimeStampStructure, &RTC_TimeStampDateStructure);
    //==============TimeStamp Display (Time and Date)=================
    sprintf(str,"%d:%d:%d\r\n", RTC_TimeStampStructure.RTC_Hours, \
            RTC_TimeStampStructure.RTC_Minutes, RTC_TimeStampStructure.RTC_Seconds);
    //UsartPrintf(USART2,str);
    sprintf(str,"%d-%d-%d\r\n", RTC_TimeStampDateStructure.RTC_WeekDay, \
           RTC_TimeStampDateStructure.RTC_Date, RTC_TimeStampDateStructure.RTC_Month);
    //UsartPrintf(USART2,str);
    
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
void RTC_AlarmConfig1(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RTC_AlarmTypeDef RTC_AlarmStructure;

	EXTI_ClearITPendingBit(EXTI_Line17); //外部中断17
	EXTI_InitStructure.EXTI_Line = EXTI_Line17; //选中外部中断17
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //模式中断
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //上升沿
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; //使能外部中断
	EXTI_Init(&EXTI_InitStructure); //外部中断初始化

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3); //中断向量分组
	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn; //连到RTC_Alarm_IRQn
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //主优先级2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //次优先级0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //使能中断向量
	NVIC_Init(&NVIC_InitStructure); //中断向量初始化

	RTC_AlarmStructure.RTC_AlarmDateWeekDay = 0x31; //报警设置
	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;//报警设置	
	RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay; //星期.日期报警的设置被屏蔽
	
	RTC_AlarmStructure.RTC_AlarmTime.RTC_H12 = RTC_H12_PM; //报警设置下午
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = 0x23; //报警设置23 时
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = 0X59; //报警设置59 分
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 0x59; //报警设置59 秒
	RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &RTC_AlarmStructure); //报警设置写入

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); //
	PWR_BackupAccessCmd(ENABLE); //使能RTC 操作
	RTC_WaitForSynchro(); //等待RTC APB 寄存器同步
	RTC_ClearFlag(RTC_FLAG_ALRAF); //清除RTC 闹钟标志
	EXTI_ClearITPendingBit(EXTI_Line17); //清除中断线17 标志（内部连接至RTC 闹钟）
	
	RTC_ITConfig(RTC_IT_ALRA, ENABLE); //使能报警中断
	RTC_AlarmCmd(RTC_Alarm_A, ENABLE); //报警功能使能
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
void RTC_AlarmConfig(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    RTC_AlarmTypeDef RTC_AlarmStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //     //   /* EXTI configuration */
    //内部连接至RTC 闹钟
    EXTI_ClearITPendingBit(EXTI_Line17);
    EXTI_InitStructure.EXTI_Line = EXTI_Line17;
    EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
    EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
    EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);

    /* Enable the RTC Alarm Interrupt */
    NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = RTC_ALARM_PRIO;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    /* Set the alarmA Masks */
    RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_All;
    RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &RTC_AlarmStructure);

    /* Set AlarmA subseconds and enable SubSec Alarm : generate 8 interripts per Second */
    //RTC_AlarmSubSecondConfig(RTC_Alarm_A, 0xFF, RTC_AlarmSubSecondMask_SS14_5);

    //RTC_AlarmSubSecondMask_SS14_5是每秒产生8次中断，那肦TC_AlarmSubSecondMask_SS14_5 * 8
    //则是一秒产生一次中断
    /*
    Bits 22:16 PREDIV_A[6:0]: Asynchronous prescaler factor
                This is the asynchronous division factor:
                ck_apre frequency = RTCCLK frequency/(PREDIV_A+1)
                Note: PREDIV_A [6:0]= 000000 is a prohibited value.
    Bit 15 Reserved, always read as 0.
    Bits 14:0 PREDIV_S[14:0]: Synchronous prescaler factor
            This is the synchronous division factor:
            ck_spre frequency = ck_apre frequency/(PREDIV_S+1)
   */
   //注意在初始化里PREDIV_A为0x7F,PREDIV_S为0xFF;
   //原晶振32768,那么
   //ck_apre frequency = RTCCLK frequency/(PREDIV_A+1),ck_apre = 256
   //ck_spre frequency = ck_apre frequency/(PREDIV_S+1)
   //即先经过128分频(0x7F + 1),在经过(0xFF + 1)分频
   //再经过(PREDIV_S+1),0xFF + 1分频,最后ck_spre = 1(=32768 / (0x7F +1) / (0xFF + 1))
   //其实就是ck_spre对应的SS记满256即为1秒,下面就是对SS的设置
   /*Bit 14:0 SS[14:0]: Sub seconds value
        This value is compared with the contents of the synchronous prescaler’s counter to
        determine if Alarm A is to be activated. Only bits 0 up MASKSS-1 are compared.
   */
   //0xFF为RTC_AlarmSubSecondValue一秒的计数值,RTC_AlarmSubSecondMask_SS14_8则为SS[7:0]参与比较
   //SS[7:0]满为255
   RTC_AlarmSubSecondConfig(RTC_Alarm_A, 0xFF, RTC_AlarmSubSecondMask_SS14_8);


    /* Enable AlarmA interrupt */
    RTC_ITConfig(RTC_IT_ALRA, ENABLE);

    /* Enable the RTC Clock */
    RCC_RTCCLKCmd(ENABLE);

    /* Wait for RTC APB registers synchronisation */
    RTC_WaitForSynchro();

    /* Enable the alarmA */
    RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
    


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
void RTC_AlarmShow(u16 x,u16 y)
{
  	RTC_AlarmTypeDef  RTC_AlarmStructure;
    
    RTC_GetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
	//LCD_String(x,y,"Alarm:  :  :",RED);
	//LCD_Num(x+95,y,RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours,2,RED);
	//LCD_Num(x+145,y,RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes,2,RED);
	//LCD_Num(x+195,y,RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds,2,RED);
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
void  ReadBackRAM(u8 off_addr,u32 *buff, u8 size)
{
    int i;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_BKPSRAM, ENABLE);
    
    for (i = 0; i < size; i++)
    {
        buff[i] =  (*(__IO uint32_t *) (BKPSRAM_BASE + i));
        
    }
          
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_BKPSRAM, DISABLE);
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
void  WriteBackRAM(u8 off_addr, u32 *buff, u8 size)
{
    int i;
    
    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_BKPSRAM, ENABLE);
    
    for (i = 0; i < size; i++)
    {
        (*(__IO uint32_t *) (BKPSRAM_BASE + i)) = buff[i];
        
    }
          
   // RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_BKPSRAM, DISABLE);
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
void RTC_Alarm_IRQHandler(void)
{
    static uint8_t  sys_Led_run = FALSE;
    
    
    if(RTC_GetITStatus(RTC_IT_ALRA) != RESET)
	{
            
        //led运行灯
        if(sys_Led_run)
        {
            GPIO_SetBits(GPIOD, GPIO_Pin_3);            
        }
        else
        {
            GPIO_ResetBits(GPIOD, GPIO_Pin_3);            
        } 
        
        sys_Led_run = !sys_Led_run;
        
        //printf("Timer5Cnt = %d,Timer2Cnt = %d\n",Timer5Cnt,Timer2Cnt);
        Timer5Cnt = 0;
        Timer2Cnt = 0;
        
		RTC_ClearITPendingBit(RTC_IT_ALRA); //清除中断标志
		EXTI_ClearITPendingBit(EXTI_Line17); //清除中断标志
	}
    
    
    
}



