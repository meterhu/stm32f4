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
#include "mystd.h"  
#include "m_config.h"
#include "UserLib.h"
#include "SysAppConfig.h"
#include "Queue.h"
#include "Buff.h"
#include "UART.h"
#include "FramePack.h"
#include "CommHandle.h"
#include "main.h"
#include "tasks.h"
#include "CommTaskProc.h"
#include "RevAdvAngle.h"
#include "PWM.h"
#include "OS_Trace.h"
#include "SimuI2C.h"
#include "ADS123x.h"
#include "CPUID.h"


extern     SAVE_PARAM_ONE  save_para_one;

extern TASK_INFO   TaskInfo[];
u16 Bit_Buff[12] = {BIT0,BIT1,BIT2,BIT3,BIT4,BIT5,BIT6,BIT7,BIT8,BIT9,BIT10,BIT11};	   //量缸号

