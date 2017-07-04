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
#include "includes.h"
#include <math.h>
#include "mystd.h"
#include "m_config.h"
#include "stm32f4xx_usart.h"
#include "SysAppConfig.h"
#include "LCD_RA8875.h"
#include "RTC.h"
#include "TimeSet.h"
#include "ID.h"
#include "tasks.h"
#include "StatTask.h"




OS_STK   AppStartTaskStk_1[APP_TASK_START_STK_SIZE];
OS_STK   AppStartTaskStk_2[GUI_TASK_STK_SIZE];
OS_STK   AppStartTaskStk_3[APP_TASK_START_STK_SIZE];
OS_STK   AppStartTaskStk_4[APP_TASK_START_STK_SIZE];

OS_EVENT  *LCD_SEM;
OS_EVENT  *TS_SYN;



extern void  TS_Task(void);
extern TASK_USER_DATA  TaskUserData[];
extern volatile uint32_t   SpeedRPM;
extern volatile uint32_t   AngleTmCnt;
extern volatile  uint32_t  SpeedTestCnt;



//--------------------------------------------------------------------------------------
OS_EVENT    *UART0_SEM;             //互斥
OS_EVENT    *UART_COMM_FRM_SEM;     //互斥
OS_EVENT    *UART0_SYN;             //同步
OS_EVENT    *UART1_SYN;             //同步


OS_EVENT    *UART_Q;                //message queue

OS_EVENT    *TMPTR_CTRL_MBOX;

OS_EVENT    *REV_CTRL_START_MBOX;
OS_EVENT    *REV_CTRL_OVER_MBOX;  
OS_EVENT    *REV_CTRL_MEAS_MBOX;
OS_EVENT    *REV_CNT_MBOX;       
OS_EVENT    *OIL_CNT_MBOX;        
OS_EVENT    *COMM_RAIL_CTRL_MBOX; 
OS_EVENT    *DIGIT_RP_MBOX;           //

SYS_APP_CFG  SysAppCfg;
APP_DATA  AppData = {0};


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
void AppStartTask_1(void *p_arg)
{
    volatile uint16_t x;
    volatile uint16_t y;
    INT8U  err;
    (void)p_arg;   
    
    
    OS_CPU_SysTickInit();
   
    OSTimeDlyHMSM(0, 0, 0, 1000);
    OSStatInit();
    TS_SYN  = OSSemCreate(0);
    
    while(1)
    {        
       
        OSSemPend(TS_SYN, 0, &err);
        if (err == OS_ERR_NONE)
        {
           
        }        
        OSTimeDlyHMSM(0, 0, 0, 55);
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
void AppStartTask_2 (void *p_arg)
{
 
	while(1)
	{   	
		          
        OSTimeDly(50);
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
void AppStartTask_3 (void *p_arg)
{
   
    
	while(1)
	{   	
        
        OSTimeDlyHMSM(0, 0, 0, 30);
        
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
void AppStartTask_4(void *p_arg)
{
        
	OSTimeDlyHMSM(0,0,4,0);
      
	while(1)
	{   	
        
  
        /*
        for (i = 0; i < 3; i++)
        {
            err  = OSTaskStkChk(APP_TASK_START_PRIO_1 + i, &data);
            ///time = PC_ElapsedStop();
        
            if (err == OS_NO_ERR) 
            {
              #ifdef PRINTF_STACK_STAT
                printf("%s  %5ld        %5ld        %5ld        %6d\n",
                        TaskUserData[i].TaskName,
                        data.OSFree + data.OSUsed,
                        data.OSFree,
                        data.OSUsed,
                        0); //0=time
              #endif                
            }
        }
        */
    
		OSTimeDlyHMSM(0, 0, 0, 30);
        
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
void CreateTasksEx(void)
{
    strcpy(TaskUserData[0].TaskName, "TS   Task");

    OSTaskCreateExt(AppStartTask_1,
                    (void *)0,
                    &AppStartTaskStk_1[APP_TASK_START_STK_SIZE - 1],
                    APP_TASK_START_PRIO_1,
                    0,
                    &AppStartTaskStk_1[0],
                    APP_TASK_START_STK_SIZE,
                    &TaskUserData[0],
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);

    strcpy(TaskUserData[1].TaskName, "GUI  Task");

    OSTaskCreateExt(AppStartTask_2,
                    (void *)0,
                    &AppStartTaskStk_2[GUI_TASK_STK_SIZE - 1],
                    APP_TASK_START_PRIO_2,
                    1,
                    &AppStartTaskStk_2[0],
                    GUI_TASK_STK_SIZE,
                    &TaskUserData[1],
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);	

    strcpy(TaskUserData[2].TaskName, "App3 Task");

    OSTaskCreateExt(AppStartTask_3,
                    (void *)0,
                    &AppStartTaskStk_3[APP_TASK_START_STK_SIZE - 1],
                    APP_TASK_START_PRIO_3,
                    2,
                    &AppStartTaskStk_3[0],
                    APP_TASK_START_STK_SIZE,
                    &TaskUserData[2],
                    OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);	

    strcpy(TaskUserData[3].TaskName, "App4 Task");


    OSTaskCreateExt(AppStartTask_4,
                    (void *)0,
                    &AppStartTaskStk_4[APP_TASK_START_STK_SIZE - 1],
                    APP_TASK_START_PRIO_4,
                    3,
                    &AppStartTaskStk_4[0],
                    APP_TASK_START_STK_SIZE,
                    &TaskUserData[3],
                    0);  
        
    
    
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
void CreateTasks(void)
{

    //OSTaskCreate(AppStartTask_1, (void *)0, &AppStartTaskStk_1[APP_TASK_START_STK_SIZE-1], APP_TASK_START_PRIO_1);

    //OSTaskCreate(AppStartTask_2, (void *)0, &AppStartTaskStk_2[GUI_TASK_STK_SIZE-1], APP_TASK_START_PRIO_2);

    //OSTaskCreate(AppStartTask_3, (void *)0, &AppStartTaskStk_3[APP_TASK_START_STK_SIZE-1], APP_TASK_START_PRIO_3);	

    //OSTaskCreate(AppStartTask_4, (void *)0, &AppStartTaskStk_4[APP_TASK_START_STK_SIZE-1], APP_TASK_START_PRIO_4);

    
}







