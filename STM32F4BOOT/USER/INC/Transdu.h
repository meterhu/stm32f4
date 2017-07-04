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

#ifndef __TRANSDU_H
#define __TRANSDU_H

//=================================================================================================
//*************************************************************************************************
//宏定义

#define  ODD_V	     	  0x01
#define  EVEN_V           0x02
#define  NONE_V           0x00

#define  RTU_WR_REG       0x06
#define  RTU_RD_REG       0x03
#define  RTU_SLAVE_ADDR   0x01

#define  FRE2DATA(fre)     do{(fre) *= 10; (fre) /= 3;}while(0)



#define  REV_STOP  0x1
#define  REV_RUN   0x2
#define  FWD_RUN   0x4

#define  FWD_STOP  0x23
//#define  REV_STOP  0x24

#define  Wait     Delay

#define  STAND_RTU_MOD     1    //标准RTU通讯模式
#define  NO_STAND_MOD      0


//=========================================================================================
//VFD命令字
#define  VFD_FRE_CMD_ADDR      0x2000
#define  VFD_FRE_FACTOR        33             //为了防止浮点数;注意实际值是3.3的比例，最终值要再除以10

#define  VFD_RPM_CMD_ADDR      0x1000         
#define  VFD_REV_CMD           0x0001
#define  VFD_FWD_CMD           0x0001 

#define  VFD_STOP_CMD_ADDR     0x1000        //多数和VFD_RPM_CMD_ADDR一致
#define  VFD_REV_STOP_CMD      0x0005        //和下面的多数一致
#define  VFD_FWD_STOP_CMD      0x0005

#define  VFD_DELAY_CNT         (7 * 60000)


#define   INIT_TRANS_COM_CHNNL()	 InitUART(USART1,9600);
#define   TRANS_COM_SEND_BYTE(byte)  UART_SendByte(USART1, byte)	

#define  US_P_MIN        60 * 1000 * 1000

//=================================================================================================
//*************************************************************************************************
//数据类型定义

typedef struct _TransduComParamSet   //变频器通讯参数设定
{
    UINT16   odd;
    UINT32   br;
   
}TRANSDU_COM_PARAM_SET;


typedef struct _TransduCmd  //变频器命令字
{
    
    char    name[20];  //wide char，显示名称----debug
    UINT16  reg_addr;
    UINT16  fre_cmd_addr;
    UINT16  fre_factor;

    UINT16  rpm_cmd_addr;  //
    UINT16  rev_cmd;
    UINT16  fwd_cmd;

    UINT16  stop_cmd_addr;
    UINT16  rev_stop_cmd;
    UINT16  fwd_stop_cmd;

    UINT32  delay_cnt;

}TRANSDU_CMD;


typedef struct _TransduSet  //变频器命令字
{
     UINT8                   protocol_mod;
     TRANSDU_CMD             transdu_cmd;
     TRANSDU_COM_PARAM_SET   transdu_cmd_param;
                
     void   (*p_init)(UINT32 param);   //LG变频器需要初始化
     void   (*p_run)(UINT16, UINT16);
     void   (*p_stop)(UINT16); 
     UINT32  init_param;

}TRANSDU_SET;

//=================================================================================================
//*************************************************************************************************
//变量定义

//========================================================================================
//


//=================================================================================================
//*************************************************************************************************
//外部函数声明
extern UINT16 CRCCheck(UINT8 *data, UINT8 len);
extern void InitTransdu(void);
extern void TransduStop(UINT16 rev_fwd);
extern void TransduRun(UINT16 rev_fwd, UINT16 rpm);
extern void TransduRunSimu(UINT16 rev_fwd, UINT16 rpm);

extern void VFDRun(UINT16 rev_fwd, UINT16 rev_num);
extern void VFDStop(UINT16 rev_fwd);

extern void TransduCtrl(UINT16 rev_fwd, UINT16 rev_num);
extern void TransduCtrlSimu(UINT16 rev_fwd, UINT16 rpm);


#endif    //__TRANSDU_H

