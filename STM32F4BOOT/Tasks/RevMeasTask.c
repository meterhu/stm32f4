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
#include "ADS123x.h"

#include "Speed.h"



//================================================================================================
uint8_t    MeasValue[256];
uint16_t   AngleValue[360];

uint16_t   RevPreq;

//================================================================================================
OS_EVENT    *REV_MEAS_RUN_MBOX;
OS_EVENT    *VIB_Q_MEAS_OVER_MBOX;

extern  OS_EVENT    *REV_CNT_MBOX;           //同步
extern  OS_EVENT    *OIL_CNT_MBOX;
extern  OS_EVENT    *REV_CTRL_MBOX;
extern  OS_EVENT    *TRANS_CTRL_MBOX;
extern  OS_EVENT    *OIL_CNT_Q;
static uint8_t   OilCntState;
static uint16_t  OilCntSetNum;

extern uint32_t   SpeedRPM;
static uint16_t   CurOilCnt;

extern SAVE_PARAM_ONE  save_para_one;
extern volatile s32  tmp_ad_v_pri[12];
extern volatile s32  tmp_cynet_wight[12];
uint32_t  oil_weight_pri;
uint32_t  oil_weight_last;

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
void InitOilCntMisc(void)
{
    OilCntState =  OIL_CNT_STOP_STATE;
    
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
void RevCntTask(void *p_arg)
{

    void      *rt;
    INT8U     err;
    //uint32_t   rev_advangle = 0;
    uint32_t pre_speed_rpm = 0;
    
    while(TRUE_P)
    {
        //由下面的程序发送信号量来同步
//        rt = OSMboxPend(REV_CNT_MBOX, 0, &err);  //开入中断里发送消息 OSMboxPost(REV_CNT_MBOX, (void *)in_state);
//        if (err == OS_ERR_NONE)
        {
            //rev_advangle =*(uint32_t *)rt;
            ///OS_Trace("SpeedRPM = %d\n", rt);
            if(SpeedRPM != pre_speed_rpm)
            {     
                SendFrame(REV_V_CW, (uint8_t *)&SpeedRPM, 4);   //2即2个字节,16bit
            }
            pre_speed_rpm = SpeedRPM;
              
            OSTimeDlyHMSM(0, 0, 0, 100);

        
        }

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
void  RevCtrlTask(void *p_arg)
{
    void      *rt;
    INT8U     err;
    
    uint32_t  rev_ctrl_w;
          
    while(TRUE_P)
    {
        rt = OSMboxPend(REV_CTRL_MBOX, 0, &err);  //开入中断里发送消息 OSMboxPost(REV_CTRL_MBOX, (void *)in_state);

        if (err == OS_ERR_NONE)
        {
            rev_ctrl_w = (uint32_t)rt;
            
            //转速值:高16bit
            //控制字:低16bit
            // 0	正转
            // 1	反转
            // 2	停止
            // 3	是否使用PID调节
            // 4	电位器开关状态：1-有效 0-无效
            AppData.set_rev = rev_ctrl_w >> 16;
                        
            if (rev_ctrl_w & (0x01 << 4)) //电位器调速
            {
                AppData.set_rev = AppData.digit_rp_cnt;
                //InitSpeedIO();
            }
            
            AppData.digit_rp_en = (rev_ctrl_w >> 4) & 0x01; //电位器使能位
            AppData.pid_en      = (rev_ctrl_w >> 3) & 0x01; //pid使能位
            
            if (rev_ctrl_w & 0x01) //正转
            {
                ///TransduRun(FWD_RUN, AppData.set_rev);
                AppData.rev_run_state = FWD_RUN;
                
            }
            else if(rev_ctrl_w & 0x02) //反转
            {
               
                ///TransduRun(REV_RUN, AppData.set_rev);
                AppData.rev_run_state = REV_RUN;
                
            } 
            else if(rev_ctrl_w & 0x04) //停止
            {
                //TransduStop(0);
                AppData.rev_run_state |= REV_STOP;
            }
            
            
            /////OS_Trace("RevCtrlTask Run\n");
            OSMboxPost(TRANS_CTRL_MBOX, (void *)((AppData.set_rev << 16)| AppData.rev_run_state));
            
        }
 

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
void RevCtrl(uint32_t rev_ctrl)
{
 
    OSMboxPost(REV_CTRL_MBOX, (void *)rev_ctrl);
 
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
void RevCnt(uint32_t *rev)
{
 
    OSMboxPost(REV_CNT_MBOX, (void *)rev);
 
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
void  OilCntTask(void *p_arg)
{
    void      *rt;
    INT8U     err;
    //uint16_t  state;

	static   uint8_t   buff[4];
    
    volatile uint32_t   tmp32;
    volatile uint32_t   oil_wight;
    
    //上位机发送"启动计数"命令后，创建该任务.先初始化以下的模块变量--这些变量在上面的函数
    //OilCnt中使用，且OilCnt为转速中断调用
    OilCntState = OIL_CNT_PRE_START_STATE;
    OilCntSetNum   = (int16_t)((uint32_t)p_arg);


    //state = 0; 
    while(TRUE_P)
    {
        //由下面的程序发送信号量来同步
        //rt = OSMboxPend(OIL_CNT_MBOX, 0, &err);  //转速中断里发送消息 OSMboxPost(OIL_CNT_MBOX, (void *)oil_cnt);
        //rt = OSQPend(OIL_CNT_Q, 0, &err);  //转速中断里发送消息 OSMboxPost(OIL_CNT_MBOX, (void *)oil_cnt);
        //if (err == OS_ERR_NONE)
        {

            //tmp32 = (uint32_t)rt;

            //油泵计数值
            buff[0] = CurOilCnt & 0xFF;
            buff[1] = (CurOilCnt >> 8) & 0xFF;  
            buff[3] = 0;
            buff[2] = 0;
            
            //OS_Trace("OilCntTask\n");
            //OS_Trace("Oil Cnt = %d\n", tmp32 & 0xFFFF);
            
            if (OilCntState & OIL_CNT_OVER_STATE)   //油泵计数进程状态位
            {
                buff[2] = BIT7; //第7位为1时计数完毕
                SendFrame(OIL_CNT_PROC_CW, (uint8_t *)buff, 4);   //2即2个字节,16bit
                
                OSTimeDlyHMSM(0, 0, 3, 0);    //等待注油稳定，测量油重
                
                              
                //上传油重，与油量参数对比
                //......
                
                OSTaskDel(OS_PRIO_SELF);
            }
            
            //上传油量计数值和状态位
            SendFrame(OIL_CNT_PROC_CW, (uint8_t *)buff, 4);   //2即2个字节,16bit
            OSTimeDlyHMSM(0, 0, 0, 100); 
       
        }

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
void OilCnt(void)
{
     //注意，此函数在转速中断中调用

     //static uint16_t oil_cnt;
     ///static   uint8_t   buff[4];

     if (OilCntState == OIL_CNT_PRE_START_STATE)
     {
         //oil_weight_pri = tmp_ad_v_pri[ADS123X_CHNNL4];   //皮重       
         tmp_cynet_wight[ADS123X_CHNNL4] = tmp_ad_v_pri[ADS123X_CHNNL4];   //皮重  
         DelayMS(500);
     
         CurOilCnt = 0;
         OilCntState = OIL_CNT_START_STATE;
         
         //下面打开挡油板
         //......
            
     }
     else if (OilCntState == OIL_CNT_START_STATE) 
     {
         CurOilCnt++;
         if (CurOilCnt == OilCntSetNum)
         {
              OilCntState = OIL_CNT_OVER_STATE; 
             
         }
         
         
         //OSQPost(OIL_CNT_Q, (void *)((OilCntState << 16) | oil_cnt));
         //
         //OSMboxPost(OIL_CNT_MBOX, (void *)((OilCntState << 16) | oil_cnt));
         

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
void RevCnt_x(uint32_t tm_cnt)
{
    uint32_t  rev;
    
   
    rev = (1000 * 1000 * 60) /  tm_cnt; //1000* 1000--1秒包含的us数;60=1分钟60秒
    //rev = (1 * 60 * 1000 * 1000 * 1000) / ((41 * us_cnt) / 10);
    //rev = 60000000000L / (4 * us_cnt);
    rev = 60000000000L / (41 * tm_cnt); //41=1000ns / 24(24M)
    OSMboxPost(REV_CNT_MBOX, (void *)rev);
   /// TRACE("rev=%d\r\n", rev);

    // 转/分钟 = 1 分钟 / 每转耗时(分钟)  F_PCLK = 24,000,000,预分频系数为0
    //         = 1 * 60 * 1000 * 1000 * 1000(ns) / [[1,000,000,000 / F_PCLK] * SpdTmCnt];
    //         = 1 * 60 * 1000 * 1000 * 1000(ns) / [[100 / 6] * SpdTmCnt](ns);
       
       

}




