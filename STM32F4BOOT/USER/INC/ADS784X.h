//===============================================================================================
/************************************************************************************************
*                                                                                                     
*            文件名称:              
*
*                摘要: 
*                   引脚名称	对应IO管脚	  开入/开出
*                   SPI1_NSS	    PA4	        开出
*                   SPI1_SCK	    PA5	        开出
*                   SPI1_MISO	    PA6	        开入
*                   SPI1_MOSI	    PA7 	    开出
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

//#include "stm32f10x_lib.h"
//#include "Share.h"


#ifndef _ADS784X_H
#define _ADS784X_H

//=================================================================================================
//*************************************************************************************************
#define   AD_IO_PORT    GPIOA
#define   AD_DO_LO()    GPIO_ResetBits(AD_IO_PORT, GPIO_Pin_12)
#define   AD_DO_HI()    GPIO_SetBits(AD_IO_PORT, GPIO_Pin_12)  

#define   AD_CS_LO()    GPIO_ResetBits(AD_IO_PORT, GPIO_Pin_11)
#define   AD_CS_HI()    GPIO_SetBits(AD_IO_PORT, GPIO_Pin_11)

#define   AD_CLK_LO()   GPIO_ResetBits(AD_IO_PORT, GPIO_Pin_10)
#define   AD_CLK_HI()   GPIO_SetBits(AD_IO_PORT, GPIO_Pin_10)


#define   AD_DI()       GPIO_ReadInputDataBit(AD_IO_PORT, GPIO_Pin_9)                // (IO0PIN & BIT19)读取状态位

#define   SPI_DLY()     Delay(400)                     //Wait(400)              //试验证明该延时越长采样值越稳定


//=================================================================================================
//*************************************************************************************************
//void  InitADS784x(void);
void  InitADS784xPin(void);

u16   ADS784xAD(u8  cmd);


#endif //_ADS784X_H
