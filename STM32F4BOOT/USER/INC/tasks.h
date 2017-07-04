//===============================================================================================
/************************************************************************************************
 *                                                                                                     
 *            文件名称: Std.h             
 *
 *                摘要: 标准头文件
 *
 *            创建时间: 2006-09-20  
 *
 *            修改时间: 2006-09-20
 *            修改原因:
 *
 *            
 *
 *                作者: 杨卫华
 *
 *************************************************************************************************/
 //===============================================================================================
#ifndef  USER_TASKS_H
#define  USER_TASKS_H

#include"os_cpu.h"   					  
#define    APP_TASK_START_STK_SIZE  512

 //===============================================================================
#define    GUI_TASK_STK_SIZE   (512)

 //===============================================================================
#define    APP_TASK_START_PRIO_1      6
#define    APP_TASK_START_PRIO_2      7
#define    APP_TASK_START_PRIO_3      8
#define    APP_TASK_START_PRIO_4      9



//=================================================================================================
//*************************************************************************************************
//宏定义
// #define  MAIN_TASK_PRIO                            3
// #define  AD_SAMPL_TASK_PRIO                        4
// #define  TMPTR_SAMPL_TASK_PRIO                     5
// #define  TRANS_CTRL_TASK_PRIO                      13
// #define  DIGIT_RP_TASK_PRIO                        6 
// #define  IN_OUT_TASK_STK_PRIO                      8
// #define  TMPTR_CTRL_TASK_PRIO                      10
// #define  REV_CTRL_TASK_PRIO                        12
// #define  REV_CNT_TASK_TASK_PRIO                    14
// #define  MEAS_OIL_TASK_PRIO                        15
// #define  OIL_CNT_TASK_PRIO                         16


// //=====================================================================================
// #define  MAIN_TASK_STK_SIZE                        512
// #define  TMPTR_SAMPL_TASK_STK_SIZE                 512
// #define  AD_SAMPL_TASK_STK_SIZE                    512
// #define  REV_CNT_TASK_STK_SIZE                     128
// #define  MEAS_OIL_TASK_STK_SIZE                    128
// #define  TRANS_CTRL_TASK_STK_SIZE                  128
// #define  REV_MEAS_TASK_STK_SIZE                    64
// #define  OIL_CNT_TASK_STK_SIZE                     64
// #define  DIGIT_RP_TASK_STK_SIZE                    256                          
// #define   REV_CTRL_TASK_STK_SIZE                   128
// #define   IN_OUT_TASK_STK_SIZE                     64
// #define   TMPTR_CTRL_TASK_STK_SIZE                 64

#define  MAIN_TASK_PRIO                            3
#define  MUTEX1_TASK_PRIO                          4
#define  OIL_CNT_TASK_PRIO                         10
#define  REV_CNT_TASK_TASK_PRIO                    11
#define  DIGIT_RP_TASK_PRIO                        12 
#define  IN_OUT_TASK_STK_PRIO                      13
#define  REV_CTRL_TASK_PRIO                        14
#define  TRANS_CTRL_TASK_PRIO                      15
#define  TMPTR_CTRL_TASK_PRIO                      16
#define  AD_SAMPL_TASK_PRIO                        17
#define  TMPTR_SAMPL_TASK_PRIO                     18
#define  MEAS_OIL_TASK_PRIO                        19


//=====================================================================================
#define  MAIN_TASK_STK_SIZE                        512
#define  AD_SAMPL_TASK_STK_SIZE                    128
#define  TMPTR_SAMPL_TASK_STK_SIZE                 128
#define  DIGIT_RP_TASK_STK_SIZE                    256   
#define  IN_OUT_TASK_STK_SIZE                      64
#define  TMPTR_CTRL_TASK_STK_SIZE                  64
#define  REV_CTRL_TASK_STK_SIZE                    128
#define  TRANS_CTRL_TASK_STK_SIZE                  128
#define  REV_CNT_TASK_STK_SIZE                     128
#define  MEAS_OIL_TASK_STK_SIZE                    128
#define  REV_MEAS_TASK_STK_SIZE                    64
#define  OIL_CNT_TASK_STK_SIZE                     64

//===============================================================================
#define   TMPTR_CTRL_TASK_I                        0
#define   OIL_CNT_TASK_I                           1
#define   REV_MEAS_TASK_I                          2


//===============================================================================
 
 
 //=====================================================================================
#define  TASK_CREAT(task, stk, stk_size, prio)\
    OSTaskCreateExt((void (*)(void *))(task),\
                    (void          * )0,\
                    (OS_STK        * )&(stk)[(stk_size) - 1],\
                    (INT8U           )(prio),\
                    (INT16U          )(prio),\
                    (OS_STK        * )&(stk)[0],\
                    (INT32U          )(stk_size),\
                    (void          * )0,\
                    (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));

#define  TASK_DEFAULT_CREAT(task, stk, stk_size, prio)\
    OSTaskCreateExt((void (*)(void *))(task),\
                    (void          * )0,\
                    (OS_STK        * )&(stk)[(stk_size) - 1],\
                    (INT8U           )(prio),\
                    (INT16U          )(prio),\
                    (OS_STK        * )&(stk)[0],\
                    (INT32U          )(stk_size),\
                    (void          * )0,\
                    (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));
                    

#define  TASK_CREAT_EXT(task, stk, stk_size, prio,param)\
    OSTaskCreateExt((void (*)(void *))(task),\
                    (void          * )(param),\
                    (OS_STK        * )&(stk)[(stk_size) - 1],\
                    (INT8U           )(prio),\
                    (INT16U          )(prio),\
                    (OS_STK        * )&(stk)[0],\
                    (INT32U          )(stk_size),\
                    (void          * )0,\
                    (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));



#define  _CREATE_TASK_EXT(id, para)\
    OSTaskCreateExt((void (*)(void *))(TaskInfo[id].task),\
                    (void          * )(para),\
                    (OS_STK        * )&TaskInfo[id].stk[TaskInfo[id].stk_size - 1],\
                    (INT8U           )TaskInfo[id].prio,\
                    (INT16U          )TaskInfo[id].prio,\
                    (OS_STK        * )&TaskInfo[id].stk[0],\
                    (INT32U          )TaskInfo[id].stk_size,\
                    (void          * )0,\
                    (INT16U          )(OS_TASK_OPT_STK_CLR | OS_TASK_OPT_STK_CHK));   
                    

#define  _CREATE_TASK(id, para)\
        OSTaskCreate((void (*)(void *))(TaskInfo[id].task),\
                    (void          * )(para),\
                    (OS_STK        * )&TaskInfo[id].stk[TaskInfo[id].stk_size - 1],\
                    (INT8U           )TaskInfo[id].prio);
                    
 //=================================================================================================
//*************************************************************************************************
//数据类型定义
typedef struct _TASK_INFO
{
    void   (*task)(void *p_arg);
    OS_STK   *stk;
    INT8U    prio;
    INT32U   stk_size;

}TASK_INFO;

typedef struct _REV_MEAS_INFO
{
    uint32_t state;
	void     *pdata;

}REV_MEAS_INFO;


extern void CreateTasks(void);
extern void CreateTasksEx(void);


#endif
