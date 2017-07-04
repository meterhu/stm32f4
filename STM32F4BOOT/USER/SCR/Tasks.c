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
#include "mystd.h"  
#include "m_config.h"
#include "stm32f4xx.h"
#include "ucos_ii.h"
#include "SysAppConfig.h"
#include "UserLib.h"
#include "UART.h"
#include "I2C.h"
#include "Timer.h"
#include "ADS784X.h"         
#include "DS18B20.h"
#include "DigitRP.h"
#include "RTC.h"
#include "FramePack.h"
#include "CommHandle.h"
#include "INOut.h"
#include "Transdu.h"
#include "PWM.h"
#include "main.h"
#include "tasks.h"
#include "OS_Trace.h"
#include "statTask.h"

//----------------------------------------------------------------------------------
#define  ADV_Q_SIZE         10
#define  TMPTR_Q_SIZE       5
#define  INOUT_Q_SIZE       2

#define  OIL_CNT_Q_SIZE     10

//--------------------------------------------------------------------------------
OS_STK  TmptrCtrlTaskStk[TMPTR_CTRL_TASK_STK_SIZE];
OS_STK  MeasOilTaskStk[MEAS_OIL_TASK_STK_SIZE];
OS_STK  OilCntTaskStk[OIL_CNT_TASK_STK_SIZE];
OS_STK  RevCntTaskStk[REV_CNT_TASK_STK_SIZE];
OS_STK  RevCtrlTaskStk[REV_CTRL_TASK_STK_SIZE];
OS_STK  TmptrSamplTaskStk[TMPTR_SAMPL_TASK_STK_SIZE];
OS_STK  ADSamplTaskStk[AD_SAMPL_TASK_STK_SIZE];
OS_STK  INOutTaskStk[IN_OUT_TASK_STK_SIZE];
OS_STK  DightRPTaskStk[DIGIT_RP_TASK_STK_SIZE];
OS_STK  TransCtrlTaskStk[TRANS_CTRL_TASK_STK_SIZE];


//----------------------------------------------------------------------------------
OS_EVENT    *PRINTF_SEM;             //互斥
OS_EVENT    *LCD_SEM;
OS_EVENT    *TS_SYN;

OS_EVENT    *UART0_SEM;             //互斥
OS_EVENT    *UART_COMM_FRM_SEM;     //互斥
OS_EVENT    *UART0_SYN;             //同步
OS_EVENT    *UART1_SYN;             //同步
OS_EVENT    *UART_Q;                //message queue

OS_EVENT    *PWM_TEST_SEM;             //互斥

OS_EVENT    *TMPTR_CTRL_MBOX;
OS_EVENT    *REV_CTRL_MBOX;
OS_EVENT    *REV_CNT_MBOX;       
OS_EVENT    *OIL_CNT_MBOX;        
OS_EVENT    *DIGIT_RP_MBOX;         //

OS_EVENT    *OIL_CNT_Q; 

OS_EVENT    *PACKED_MUTEX;             //互斥


//----------------------------------------------------------------------------------
void   *UARTQBuff[UARTQ_BUFF_SIZE];
void   *ADValueBuff[ADV_Q_SIZE]; //
void   *TmptrBuff[TMPTR_Q_SIZE]; //
void   *OilCntQBuff[OIL_CNT_Q_SIZE];


extern OS_EVENT    *REV_MEAS_RUN_MBOX;
extern OS_EVENT    *ADV_Q;
extern OS_EVENT    *TMPTR_Q;
extern OS_EVENT    *INOUT_MBOX;


//----------------------------------------------------------------------------------
extern  void  TmptrCtrlTask(void *p_arg);
extern  void  OilCntTask(void *p_arg);
extern  void  MeasOilTask(void *p_arg);
extern  void  RevCtrlTask(void *p_arg);
extern  void  DightRPTask(void *p_arg);
extern  void  RevCntTask(void *p_arg);
extern  void  TmptrSamplTask(void *p_arg);
extern  void  ADSamplTask(void *p_arg);
extern  void  INOutTask(void *p_arg);
extern  void  TransduCtrlTask(void *p_arg);

//static  void  TmptrCtrlMainTask(void *p_arg);

extern void  TS_Task(void);
extern uint32_t   SpeedRPM;
extern volatile uint32_t   AngleTmCnt;
extern volatile  uint32_t  SpeedTestCnt;


//----------------------------------------------------------------------------------
TASK_INFO   TaskInfo[]=
{
    {TmptrCtrlTask, TmptrCtrlTaskStk,  TMPTR_CTRL_TASK_PRIO, TMPTR_CTRL_TASK_STK_SIZE},
    {OilCntTask,  OilCntTaskStk, OIL_CNT_TASK_PRIO, OIL_CNT_TASK_STK_SIZE},
   //{RevMeasRunTask, RevMeasRunTaskStk, REV_MEAS_TASK_PRIO,REV_MEAS_TASK_STK_SIZE},
};

//----------------------------------------------------------------------------------
SYS_APP_CFG  SysAppCfg;
APP_DATA  AppData;


extern TASK_USER_DATA  TaskUserData[];
extern OS_EVENT    *TRANS_CTRL_MBOX;  

//======================================================================================================
/*******************************************************************************************************
 *
 *         函数名称: 
 *
 *         函数功能: 接收串口命令--解释执行
 *
 *         入口参数: 
 *
 *         出口参数: 无
 *
 *             说明:
 *
 *******************************************************************************************************/
void  MainTask(void *p_arg)
{
    OS_CPU_SR     cpu_sr = 0;
    INT8U err;
    
    //OS_ENTER_CRITICAL();
    
    InitApp();    
    InitBoardDev();    

#if 0   
    OS_ENTER_CRITICAL();    
    //互斥量
    PRINTF_SEM        = OSSemCreate(1); 
    UART_COMM_FRM_SEM = OSSemCreate(1); 
    PWM_TEST_SEM      = OSSemCreate(1);
    //LCD_SEM    = OSSemCreate(1);
    
    //同步量
    //TS_SYN     = OSSemCreate(0);
    
    //消息
    DIGIT_RP_MBOX         =  OSMboxCreate((void *)0);
    INOUT_MBOX            =  OSMboxCreate((void *)0);
    REV_CNT_MBOX          =  OSMboxCreate((void *)0);
    REV_CTRL_MBOX         =  OSMboxCreate((void *)0);
    TRANS_CTRL_MBOX       =  OSMboxCreate((void *)0);
    

    
    OIL_CNT_MBOX          =  OSMboxCreate((void *)0);
    ///OIL_CNT_Q             =  OSQCreate(OilCntQBuff,   OIL_CNT_Q_SIZE);
     
    PACKED_MUTEX          =  OSMutexCreate(MUTEX1_TASK_PRIO, &err);
    
    //温度采样
    OSTaskCreate(TmptrSamplTask, (void *)0, &TmptrSamplTaskStk[TMPTR_SAMPL_TASK_STK_SIZE-1],   TMPTR_SAMPL_TASK_PRIO);
    OSTaskNameSet(TMPTR_SAMPL_TASK_PRIO,"TmptrSampl", &err);
    
    //AD采样 
    OSTaskCreate(ADSamplTask,    (void *)0, &ADSamplTaskStk[AD_SAMPL_TASK_STK_SIZE-1],         AD_SAMPL_TASK_PRIO);
    OSTaskNameSet(AD_SAMPL_TASK_PRIO,"ADSampl", &err);
    
    //温度控制
    //OSTaskCreate(TmptrCtrlTask, (void *)0, &TmptrCtrlTaskStk[TMPTR_CTRL_TASK_STK_SIZE-1],      TMPTR_CTRL_TASK_PRIO);
    //OSTaskNameSet(TMPTR_CTRL_TASK_PRIO,"TmptrCtrl", &err);
    //OSTaskSuspend(TMPTR_CTRL_TASK_PRIO); //先挂起该任务
    
    //数字电位器
    OSTaskCreate(DightRPTask,    (void *)0, &DightRPTaskStk[DIGIT_RP_TASK_STK_SIZE-1],         DIGIT_RP_TASK_PRIO);
    OSTaskNameSet(DIGIT_RP_TASK_PRIO,"DightRP", &err);

    
    //开入
    OSTaskCreate(INOutTask,      (void *)0, &INOutTaskStk[IN_OUT_TASK_STK_SIZE-1],             IN_OUT_TASK_STK_PRIO);
    OSTaskNameSet(IN_OUT_TASK_STK_PRIO,"INOut", &err);
    
    //转速计数    
    OSTaskCreate(RevCntTask,      (void *)0, &RevCntTaskStk[REV_CNT_TASK_STK_SIZE-1],          REV_CNT_TASK_TASK_PRIO);
    OSTaskNameSet(REV_CNT_TASK_TASK_PRIO,"RevCnt", &err);
    
    //转速控制
    OSTaskCreate(RevCtrlTask,      (void *)0, &RevCtrlTaskStk[REV_CTRL_TASK_STK_SIZE-1],       REV_CTRL_TASK_PRIO);
    OSTaskNameSet(REV_CTRL_TASK_PRIO,"RevCtrl", &err);
    
    //量油计数 
    //OSTaskCreate(OilCntTask,      (void *)0, &OilCntTaskStk[OIL_CNT_TASK_STK_SIZE-1],          OIL_CNT_TASK_PRIO);
    //OSTaskNameSet(OIL_CNT_TASK_PRIO,"OilCnt", &err);
    
    //油量测量
    OSTaskCreate(MeasOilTask,      (void *)0, &MeasOilTaskStk[MEAS_OIL_TASK_STK_SIZE-1],       MEAS_OIL_TASK_PRIO);
    OSTaskNameSet(MEAS_OIL_TASK_PRIO,"MeasOil", &err);
    
    //变频器控制
    OSTaskCreate(TransduCtrlTask, (void *)0, &TransCtrlTaskStk[TRANS_CTRL_TASK_STK_SIZE-1],    TRANS_CTRL_TASK_PRIO);
    OSTaskNameSet(TRANS_CTRL_TASK_PRIO,"TransduCtrl", &err);
    
        
  
    OS_EXIT_CRITICAL();
#endif      
    while(TRUE_P) 
    {                                          
        FrameParseExec();
        
        OSTimeDlyHMSM(0, 0, 0, 60);
        
    }

}

//======================================================================================================
/*******************************************************************************************************
 *
 *         函数名称: 
 *
 *         函数功能: 接收串口命令--解释执行
 *
 *         入口参数: 
 *
 *         出口参数: 无
 *
 *             说明:
 *
 *******************************************************************************************************/
void  MainTask1(void *p_arg)
{
    
    //INT8U        err;
    //char         *pb;
    //void         *rt;
   
    
    //OS_CPU_SysTickHandler
    //SysTick_Config(SystemCoreClock/OS_TICKS_PER_SEC - 1); //1ms                                 
    
  
    
    
    #if 0
    //----------------------------------------------------------------
    //同步 & 互斥量
    UART0_SEM             =  OSSemCreate(1);  //互斥量
    UART0_SYN             =  OSSemCreate(0);  //同步量，等于消息
	UART1_SYN             =  OSSemCreate(0);  //同步量，等于消息
    
    
    //SEND_FRM2_SYN       =  OSSemCreate(0);
    UART_COMM_FRM_SEM     =  OSSemCreate(1);  //互斥量

    //----------------------------------------------------------------
    //邮箱
    INOUT_MBOX            = OSMboxCreate((void *)0);
    TMPTR_CTRL_MBOX       = OSMboxCreate((void *)0);
	//RevMeasRun
	REV_MEAS_RUN_MBOX     = OSMboxCreate((void *)0);
    
    REV_CTRL_START_MBOX   = OSMboxCreate((void *)0);
    REV_CTRL_OVER_MBOX    = OSMboxCreate((void *)0);
    
    REV_CTRL_MEAS_MBOX    = OSMboxCreate((void *)0);


    REV_CNT_MBOX          =  OSMboxCreate((void *)0); 
    OIL_CNT_MBOX          =  OSMboxCreate((void *)0);
    COMM_RAIL_CTRL_MBOX   =  OSMboxCreate((void *)0);
    DIGIT_RP_MBOX         =  OSMboxCreate((void *)0);
    //----------------------------------------------------------------
    //队列
    UART_Q   =  OSQCreate(UARTQBuff,   UARTQ_BUFF_SIZE);
    ADV_Q    =  OSQCreate(ADValueBuff, ADV_Q_SIZE);
    TMPTR_Q  =  OSQCreate(TmptrBuff,   TMPTR_Q_SIZE);
   // IN_Q     =  OSQCreate(INOutBuff,   INOUT_Q_SIZE);      //也可以用msg box
    
    TS_SYN  = OSSemCreate(0);
    LCD_SEM = OSSemCreate(1);
   //TMPTR_CTRL_Q = OSQCreate(TmptrBuff,   TMPTR_Q_SIZE);
    
    #endif
    

    
    #if 0
    //创建其他任务
    //---------------------------------------------------------------------------------------------
    strcpy(TaskUserData[0].TaskName, "TS   Task");
    OSTaskCreateExt(TmptrSamplTask,  //Create the start task. 
                    (void          * )0,
                    &TmptrSamplTaskStk[TMPTR_SAMPL_TASK_STK_SIZE - 1],
                    TMPTR_SAMPL_TASK_PRIO,
                    TMPTR_SAMPL_TASK_PRIO,
                    &TmptrSamplTaskStk[0],
                    TMPTR_SAMPL_TASK_STK_SIZE,
                    &TaskUserData[0],
                    (OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

//      OSTaskCreateExt(AppStartTask_1,
//                     (void *)0,
//                     &AppStartTaskStk_1[APP_TASK_START_STK_SIZE - 1],
//                     APP_TASK_START_PRIO_1,
//                     0,
//                     &AppStartTaskStk_1[0],
//                     APP_TASK_START_STK_SIZE,
//                     &TaskUserData[0],
//                     OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR);
   
    OSTaskCreateExt((void (*)(void *))ADSamplTask,  //Create the start task. 
                    (void          * )0,
                    (OS_STK        * )&ADSamplTaskStk[AD_SAMPL_TASK_STK_SIZE - 1],
                    (INT8U           )AD_SAMPL_TASK_PRIO,
                    (INT16U          )AD_SAMPL_TASK_PRIO,
                    (OS_STK        * )&ADSamplTaskStk[0],
                    (INT32U          )AD_SAMPL_TASK_STK_SIZE,
                    (void          * )0,
                    (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
 
   
    OSTaskCreateExt((void (*)(void *))SendFrame1Task,  //Create the start task. 
                    (void          * )0,
                    (OS_STK        * )&SendFrame1TaskStk[SEND_FRAME1_TASK_STK_SIZE - 1],
                    (INT8U           )SEND_FRAME1_TASK_PRIO,
                    (INT16U          )SEND_FRAME1_TASK_PRIO,
                    (OS_STK        * )&SendFrame1TaskStk[0],
                    (INT32U          )SEND_FRAME1_TASK_STK_SIZE,
                    (void          * )0,
                    (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
 
   
   
    OSTaskCreateExt((void (*)(void *))SendTmptrADFrameTask,  //Create the start task. 
                    (void          * )0,
                    (OS_STK        * )&SendTmptrADFrameTaskStk[SEND_TMPTR_AD_FRAME_TASK_STK_SIZE - 1],
                    (INT8U           )SEND_TMPTR_AD_FRAME_TASK_TASK_PRIO,
                    (INT16U          )SEND_TMPTR_AD_FRAME_TASK_TASK_PRIO,
                    (OS_STK        * )&SendTmptrADFrameTaskStk[0],
                    (INT32U          )SEND_TMPTR_AD_FRAME_TASK_STK_SIZE,
                    (void          * )0,
                    (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));  

                    
    OSTaskCreateExt((void (*)(void *))SendRunRevFrameTask,  //Create the start task. 
                    (void          * )0,
                    (OS_STK        * )&SendRunRevFrameTaskStk[SEND_RUN_REV_FRAME_TASK_STK_SIZE - 1],
                    (INT8U           )SEND_RUN_REV_FRAME_TASK_TASK_PRIO,
                    (INT16U          )SEND_RUN_REV_FRAME_TASK_TASK_PRIO,
                    (OS_STK        * )&SendRunRevFrameTaskStk[0],
                    (INT32U          )SEND_RUN_REV_FRAME_TASK_STK_SIZE,
                    (void          * )0,
                    (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
                      
    OSTaskCreateExt((void (*)(void *))MeasOilTask,  //Create the start task. 
                    (void          * )0,
                    (OS_STK        * )&MeasOilTaskStk[MEAS_OIL_TASK_STK_SIZE - 1],
                    (INT8U           )MEAS_OIL_TASK_PRIO,
                    (INT16U          )MEAS_OIL_TASK_PRIO,
                    (OS_STK        * )&MeasOilTaskStk[0],
                    (INT32U          )MEAS_OIL_TASK_STK_SIZE,
                    (void          * )0,
                    (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));                      
                 
    OSTaskCreateExt((void (*)(void *))FuelInjeCommRailCtrlTask,  //Create the start task. 
                    (void          * )0,
                    (OS_STK        * )&F_I_C_R_TaskStk[F_I_C_R_TASK_STK_SIZE - 1],
                    (INT8U           )F_I_C_R_TASK_PRIO,
                    (INT16U          )F_I_C_R_TASK_PRIO,
                    (OS_STK        * )&F_I_C_R_TaskStk[0],
                    (INT32U          )F_I_C_R_TASK_STK_SIZE,
                    (void          * )0,
                    (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));                     

 
   
    OSTaskCreateExt((void (*)(void *))DightRPTask,  //Create the start task. 
                    (void          * )0,
                    (OS_STK        * )&DightRPTaskStk[DIGIT_RP_TASK_STK_SIZE - 1],
                    (INT8U           )DIGIT_RP_TASK_PRIO,
                    (INT16U          )DIGIT_RP_TASK_PRIO,
                    (OS_STK        * )&DightRPTaskStk[0],
                    (INT32U          )DIGIT_RP_TASK_STK_SIZE,
                    (void          * )0,
                    (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));  

    TASK_CREAT(SendIOStateFrameTask, SendIOStateFrameTaskStk, SEND_IO_STATE_FRAME_TASK_STK_SIZE, SEND_IO_STATE_FRAME_TASK_STK_PRIO);

    
    TASK_CREAT(RevCtrlTask, RevCtrlTaskStk, REV_CTRL_TASK_STK_SIZE, REV_CTRL_TASK_PRIO);
    OSTaskSuspend(REV_CTRL_TASK_PRIO); //先挂起该任务

   
    TASK_CREAT(RevMeasRunTask, RevMeasRunTaskStk, REV_MEAS_TASK_STK_SIZE, REV_MEAS_TASK_PRIO);


    #endif
    //TASK_CREAT(TmptrCtrlMainTask, TmptrCtrlMainTaskStk, TMPTR_CTRL_MAIN_TASK_STK_SIZE, TMPTR_CTRL_MAIN_TASK_PRIO);
    
    //OSTaskSuspend(REV_CTRL_TASK_PRIO);

    //---------------------------------------------------------------------------------------------
    
   
    
    ///OSStatInit();
    ////PWM_Start(1);
    while(TRUE_P) 
    {                                          
        FrameParseExec();
        //printf("this is the first\n");
        OSTimeDlyHMSM(0, 0, 0, 50);
    }


}







