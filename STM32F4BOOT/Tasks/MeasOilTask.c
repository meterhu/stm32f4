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
#include "mystd.h"  
#include "m_config.h"
#include "stm32f4xx.h"
#include "ucos_ii.h"
#include "SysAppConfig.h"
#include "UserLib.h"
#include "FramePack.h"
#include "CommHandle.h"
#include "ADS123x.h"
#include "SimuI2C.h"

volatile u16  CylNetWeightPri;
volatile s32  tmp_ad_v_pri[12] = {0};
extern volatile s32  tmp_cynet_wight[12];
SAVE_PARAM_ONE  save_para_one;
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
void MeasOilTask(void *p_arg)
{

    uint16_t   buff[14] = {0};
    s32 tmp_suttle;

    I2C_Simu_BuffRead((u8 *)&save_para_one, 0, sizeof(SAVE_PARAM_ONE));   

    //buff[0] = 0x1000; //save_p.CylNetNum;
	
    while(TRUE_P)
    {
        //���ADԭʼֵ
        tmp_ad_v_pri[ADS123X_CHNNL4] = ReadADS123x(ADS123X_CHNNL4) ;
        printf("AS123x is %d \n", tmp_ad_v_pri[ADS123X_CHNNL4]);
        //buff[2]	= tmp_ad_v_pri[ADS123X_CHNNL4];	
		
        //����AD��ֵ�����������ز�ֵ
        if(tmp_cynet_wight[ADS123X_CHNNL4])
        {
            tmp_suttle = tmp_ad_v_pri[ADS123X_CHNNL4] - tmp_cynet_wight[ADS123X_CHNNL4];//���� = ����ֵ - Ƥ��
        }
        else
        {
            tmp_suttle = tmp_ad_v_pri[ADS123X_CHNNL4] - save_para_one.CylNetWeight;	//��AD��ֵ������ = ����ֵ - Ƥ��

        }
        
        //����ʵ������ֵ����λ0.1 ml
        if(tmp_suttle < 0)
        {
            tmp_suttle = 0;
        }
        else 
        {
             tmp_suttle  /= save_para_one.OilFactor;   //ʵ������ֵ
        }
        
        //������ʵ������ֵ
        save_para_one.OilWeight =  tmp_suttle; 
        
        buff[0] = save_para_one.OilWeight ;
				
        //ֻ����ʵ�����õ���Ͳ��
        SendFrame(OIL_Q_V_CW, (uint8_t *)&buff, 3 * 2);   //
        
        printf("OilWeight is %d\n", save_para_one.OilWeight);
        
        OSTimeDlyHMSM(0, 0, 0, 400);
    
    }

}

