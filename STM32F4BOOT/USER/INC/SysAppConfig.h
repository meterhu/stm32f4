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

#ifndef __SYS_APP_CFG_H
#define __SYS_APP_CFG_H

//=================================================================================================
//*************************************************************************************************
//宏定义

#define  RUN_STATE            0x01
#define  STOP_STATE           0x02



#define  AD_CHNNL0_VALID_CFG    0x01    //0:未使用;1:使用
#define  AD_CHNNL1_VALID_CFG    0x01
#define  AD_CHNNL2_VALID_CFG    0x01
#define  AD_CHNNL3_VALID_CFG    0x01

#define  AD_CHNNL_NUMS           4

//========================================================================================
#define  MAX_MEAS_REV        (200000) //最大测量转速/分钟
#define  MEAS_START_REV      0        //测量起始转速
#define  MEAS_REV_STEP       250      //转速测量步进量
#define  REV_MEAS_FRE        ((60 * 1000 * 1000 * 1000) / MAX_MEAS_REV)          //转速测试频率

//========================================================================================
#define   TMPTR_USING        1
#define   AD_USING           1


//=================================================================================================
//*************************************************************************************************
//数据类型定义
typedef struct __SysAppCfg  
{
    uint8_t ad_chnnl_nums;      //ad通道数
    uint8_t ad_chnnl_use[4];    //ad使用的通道
    uint8_t tmptr_bits;         //温度采样位数:9-12,详见DS18B20驱动
    uint8_t out_bits;           //开出通道数
    

}SYS_APP_CFG;


typedef  struct _AppGData  
{
    //温度
    volatile int16_t   cur_tmptr;
    volatile int16_t   set_tmptr;
    volatile uint16_t  tmptr_run_state;
    
    //转速
    volatile uint32_t  cur_rev;
    volatile uint32_t  set_rev;
    volatile uint16_t  rev_run_state;
    
    //电位器
    volatile uint16_t  digit_rp_en;
    volatile uint16_t  digit_rp_cnt;
    
    //角度
    volatile uint16_t  set_angle_v;
    volatile uint16_t  angle_run_state;
    
    //pid
    volatile uint16_t  pid_en;  
      
    //uint16_t  oil_cnt;
    //uint16_t  oil_cnt_state;

   
}APP_DATA;


typedef __packed struct _SAVE_PARAM       
{
     uint16_t  CylNetWeight[12];
     uint16_t  OilFactor[12];
     uint8_t   AdChnnl;
     uint16_t  OilWeight[12];
     uint16_t  CylNetNum;
	

}SAVE_PARAM;

typedef __packed struct _SAVE_PARAM_One       
{
     uint32_t  CylNetWeight;
     uint16_t  OilFactor;
     uint8_t   AdChnnl;
     uint32_t  OilWeight;
     uint16_t  CylNetNum;
	
}SAVE_PARAM_ONE;

typedef __packed struct _MCU_INFO       
{
     //CPU_INFO  CPUInfo;
     uint32_t  id;
     uint32_t  flash_size;
     uint16_t  sector_num;
     uint32_t  flash_start_addr;
	
}MCU_INFO;


extern APP_DATA  AppData;

#define  UpdateCurRev(v)               AppData.cur_rev =  (v)
#define  UpdateSetRev(v)               AppData.set_rev =  (v)
#define  GetCurRev()                   AppData.cur_rev
#define  GetSetRev()                   AppData.set_rev
#define  RevRun(state)                 AppData.rev_run_state =  (state)
#define  RevRunState()                 AppData.rev_run_state


#define  UpdateCurTmptr(v)             AppData.cur_tmptr =  (v)
#define  UpdateSetTmptr(v)             AppData.set_tmptr =  (v)
#define  GetCurTmptr()                 AppData.cur_tmptr
#define  GetSetTmptr()                 AppData.set_tmptr
#define  TmptrCtrlRun(state)           AppData.tmptr_run_state =  (state)
#define  TmptrRunState()               AppData.tmptr_run_state


#define  UpdateAngle(v)                AppData.set_angle_v =  (v);
#define  GetAngle()                    AppData.set_angle_v
#define  AngleRun(state)               AppData.angle_run_state =  (state)
#define  AngleRunState()               AppData.angle_run_state

#define  UpdateDRP(v)                  AppData.digit_rp_cnt =  (v);
#define  GetDRP()                      AppData.digit_rp_cnt
#define  EnableDRP(state)              AppData.digit_rp_en = state
#define  DRPEnableState()              AppData.digit_rp_en

#define  EnablePID(state)              AppData.pid_en = state
#define  PIDPEnableState()             AppData.pid_en


typedef  __packed struct _REV_MEAS_ANGLE_V  
{
    int32_t   rev_v;
    int16_t   meas_v;
    int16_t   angle_v;
   

   
}REV_MEAS_ANGLE_V;

//=================================================================================================
//*************************************************************************************************
//变量定义

//========================================================================================
//


//=================================================================================================
//*************************************************************************************************
//外部函数声明

//extern    
extern SYS_APP_CFG  SysAppCfg;




#endif    //__SYS_APP_CFG_H
