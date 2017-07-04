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
        /////RTC_TimeRegulate(); //����ʱ��
    }
    else//������ʱ��
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
void RTC_AlarmConfig1(void)
{
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	RTC_AlarmTypeDef RTC_AlarmStructure;

	EXTI_ClearITPendingBit(EXTI_Line17); //�ⲿ�ж�17
	EXTI_InitStructure.EXTI_Line = EXTI_Line17; //ѡ���ⲿ�ж�17
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; //ģʽ�ж�
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising; //������
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; //ʹ���ⲿ�ж�
	EXTI_Init(&EXTI_InitStructure); //�ⲿ�жϳ�ʼ��

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_3); //�ж���������
	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn; //����RTC_Alarm_IRQn
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2; //�����ȼ�2
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0; //�����ȼ�0
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE; //ʹ���ж�����
	NVIC_Init(&NVIC_InitStructure); //�ж�������ʼ��

	RTC_AlarmStructure.RTC_AlarmDateWeekDay = 0x31; //��������
	RTC_AlarmStructure.RTC_AlarmDateWeekDaySel = RTC_AlarmDateWeekDaySel_Date;//��������	
	RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay; //����.���ڱ��������ñ�����
	
	RTC_AlarmStructure.RTC_AlarmTime.RTC_H12 = RTC_H12_PM; //������������
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = 0x23; //��������23 ʱ
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = 0X59; //��������59 ��
	RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = 0x59; //��������59 ��
	RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &RTC_AlarmStructure); //��������д��

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE); //
	PWR_BackupAccessCmd(ENABLE); //ʹ��RTC ����
	RTC_WaitForSynchro(); //�ȴ�RTC APB �Ĵ���ͬ��
	RTC_ClearFlag(RTC_FLAG_ALRAF); //���RTC ���ӱ�־
	EXTI_ClearITPendingBit(EXTI_Line17); //����ж���17 ��־���ڲ�������RTC ���ӣ�
	
	RTC_ITConfig(RTC_IT_ALRA, ENABLE); //ʹ�ܱ����ж�
	RTC_AlarmCmd(RTC_Alarm_A, ENABLE); //��������ʹ��
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
void RTC_AlarmConfig(void)
{
    EXTI_InitTypeDef EXTI_InitStructure;
    RTC_AlarmTypeDef RTC_AlarmStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    //     //   /* EXTI configuration */
    //�ڲ�������RTC ����
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

    //RTC_AlarmSubSecondMask_SS14_5��ÿ�����8���жϣ����RTC_AlarmSubSecondMask_SS14_5 * 8
    //����һ�����һ���ж�
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
   //ע���ڳ�ʼ����PREDIV_AΪ0x7F,PREDIV_SΪ0xFF;
   //ԭ����32768,��ô
   //ck_apre frequency = RTCCLK frequency/(PREDIV_A+1),ck_apre = 256
   //ck_spre frequency = ck_apre frequency/(PREDIV_S+1)
   //���Ⱦ���128��Ƶ(0x7F + 1),�ھ���(0xFF + 1)��Ƶ
   //�پ���(PREDIV_S+1),0xFF + 1��Ƶ,���ck_spre = 1(=32768 / (0x7F +1) / (0xFF + 1))
   //��ʵ����ck_spre��Ӧ��SS����256��Ϊ1��,������Ƕ�SS������
   /*Bit 14:0 SS[14:0]: Sub seconds value
        This value is compared with the contents of the synchronous prescaler��s counter to
        determine if Alarm A is to be activated. Only bits 0 up MASKSS-1 are compared.
   */
   //0xFFΪRTC_AlarmSubSecondValueһ��ļ���ֵ,RTC_AlarmSubSecondMask_SS14_8��ΪSS[7:0]����Ƚ�
   //SS[7:0]��Ϊ255
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
void RTC_Alarm_IRQHandler(void)
{
    static uint8_t  sys_Led_run = FALSE;
    
    
    if(RTC_GetITStatus(RTC_IT_ALRA) != RESET)
	{
            
        //led���е�
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
        
		RTC_ClearITPendingBit(RTC_IT_ALRA); //����жϱ�־
		EXTI_ClearITPendingBit(EXTI_Line17); //����жϱ�־
	}
    
    
    
}



