#ifndef  __ADS1234_H
#define  __ADS1234_H

//=================================================================================================
//*************************************************************************************************
//宏定义

//--------------------------------------------------------------------------------------------------
#define   AD1_A0_LO()     GPIO_ResetBits(GPIOB, GPIO_Pin_14)
#define   AD1_A0_HI()     GPIO_SetBits(GPIOB, GPIO_Pin_14)

#define   AD1_A1_LO()     GPIO_ResetBits(GPIOB, GPIO_Pin_11)
#define   AD1_A1_HI()     GPIO_SetBits(GPIOB, GPIO_Pin_11)

#define   AD1_RST_LO()    GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define   AD1_RST_HI()    GPIO_SetBits(GPIOB, GPIO_Pin_12)

#define   AD1_CLK_LO()    GPIO_ResetBits(GPIOB, GPIO_Pin_15)
#define   AD1_CLK_HI()    GPIO_SetBits(GPIOB, GPIO_Pin_15) 

#define   AD1_DO()        GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_13)

//--------------------------------------------------------------------------------------------------
#define   SCLK_DLY()	  DelayUS(250)

//--------------------------------------------------------------------------------------------------
#define BIT0        0x0001
#define BIT1        0x0002
#define BIT2        0x0004
#define BIT3        0x0008
#define BIT4        0x0010
#define BIT5        0x0020
#define BIT6        0x0040
#define BIT7        0x0080
#define BIT8        0x0100
#define BIT9        0x0200
#define BIT10       0x0400
#define BIT11       0x0800
//--------------------------------------------------------------------------------------------------
#define CYLI_LEN    30
#define BIT_LEN     12
//--------------------------------------------------------------------------------------------------
#define PIW_LEN     30
#define SUMPIW_LEN  26

//=================================================================================================
//*************************************************************************************************
//外部函数
void InitADS1234Pin(void);
void InitADS1234(void);

u32  ADS1234AD1(u8 num);
u32 ADS1234Sampl1(u8 num);

void CRC_ADS1234(void);

extern u16 ADS1234Sampl_AD(u8 n);

extern void  CyliShel(u16 cmd);

#endif





