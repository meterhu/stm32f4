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
#include "SysAppConfig.h"
#include "UserLib.h"
#include "OS_Trace.h"
#include "FramePack.h"
#include "CommHandle.h"
#include "main.h"
#include "tasks.h"
#include "RevAdvAngle.h"
#include "DigitRP.h"
#include "Transdu.h"
#include "OS_Trace.h"



//================================================================================================
OS_EVENT    *TRANS_CTRL_MBOX;  
OS_EVENT    *TransCtrlQ;
void   *TransCtrlQBuff[30]; //
//================================================================================================



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
void TransduCtrlTask(void *p_arg)
{

    void      *rt;
    INT8U     err;
    uint32_t   ctrl_w;
	
    
    ///TransCtrlQ   =  OSQCreate(TransCtrlQBuff,   30);
    
    while(TRUE_P)
    {
        //由下面的程序发送信号量来同步
        rt = OSMboxPend(TRANS_CTRL_MBOX, 0, &err);  //开入中断里发送消息 OSMboxPost(REV_CNT_MBOX, (void *)in_state);
        if (err == OS_ERR_NONE)
        {
            ctrl_w = (uint32_t)rt; 
            
            //转速值:高16bit
            //控制字:低16bit            
            //TransduCtrl(ctrl_w & 0xFFFF, ctrl_w >> 16); 
            TransduCtrlSimu(ctrl_w & 0xFFFF, ctrl_w >> 16);
            
            OS_Trace("TransduCtrlTask\n");
            OS_Trace("TransduCtrl %d\n",ctrl_w & 0xFFFF);
            
            OSTimeDlyHMSM(0, 0, 0, 600);
        
        }

    }

}

