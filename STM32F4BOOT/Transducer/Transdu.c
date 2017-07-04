//===============================================================================================
/************************************************************************************************
 *                                                                                                     
 *            文件名称:              
 *
 *                摘要: 
 *
 *            创建时间: 2012-05-20  
 *
 *            修改时间: 2012-05-20
 *            修改原因:
 *
 *                     
 *
 *                作者: 杨卫华
 *
 *************************************************************************************************/
 //===============================================================================================
#include "stm32f4xx.h" 
#include "stm32f4_std_libs.h"
#include "mystd.h"  
#include "m_config.h"
#include "UserLib.h"
#include "SysAppConfig.h"
#include "UART.h"
#include "Transdu.h"
#include "ucos_ii.h"


//===============================================================================
//特殊的非RTU变频器协议通讯:东元,CD,MyTransducerRun,LGRun
void DongyuanRun(UINT16 rev_fwd, UINT16 rev_num);
void DongyuanStop(UINT16 rev_fwd);

void CDRun(UINT16 rev_fwd, UINT16 rev_num);
void CDStop(UINT16 rev_fwd);

void MyTransduRun(UINT16 rev_fwd, UINT16 rev_num);
void MyTransduStop(UINT16 rev_fwd);

void LGRun(UINT16 rev_fwd, UINT16 rev_num);
void LGStop(UINT16 rev_fwd);
void OpenLGWP(UINT32 param);
//=================================================================================

extern APP_DATA  AppData;

extern OS_EVENT    *PWM_TEST_SEM;  

//========================================================================================================================================================================================
UINT8       CurTrans = 0;	 //DEBUG
TRANSDU_SET TransduSet[] =
{
//  协议模式        名称        寄存器地址   频率命令       转速运行命令(正转,反转)   停止命令(正转,反转)    延时        奇偶校验  波特率   p_init,   p_run,   p_stop, init_param
//                            地址-频率比率  地址-命令字               地址-命令字                                                                 
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------  
   {STAND_RTU_MOD,  "VFD",       0x06,        0x0907,1,      0x2000, 0x0012,0x0022,    0x2000,0x0011,0x0021,  500000,     0,        9600,   NULL,     NULL    ,NULL,    0},    //台达
   {STAND_RTU_MOD,  "ESD",       0x06,        0x0502,1,      0x0279, 0x0007,0x0008,    0x0279,0x0006,0x0006,  500000,     0,        9600,   NULL,     NULL    ,NULL,    0},    //ESD
   {STAND_RTU_MOD,  "AD3000",    0x06,        0x0002,1,      0x0001, 0x0001,0x0002,    0x0001,0x0003,0x0003,  500000,     0,        9600,   NULL,     NULL    ,NULL,    0},    //AD3000
   {STAND_RTU_MOD,  "TAIAN",     0x06,        0x0102,1,      0x0101, 0x0001,0x0003,    0x0101,0x0000,0x0002,  500000,     0,        9600,   NULL,     NULL    ,NULL,    0},    //台安
   {STAND_RTU_MOD,  "NVF",       0x06,        0x2000,1,      0x1000, 0x0001,0x0002,    0x1000,0x0005,0x0005,  500000,     0,        9600,   NULL,     NULL    ,NULL,    0},    //NVF
   {STAND_RTU_MOD,  "sanji",     0x06,        0x2001,1,      0x2000, 0x0012,0x0022,    0x2000,0x0011,0x0021,  500000,     0,        9600,   NULL,     NULL    ,NULL,    0},    //sanji
   {STAND_RTU_MOD,  "yikong",    0x06,        0x2001,1,      0x2000, 0x0001,0x0003,    0x2000,0x0000,0x0002,  500000,     0,        9600,   NULL,     NULL    ,NULL,    0},    //yikong
 //{STAND_RTU_MOD,  "EDS",       0x06,        0x0502,1,      0x0279, 0x0007,0x0008,    0x0279,0x0006,0x0006,  500000,     0,        9600,   NULL,     NULL    ,NULL,    0},    //EDS
   

   //一下为非RTU的协议
   {NO_STAND_MOD,   "dongyuan",  0x00,        0x0000,1,      0x0000, 0x0000,0x0000,    0x0000,0x0000,0x0000,        0,    0,        9600,   NULL,     DongyuanRun    ,DongyuanStop,    0},   //东元
   {NO_STAND_MOD,   "CD",        0x00,        0x0000,1,      0x0000, 0x0000,0x0000,    0x0000,0x0000,0x0000,        0,    0,        9600,   NULL,     CDRun          ,CDStop,          0},         //CD
   {NO_STAND_MOD,   "MyTransdu", 0x00,        0x0000,1,      0x0000, 0x0000,0x0000,    0x0000,0x0000,0x0000,        0,    0,        9600,   NULL,     MyTransduRun   ,MyTransduStop,   0},  //       //CD
   {NO_STAND_MOD,   "LG",        0x00,        0x0000,1,      0x0000, 0x0000,0x0000,    0x0000,0x0000,0x0000,        0,    0,        9600,   OpenLGWP, LGRun          ,LGStop,          0},         

};                                                                                                                                                                                     //LG



//================================================================================================
/*************************************************************************************************
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
 *************************************************************************************************/
void InitTransdu(void)
{
     //主要包括初始化串口
    

	 TRANSDU_SET *pTr = &TransduSet[CurTrans];
    

     if (pTr->transdu_cmd_param.odd)        //重新初始化串口
     {
          
     }
     else
     {

     }

     if (pTr->p_init != NULL)
     {
         (*pTr->p_init)(pTr->init_param);

     }

	 INIT_TRANS_COM_CHNNL();   //奇校验

}


//================================================================================================
/*************************************************************************************************
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
 *************************************************************************************************/
UINT16 CRCCheck(UINT8 *data, UINT8 len)
{
    UINT8   j;
    UINT16  reg_crc;
    
    reg_crc = 0xFFFF;
    
    while (len--)
    {
        reg_crc ^= *data++;
        
        for (j = 0; j < 8; j++)
        {
            if (reg_crc & 0x01)
            {
                reg_crc = (reg_crc >> 1) ^ 0xA001;
                           
            }
            else
            {
                reg_crc >>= 1;
            
            }
        
        }
    
    
    }

    return reg_crc;


}



//================================================================================================
/*************************************************************************************************
 *
 *         函数名称:   RTUPackWRFrame
 *
 *         函数功能:   包装写寄存器命令帧
 *
 *         入口参数:  addr:寄存器地址;data:数据;pb:输出缓冲区
 *
 *         出口参数: 无
 *
 *             说明:
 *
 *************************************************************************************************/
void RTUPackWRFrame(UINT16 reg_addr, UINT16 addr, UINT16 data, UINT8 *pb)
{

    UINT16  tmp_crc;

	pb[0] = RTU_SLAVE_ADDR;
	pb[1] = reg_addr;    //写入单笔数据
	
	
    //地址先高后低
	pb[2] = addr >> 8;    
	pb[3] = (addr & 0xFF);
	
    //数据先高后低    
	pb[4] = data >> 8;   
	pb[5] = (data & 0xFF);
	    
	tmp_crc = CRCCheck(pb, 6);
	
    //注意此处低位在先    
	pb[6] = tmp_crc & 0xFF;   
	pb[7] = tmp_crc >> 8;
	
          
}

//================================================================================================
/*************************************************************************************************
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
 *************************************************************************************************/
void RTUWReg(UINT16 reg_addr, UINT16 addr, UINT16 data)
{
    UINT8    i;
    UINT8   buff[30];
  
    RTUPackWRFrame(reg_addr, addr, data, buff);

	//ENA_RS485_OUT();        
	for (i = 0; i < 8; i++)
	{
  	    TRANS_COM_SEND_BYTE(buff[i]);
	    Wait(30);
	    
	} 
    //DIS_RS485_OUT();
             
}


//================================================================================================
/*************************************************************************************************
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
 *************************************************************************************************/
void TransduRun(UINT16 rev_fwd, UINT16 rpm)
{
    
    TRANSDU_CMD *pTr;

    if (TransduSet[CurTrans].protocol_mod == STAND_RTU_MOD)
    {
    
        pTr = &(TransduSet[CurTrans].transdu_cmd);
    
        //RTUWReg(pTr->fre_cmd_addr,  pTr->fre_factor * rpm);
    	RTUWReg(pTr->reg_addr, pTr->fre_cmd_addr,  (rpm * 10) / 3);
        
        Wait(pTr->delay_cnt);   //两帧数据之间的延时
    
        if (rev_fwd == REV_RUN)
        {
             RTUWReg(pTr->reg_addr, pTr->rpm_cmd_addr,  pTr->rev_cmd);
        }
        else if (rev_fwd == FWD_RUN)
        {
             RTUWReg(pTr->reg_addr, pTr->rpm_cmd_addr,  pTr->fwd_cmd);
        }
    
    }
    else
    {
        (*TransduSet[CurTrans].p_run)(rev_fwd,rpm);

    }

    
}

//================================================================================================
/*************************************************************************************************
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
 *************************************************************************************************/
void TransduRunSimu(UINT16 rev_fwd, UINT16 rpm)
{
    
    rev_fwd = rev_fwd;
    rpm     = rpm;
    
}


//================================================================================================
/*************************************************************************************************
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
 *************************************************************************************************/
void TransduStop(UINT16 rev_fwd)
{
    
     TRANSDU_CMD *pTr = &TransduSet[CurTrans].transdu_cmd;

    if (TransduSet[CurTrans].protocol_mod == STAND_RTU_MOD)
    {

        if (rev_fwd == FWD_STOP)
        {
             RTUWReg(pTr->reg_addr, pTr->stop_cmd_addr,  pTr->fwd_stop_cmd);
        }
        else if (rev_fwd == REV_STOP)
        {
             RTUWReg(pTr->reg_addr, pTr->stop_cmd_addr,  pTr->rev_stop_cmd);
        } 
    
    }
    else
    {
        (*TransduSet[CurTrans].p_stop)(rev_fwd);

     } 
     
} 


//================================================================================================
/*************************************************************************************************
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
 *************************************************************************************************/
void TransduCtrl(UINT16 rev_fwd, UINT16 rpm)
{
     if (rev_fwd & REV_STOP)
     {
         TransduStop(rev_fwd);
         
     }
     else
     {
         TransduRun(rev_fwd, rpm);
         
     }
     
    
}
//================================================================================================
/*************************************************************************************************
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
 *************************************************************************************************/
void TransduCtrlSimu(UINT16 rev_fwd, UINT16 rpm)
{
     uint32_t us_p_rev;
     INT8U        err = err;
     static  uint8_t  tim3_state = DISABLE;
    
    
     if (rev_fwd & REV_STOP)
     {
         OSSemPend(PWM_TEST_SEM, 5, &err);
         if(err == OS_ERR_NONE)
         {
             //if(tim3_state == ENABLE)
             {
                 TIM_Cmd(TIM3, DISABLE);
                 tim3_state = DISABLE;
             }            
             
         }

         OSSemPost(PWM_TEST_SEM);         
     }
     else
     {
        OSSemPend(PWM_TEST_SEM, 5, &err);
         
        if(err == OS_ERR_NONE)
        {
            TIM_Cmd(TIM3, DISABLE);
            us_p_rev = US_P_MIN / (10 * AppData.set_rev);
            TIM_SetAutoreload(TIM3, us_p_rev);
            TIM_SetCompare4(TIM3,us_p_rev / 2);
            TIM_OC4FastConfig(TIM3, ENABLE);
            //if(tim3_state == DISABLE)
            {
                
                TIM_Cmd(TIM3, ENABLE);
                //tim3_state = ENABLE;            
            }
        }
        
        
        OSSemPost(PWM_TEST_SEM); 
     }
     
    
}


