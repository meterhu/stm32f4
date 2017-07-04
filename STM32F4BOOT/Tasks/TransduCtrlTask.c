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
#include "OS_Trace.h"



//================================================================================================
OS_EVENT    *TRANS_CTRL_MBOX;  
OS_EVENT    *TransCtrlQ;
void   *TransCtrlQBuff[30]; //
//================================================================================================



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
void TransduCtrlTask(void *p_arg)
{

    void      *rt;
    INT8U     err;
    uint32_t   ctrl_w;
	
    
    ///TransCtrlQ   =  OSQCreate(TransCtrlQBuff,   30);
    
    while(TRUE_P)
    {
        //������ĳ������ź�����ͬ��
        rt = OSMboxPend(TRANS_CTRL_MBOX, 0, &err);  //�����ж��﷢����Ϣ OSMboxPost(REV_CNT_MBOX, (void *)in_state);
        if (err == OS_ERR_NONE)
        {
            ctrl_w = (uint32_t)rt; 
            
            //ת��ֵ:��16bit
            //������:��16bit            
            //TransduCtrl(ctrl_w & 0xFFFF, ctrl_w >> 16); 
            TransduCtrlSimu(ctrl_w & 0xFFFF, ctrl_w >> 16);
            
            OS_Trace("TransduCtrlTask\n");
            OS_Trace("TransduCtrl %d\n",ctrl_w & 0xFFFF);
            
            OSTimeDlyHMSM(0, 0, 0, 600);
        
        }

    }

}

