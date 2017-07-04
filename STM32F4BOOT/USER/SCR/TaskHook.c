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
#include "mystd.h"
#include "m_config.h"
#include "StatTask.h"

/* 
*********************************************************************************************************
*********************************************************************************************************
*                                          uC/OS-II APP HOOKS
*********************************************************************************************************
*********************************************************************************************************
*/

#if (OS_APP_HOOKS_EN > 0)
/*
*********************************************************************************************************
*                                      TASK CREATION HOOK (APPLICATION)
*
* Description : This function is called when a task is created.
*
* Argument(s) : ptcb   is a pointer to the task control block of the task being created.
*
* Note(s)     : (1) Interrupts are disabled during this call.
*********************************************************************************************************
*/

void  App_TaskCreateHook (OS_TCB *ptcb)
{
#if ((APP_OS_PROBE_EN   == DEF_ENABLED) && \
     (OS_PROBE_HOOKS_EN == DEF_ENABLED))
    OSProbe_TaskCreateHook(ptcb);
#endif
}

/*
*********************************************************************************************************
*                                    TASK DELETION HOOK (APPLICATION)
*
* Description : This function is called when a task is deleted.
*
* Argument(s) : ptcb   is a pointer to the task control block of the task being deleted.
*
* Note(s)     : (1) Interrupts are disabled during this call.
*********************************************************************************************************
*/

void  App_TaskDelHook (OS_TCB *ptcb)
{
    (void)ptcb;
}

/*
*********************************************************************************************************
*                                      IDLE TASK HOOK (APPLICATION)
*
* Description : This function is called by OSTaskIdleHook(), which is called by the idle task.  This hook
*               has been added to allow you to do such things as STOP the CPU to conserve power.
*
* Argument(s) : none.
*
* Note(s)     : (1) Interrupts are enabled during this call.
*********************************************************************************************************
*/

#if OS_VERSION >= 251
void  App_TaskIdleHook (void)
{
}
#endif

/*
*********************************************************************************************************
*                                        STATISTIC TASK HOOK (APPLICATION)
*
* Description : This function is called by OSTaskStatHook(), which is called every second by uC/OS-II's
*               statistics task.  This allows your application to add functionality to the statistics task.
*
* Argument(s) : none.
*********************************************************************************************************
*/




TASK_USER_DATA  TaskUserData[7];


void  App_TaskStatHook (void)
{
 #ifdef PRINT_TASK_STAT   
    char    s[80];
    INT8U   i;
    INT32U  total;
    INT8U   pct;


    total = 0L;                                          /* Totalize TOT. EXEC. TIME for each task */
    for (i = 0; i < 4; i++) {
        total += TaskUserData[i].TaskTotExecTime;
        ///DispTaskStat(i);                                 /* Display task data                      */
        sprintf(s, "%-18s %05u  %5u     %10ld\n",
            TaskUserData[i].TaskName,
            TaskUserData[i].TaskCtr,
            TaskUserData[i].TaskExecTime,
            TaskUserData[i].TaskTotExecTime);
        
        printf("%s\n",s);
    }
    if (total > 0) {
        for (i = 0; i < 4; i++) {                        /* Derive percentage of each task         */
            pct = 100 * TaskUserData[i].TaskTotExecTime / total;
            ////sprintf(s, "%3d %%", pct);
            printf("%3d %%\n",pct);
            ////PC_DispStr(62, i + 11, s, DISP_FGND_BLACK + DISP_BGND_LIGHT_GRAY);
        }
    }
    if (total > 1000000000L) {                           /* Reset total time counters at 1 billion */
        for (i = 0; i < 4; i++) {
            TaskUserData[i].TaskTotExecTime = 0L;
        }
    }
  
  #endif
    
}

/*
*********************************************************************************************************
*                                        TASK SWITCH HOOK (APPLICATION)
*
* Description : This function is called when a task switch is performed.  This allows you to perform other
*               operations during a context switch.
*
* Argument(s) : none.
*
* Note(s)     : (1) Interrupts are disabled during this call.
*
*               (2) It is assumed that the global pointer 'OSTCBHighRdy' points to the TCB of the task that
*                   will be 'switched in' (i.e. the highest priority task) and, 'OSTCBCur' points to the
*                  task being switched out (i.e. the preempted task).
*********************************************************************************************************
*/

#if OS_TASK_SW_HOOK_EN > 0
void  App_TaskSwHook (void)
{
#if ((APP_OS_PROBE_EN   == DEF_ENABLED) && \
     (OS_PROBE_HOOKS_EN == DEF_ENABLED))
    OSProbe_TaskSwHook();
#endif
    
    INT16U           time = 0; //time...
    TASK_USER_DATA  *puser;


  //  time  = PC_ElapsedStop();                    /* This task is done                                  */
  //  PC_ElapsedStart();                           /* Start for next task                                */
    puser = OSTCBCur->OSTCBExtPtr;               /* Point to used data                                 */
    if (puser != (TASK_USER_DATA *)0) {
        puser->TaskCtr++;                        /* Increment task counter                             */
        puser->TaskExecTime     = time;          /* Update the task's execution time                   */
        puser->TaskTotExecTime += time;          /* Update the task's total execution time             */
    }
}
#endif

/*
*********************************************************************************************************
*                                     OS_TCBInit() HOOK (APPLICATION)
*
* Description : This function is called by OSTCBInitHook(), which is called by OS_TCBInit() after setting
*               up most of the TCB.
*
* Argument(s) : ptcb    is a pointer to the TCB of the task being created.
*
* Note(s)     : (1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/

#if OS_VERSION >= 204
void  App_TCBInitHook (OS_TCB *ptcb)
{
    (void)ptcb;
}
#endif

/*
*********************************************************************************************************
*                                        TICK HOOK (APPLICATION)
*
* Description : This function is called every tick.
*
* Argument(s) : none.
*
* Note(s)     : (1) Interrupts may or may not be ENABLED during this call.
*********************************************************************************************************
*/

#if OS_TIME_TICK_HOOK_EN > 0
void  App_TimeTickHook (void)
{
#if ((APP_OS_PROBE_EN   == DEF_ENABLED) && \
     (OS_PROBE_HOOKS_EN == DEF_ENABLED))
    OSProbe_TickHook();
#endif
}
#endif
#endif
