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

#ifndef __INOUT_H
#define __INOUT_H

//=================================================================================================
//*************************************************************************************************
//宏定义
//BN=bit number
#define OUT_COOL_DW_BN             0     //P1.0
#define OUT_COOL_DW_FAN_BN         1
#define OUT_WARM_UP_BN             2
#define OUT_H_WARM_UP_BN           3     //强加热


#define OUT_OIL_PUMP_BN            5
#define IN_SAFE_GAME_BN            6


//#define OUT_COOL_DW()               BIT(OUT_COOL_DW_BN)     //P1.0
//#define OUT_COOL_DW_FAN()           BIT(OUT_COOL_DW_FAN_BN)
//#define OUT_WARM_UP()               BIT(OUT_WARM_UP_BN)
//#define OUT_H_WARM_UP()             BIT(OUT_H_WARM_UP_BN)     //强加热
//#define OUT_OIL_PUMP()              BIT(OUT_OIL_PUMP_BN)
//#define IN_SAFE_GAME()              BIT(IN_SAFE_GAME_BN)

#define OUT_COOL_DW()                 OutH(OUT_COOL_DW_BN)     //P1.0
#define OUT_COOL_DW_FAN()             OutH(OUT_COOL_DW_FAN_BN)
#define OUT_WARM_UP()                 OutH(OUT_WARM_UP_BN)
#define OUT_H_WARM_UP()               OutH(OUT_H_WARM_UP_BN)     //强加热

#define OUT_COOL_DW_L()               OutL(OUT_COOL_DW_BN)     //P1.0
#define OUT_COOL_DW_FAN_L()           OutL(OUT_COOL_DW_FAN_BN)
#define OUT_WARM_UP_L()               OutL(OUT_WARM_UP_BN)
#define OUT_H_WARM_UP_L()             OutL(OUT_H_WARM_UP_BN)     //强加热


#define OUT_YW()                      OutH(4)     //P1.0
#define OUT_GY()                      OutH(5)
#define OUT_YB()                      OutH(6)
#define OUT_AQ()                      OutH(7)     //强加热

#define OUT_YW_L()                    OutL(4)     //P1.0
#define OUT_GY_L()                    OutL(5)
#define OUT_YB_L()                    OutL(6)
#define OUT_AQ_L()                    OutL(7)     //强加热


#define OUT_OIL_PUMP()                OutH(OUT_OIL_PUMP_BN)
#define IN_SAFE_GAME()                OutH(IN_SAFE_GAME_BN)


#define  IS_IO_CHG_BIT(bit_n)         (AppData.io.io32_state ^ (0x1 << (bit_n)))
#define  SET_IO_BIT(bit_n)            do{AppData.io.io32_state &= ~(0x1 << (bit_n));AppData.io.io32_state |= ~(0x1 << (bit_n));}while(0)

//=================================================================================================
//*************************************************************************************************
//数据类型定义



//=================================================================================================
//*************************************************************************************************
//变量定义

//========================================================================================
//


//=================================================================================================
//*************************************************************************************************
//外部函数声明
extern void  InitInputIO(void);
extern void  IN(uint8_t in_bit_num, uint8_t new_state);
extern void  Out(uint8_t out6_bit_num);   
extern void  OutH(uint8_t out6_bit_num); 
extern void  OutL(uint8_t out6_bit_num); 


#endif    //__INOUT_H


