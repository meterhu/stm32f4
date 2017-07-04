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
#include "FramePack.h"
#include "CommHandle.h"
#include "main.h"
#include "tasks.h"
#include "DigitRP.h"
#include "OS_Trace.h"
#include "RevAdvAngle.h"
#include "Transdu.h"


extern  OS_EVENT    *DIGIT_RP_MBOX;           //
extern  OS_EVENT    *TransCtrlQ;
extern  int32_t       DigitRPCnt;
extern  OS_EVENT     *TRANS_CTRL_MBOX;  

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
void  DigitRP(uint8_t  left_right_drp)
{
    ///OSMboxPost(DIGIT_RP_MBOX, (void *)left_right_drp);
    OSMboxPostOpt(DIGIT_RP_MBOX, (void *)left_right_drp,OS_POST_OPT_NO_SCHED);
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
void  DightRPTask1(void *p_arg)
{

    void      *rt;
    INT8U     err;
    uint32_t  msg;
  
	static   uint8_t   buff[4];
    
    
    while(TRUE_P)
    {
        //由下面的程序发送信号量来同步
        rt = OSMboxPend(DIGIT_RP_MBOX, 0, &err);  //转速中断里发送消息 OSMboxPost(OIL_CNT_MBOX, (void *)oil_cnt);
        if (err == OS_ERR_NONE)
        {
            //判断左右电位器
            if ((((uint32_t)rt) & 0xFF) == LEFT_DRP)
            {
                LeftDigitRP();
            }
            else
            {
                RightDigitRP();

            }
            
            AppData.digit_rp_cnt = DigitRPCnt;
            
            if (!(AppData.rev_run_state & REV_STOP)) //当前转速控制状态在运行上
            {
                AppData.set_rev = DigitRPCnt; 
                ///TransduRun(FWD_RUN, AppData.set_rev);
                
                //发送消息到变频器控制任务     
                msg = (AppData.set_rev << 16)| AppData.rev_run_state;
                OSMboxPostOpt(TRANS_CTRL_MBOX, (void *)(msg),OS_POST_OPT_NO_SCHED);
            }  
            
            //上传电位器值到上位机            
            buff[0] = DigitRPCnt & 0xFF;
            buff[1] = (DigitRPCnt >> 8) & 0xFF;  
            buff[3] = 0;
            buff[2] = 0;
            
            //OS_Trace("DightRPTask\n");
            ///OS_Trace("DigitRPCnt = %d\n",DigitRPCnt);

            buff[2] = BIT7; //第7位为1时计数完毕
            SendFrame(DR_CNT_CW, (uint8_t *)buff, 4);   //2即2个字节,16bit
                      
                  
        }
        
        OSTimeDlyHMSM(0, 0, 0, 40);

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
void  DightRPTask(void *p_arg)
{

    void      *rt;
    INT8U     err;
    uint32_t  msg;
  
	static   uint8_t   buff[4];
    
    int32_t    PreDigitRPCnt  = 0;
    
    while(TRUE_P)
    {
       
        //if (err == OS_ERR_NONE)
        {
            //判断左右电位器
//             if (dight_rp == LEFT_DRP)
//             {
//                 LeftDigitRP();
//             }
//             else
//             {
//                 RightDigitRP();

//             }
            
            AppData.digit_rp_cnt = DigitRPCnt;
            
            if (!(AppData.rev_run_state & REV_STOP)) //当前转速控制状态在运行上
            {
                AppData.set_rev = DigitRPCnt; 
                ///TransduRun(FWD_RUN, AppData.set_rev);
                
                //发送消息到变频器控制任务     
                msg = (AppData.set_rev << 16)| AppData.rev_run_state;
                //OSMboxPostOpt(TRANS_CTRL_MBOX, (void *)(msg),OS_POST_OPT_NO_SCHED);
            }  
            
            //上传电位器值到上位机            
            buff[0] = DigitRPCnt & 0xFF;
            buff[1] = (DigitRPCnt >> 8) & 0xFF;  
            buff[3] = 0;
            buff[2] = 0;
            
            //OS_Trace("DightRPTask\n");
            ///OS_Trace("DigitRPCnt = %d\n",DigitRPCnt);

            buff[2] = BIT7; //第7位为1时计数完毕
            if(DigitRPCnt != PreDigitRPCnt)
            {
                //发送消息到变频器控制任务   
                OSMboxPostOpt(TRANS_CTRL_MBOX, (void *)(msg),OS_POST_OPT_NO_SCHED);
                SendFrame(DR_CNT_CW, (uint8_t *)buff, 4);   //2即2个字节,16bit    
            }
            PreDigitRPCnt = DigitRPCnt;
            
                      
                  
        }
        
        OSTimeDlyHMSM(0, 0, 0, 40);

    }        

}










