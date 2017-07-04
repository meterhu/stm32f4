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
 *                ����: ������
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
void  DigitRP(uint8_t  left_right_drp)
{
    ///OSMboxPost(DIGIT_RP_MBOX, (void *)left_right_drp);
    OSMboxPostOpt(DIGIT_RP_MBOX, (void *)left_right_drp,OS_POST_OPT_NO_SCHED);
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
void  DightRPTask1(void *p_arg)
{

    void      *rt;
    INT8U     err;
    uint32_t  msg;
  
	static   uint8_t   buff[4];
    
    
    while(TRUE_P)
    {
        //������ĳ������ź�����ͬ��
        rt = OSMboxPend(DIGIT_RP_MBOX, 0, &err);  //ת���ж��﷢����Ϣ OSMboxPost(OIL_CNT_MBOX, (void *)oil_cnt);
        if (err == OS_ERR_NONE)
        {
            //�ж����ҵ�λ��
            if ((((uint32_t)rt) & 0xFF) == LEFT_DRP)
            {
                LeftDigitRP();
            }
            else
            {
                RightDigitRP();

            }
            
            AppData.digit_rp_cnt = DigitRPCnt;
            
            if (!(AppData.rev_run_state & REV_STOP)) //��ǰת�ٿ���״̬��������
            {
                AppData.set_rev = DigitRPCnt; 
                ///TransduRun(FWD_RUN, AppData.set_rev);
                
                //������Ϣ����Ƶ����������     
                msg = (AppData.set_rev << 16)| AppData.rev_run_state;
                OSMboxPostOpt(TRANS_CTRL_MBOX, (void *)(msg),OS_POST_OPT_NO_SCHED);
            }  
            
            //�ϴ���λ��ֵ����λ��            
            buff[0] = DigitRPCnt & 0xFF;
            buff[1] = (DigitRPCnt >> 8) & 0xFF;  
            buff[3] = 0;
            buff[2] = 0;
            
            //OS_Trace("DightRPTask\n");
            ///OS_Trace("DigitRPCnt = %d\n",DigitRPCnt);

            buff[2] = BIT7; //��7λΪ1ʱ�������
            SendFrame(DR_CNT_CW, (uint8_t *)buff, 4);   //2��2���ֽ�,16bit
                      
                  
        }
        
        OSTimeDlyHMSM(0, 0, 0, 40);

    }        

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
            //�ж����ҵ�λ��
//             if (dight_rp == LEFT_DRP)
//             {
//                 LeftDigitRP();
//             }
//             else
//             {
//                 RightDigitRP();

//             }
            
            AppData.digit_rp_cnt = DigitRPCnt;
            
            if (!(AppData.rev_run_state & REV_STOP)) //��ǰת�ٿ���״̬��������
            {
                AppData.set_rev = DigitRPCnt; 
                ///TransduRun(FWD_RUN, AppData.set_rev);
                
                //������Ϣ����Ƶ����������     
                msg = (AppData.set_rev << 16)| AppData.rev_run_state;
                //OSMboxPostOpt(TRANS_CTRL_MBOX, (void *)(msg),OS_POST_OPT_NO_SCHED);
            }  
            
            //�ϴ���λ��ֵ����λ��            
            buff[0] = DigitRPCnt & 0xFF;
            buff[1] = (DigitRPCnt >> 8) & 0xFF;  
            buff[3] = 0;
            buff[2] = 0;
            
            //OS_Trace("DightRPTask\n");
            ///OS_Trace("DigitRPCnt = %d\n",DigitRPCnt);

            buff[2] = BIT7; //��7λΪ1ʱ�������
            if(DigitRPCnt != PreDigitRPCnt)
            {
                //������Ϣ����Ƶ����������   
                OSMboxPostOpt(TRANS_CTRL_MBOX, (void *)(msg),OS_POST_OPT_NO_SCHED);
                SendFrame(DR_CNT_CW, (uint8_t *)buff, 4);   //2��2���ֽ�,16bit    
            }
            PreDigitRPCnt = DigitRPCnt;
            
                      
                  
        }
        
        OSTimeDlyHMSM(0, 0, 0, 40);

    }        

}










