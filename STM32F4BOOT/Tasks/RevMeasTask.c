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

extern  OS_EVENT    *REV_CNT_MBOX;           //ͬ��
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
void InitOilCntMisc(void)
{
    OilCntState =  OIL_CNT_STOP_STATE;
    
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
void RevCntTask(void *p_arg)
{

    void      *rt;
    INT8U     err;
    //uint32_t   rev_advangle = 0;
    uint32_t pre_speed_rpm = 0;
    
    while(TRUE_P)
    {
        //������ĳ������ź�����ͬ��
//        rt = OSMboxPend(REV_CNT_MBOX, 0, &err);  //�����ж��﷢����Ϣ OSMboxPost(REV_CNT_MBOX, (void *)in_state);
//        if (err == OS_ERR_NONE)
        {
            //rev_advangle =*(uint32_t *)rt;
            ///OS_Trace("SpeedRPM = %d\n", rt);
            if(SpeedRPM != pre_speed_rpm)
            {     
                SendFrame(REV_V_CW, (uint8_t *)&SpeedRPM, 4);   //2��2���ֽ�,16bit
            }
            pre_speed_rpm = SpeedRPM;
              
            OSTimeDlyHMSM(0, 0, 0, 100);

        
        }

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
void  RevCtrlTask(void *p_arg)
{
    void      *rt;
    INT8U     err;
    
    uint32_t  rev_ctrl_w;
          
    while(TRUE_P)
    {
        rt = OSMboxPend(REV_CTRL_MBOX, 0, &err);  //�����ж��﷢����Ϣ OSMboxPost(REV_CTRL_MBOX, (void *)in_state);

        if (err == OS_ERR_NONE)
        {
            rev_ctrl_w = (uint32_t)rt;
            
            //ת��ֵ:��16bit
            //������:��16bit
            // 0	��ת
            // 1	��ת
            // 2	ֹͣ
            // 3	�Ƿ�ʹ��PID����
            // 4	��λ������״̬��1-��Ч 0-��Ч
            AppData.set_rev = rev_ctrl_w >> 16;
                        
            if (rev_ctrl_w & (0x01 << 4)) //��λ������
            {
                AppData.set_rev = AppData.digit_rp_cnt;
                //InitSpeedIO();
            }
            
            AppData.digit_rp_en = (rev_ctrl_w >> 4) & 0x01; //��λ��ʹ��λ
            AppData.pid_en      = (rev_ctrl_w >> 3) & 0x01; //pidʹ��λ
            
            if (rev_ctrl_w & 0x01) //��ת
            {
                ///TransduRun(FWD_RUN, AppData.set_rev);
                AppData.rev_run_state = FWD_RUN;
                
            }
            else if(rev_ctrl_w & 0x02) //��ת
            {
               
                ///TransduRun(REV_RUN, AppData.set_rev);
                AppData.rev_run_state = REV_RUN;
                
            } 
            else if(rev_ctrl_w & 0x04) //ֹͣ
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
void RevCtrl(uint32_t rev_ctrl)
{
 
    OSMboxPost(REV_CTRL_MBOX, (void *)rev_ctrl);
 
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
void RevCnt(uint32_t *rev)
{
 
    OSMboxPost(REV_CNT_MBOX, (void *)rev);
 
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
void  OilCntTask(void *p_arg)
{
    void      *rt;
    INT8U     err;
    //uint16_t  state;

	static   uint8_t   buff[4];
    
    volatile uint32_t   tmp32;
    volatile uint32_t   oil_wight;
    
    //��λ������"��������"����󣬴���������.�ȳ�ʼ�����µ�ģ�����--��Щ����������ĺ���
    //OilCnt��ʹ�ã���OilCntΪת���жϵ���
    OilCntState = OIL_CNT_PRE_START_STATE;
    OilCntSetNum   = (int16_t)((uint32_t)p_arg);


    //state = 0; 
    while(TRUE_P)
    {
        //������ĳ������ź�����ͬ��
        //rt = OSMboxPend(OIL_CNT_MBOX, 0, &err);  //ת���ж��﷢����Ϣ OSMboxPost(OIL_CNT_MBOX, (void *)oil_cnt);
        //rt = OSQPend(OIL_CNT_Q, 0, &err);  //ת���ж��﷢����Ϣ OSMboxPost(OIL_CNT_MBOX, (void *)oil_cnt);
        //if (err == OS_ERR_NONE)
        {

            //tmp32 = (uint32_t)rt;

            //�ͱü���ֵ
            buff[0] = CurOilCnt & 0xFF;
            buff[1] = (CurOilCnt >> 8) & 0xFF;  
            buff[3] = 0;
            buff[2] = 0;
            
            //OS_Trace("OilCntTask\n");
            //OS_Trace("Oil Cnt = %d\n", tmp32 & 0xFFFF);
            
            if (OilCntState & OIL_CNT_OVER_STATE)   //�ͱü�������״̬λ
            {
                buff[2] = BIT7; //��7λΪ1ʱ�������
                SendFrame(OIL_CNT_PROC_CW, (uint8_t *)buff, 4);   //2��2���ֽ�,16bit
                
                OSTimeDlyHMSM(0, 0, 3, 0);    //�ȴ�ע���ȶ�����������
                
                              
                //�ϴ����أ������������Ա�
                //......
                
                OSTaskDel(OS_PRIO_SELF);
            }
            
            //�ϴ���������ֵ��״̬λ
            SendFrame(OIL_CNT_PROC_CW, (uint8_t *)buff, 4);   //2��2���ֽ�,16bit
            OSTimeDlyHMSM(0, 0, 0, 100); 
       
        }

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
void OilCnt(void)
{
     //ע�⣬�˺�����ת���ж��е���

     //static uint16_t oil_cnt;
     ///static   uint8_t   buff[4];

     if (OilCntState == OIL_CNT_PRE_START_STATE)
     {
         //oil_weight_pri = tmp_ad_v_pri[ADS123X_CHNNL4];   //Ƥ��       
         tmp_cynet_wight[ADS123X_CHNNL4] = tmp_ad_v_pri[ADS123X_CHNNL4];   //Ƥ��  
         DelayMS(500);
     
         CurOilCnt = 0;
         OilCntState = OIL_CNT_START_STATE;
         
         //����򿪵��Ͱ�
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
void RevCnt_x(uint32_t tm_cnt)
{
    uint32_t  rev;
    
   
    rev = (1000 * 1000 * 60) /  tm_cnt; //1000* 1000--1�������us��;60=1����60��
    //rev = (1 * 60 * 1000 * 1000 * 1000) / ((41 * us_cnt) / 10);
    //rev = 60000000000L / (4 * us_cnt);
    rev = 60000000000L / (41 * tm_cnt); //41=1000ns / 24(24M)
    OSMboxPost(REV_CNT_MBOX, (void *)rev);
   /// TRACE("rev=%d\r\n", rev);

    // ת/���� = 1 ���� / ÿת��ʱ(����)  F_PCLK = 24,000,000,Ԥ��Ƶϵ��Ϊ0
    //         = 1 * 60 * 1000 * 1000 * 1000(ns) / [[1,000,000,000 / F_PCLK] * SpdTmCnt];
    //         = 1 * 60 * 1000 * 1000 * 1000(ns) / [[100 / 6] * SpdTmCnt](ns);
       
       

}




