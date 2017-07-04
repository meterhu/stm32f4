//===============================================================================================
/************************************************************************************************
*                                                                                                     
*            文件名称:              
*
*                摘要: 
*
*            创建时间: 2009-07-02  
*
*            修改时间: 
*            修改原因:
*
*            
*
*                作者: 杨卫华
*
*************************************************************************************************/
//===============================================================================================
#ifndef _ADS123X_H
#define _ADS123X_H

//=================================================================================================
//*************************************************************************************************

#define  ADS123X_CHNNL1        0x0
#define  ADS123X_CHNNL2        0x1
#define  ADS123X_CHNNL3        0x2
#define  ADS123X_CHNNL4        0x3

//--------------------------------------------------------------------------


#define  ADS123X_CLK_H()       GPIO_SetBits(GPIOB, GPIO_Pin_15) 
#define  ADS123X_CLK_L()       GPIO_ResetBits(GPIOB, GPIO_Pin_15)

#define  ADS123X_A0_H()        GPIO_SetBits(GPIOB, GPIO_Pin_14) 
#define  ADS123X_A0_L()        GPIO_ResetBits(GPIOB, GPIO_Pin_14)

#define  ADS123X_A1_H()        GPIO_SetBits(GPIOB, GPIO_Pin_11) 
#define  ADS123X_A1_L()        GPIO_ResetBits(GPIOB, GPIO_Pin_11)

#define  ADS123X_RST_H()       GPIO_SetBits(GPIOB, GPIO_Pin_12) 
#define  ADS123X_RST_L()       GPIO_ResetBits(GPIOB, GPIO_Pin_12)

#define  ADS123X_DO()          GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13) 




//=================================================================================================
//*************************************************************************************************
//void  InitADS784x(void);
extern void InitADS123x(void);
extern s32 ReadADS123x_Pri(u8 chnnl, u8 calibr);
extern s32 ReadADS123x_Pri1(u8 chnnl, u8 calibr);
extern s32 ReadADS123x(u8 chnnl);

#endif //_ADS123X_H
