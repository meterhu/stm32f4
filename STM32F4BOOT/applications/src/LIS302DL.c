
/**
date: 2012-03-21
author: wzh
e-mail: wangzhihai_138@163.com

note: accelerometer MEMS sensor--LIS302DL. source file

<for STM32F4 discovery board.> */

/* includes----------------------------------------------*/
#include "lis302dl.h"

/* Private function prototypes --------------------------*/
static uint8_t Lis302dl_SPI_ReadWrite(uint8_t dat);

/* Private functions ------------------------------------*/
/* Read&Write LIS302DL register */
static uint8_t Lis302dl_SPI_ReadWrite(uint8_t dat)
{
	while((SPI1->SR & 0x02) != 0x02); //TXE=1,then write. 
	SPI1->DR = dat;
	
	while((SPI1->SR & 0x01) != 0x01); //RXNE=1,then read.
    return SPI1->DR;  
}

/* Read lis302dl */
void Lis302dl_Read(uint8_t addr, uint8_t *dat, uint8_t num)
{	
	if(num>1)
	{
		addr |= 0xc0;
	}
	else
	{
		addr |= 0x80;
	}
	
	LIS302DL_CSL;
	Lis302dl_SPI_ReadWrite(addr); 
	while(num>0)
	{
		*dat = Lis302dl_SPI_ReadWrite(0x00);
		dat++;
		num--;
	}
	LIS302DL_CSH;	
}

/* Write lis302dl */
void Lis302dl_Write(uint8_t addr, uint8_t dat)
{
	LIS302DL_CSL;
	Lis302dl_SPI_ReadWrite(addr);
	Lis302dl_SPI_ReadWrite(dat);
	LIS302DL_CSH;	
}

/* config LIS302DL MEMS sensor */
void Lis302dl_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	SPI_InitTypeDef SPI_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SPI1, ENABLE); //open spi1 clock.
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA | RCC_AHB1Periph_GPIOE, ENABLE); //open PA&PE IO clock.
	
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource5, GPIO_AF_SPI1); //alternate function,
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource6, GPIO_AF_SPI1); //AF5(spi1) mapping to
	GPIO_PinAFConfig(GPIOA, GPIO_PinSource7, GPIO_AF_SPI1); //PA5,6,7 pin.
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5 | GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd =  GPIO_PuPd_DOWN;
	GPIO_Init(GPIOA, &GPIO_InitStructure); //config SPI1's GPIO.
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_Init(GPIOE, &GPIO_InitStructure); //spi1 chip select.
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOE, &GPIO_InitStructure); //lis302dl interrupt pin1&2.
	
	SPI_I2S_DeInit(SPI1); //reset spi1.
	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_Low;
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_1Edge;
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
	SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_4;
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
	SPI_InitStructure.SPI_CRCPolynomial = 7;
	SPI_Init(SPI1, &SPI_InitStructure); //config spi1.
	
	SPI_Cmd(SPI1, ENABLE); //start spi1.

	Lis302dl_Write(LIS302DL_CTRL_REG1_RW, 0x47); //power-up lis302dl.	
}

/* Read X-Y-Z axis data */
uint32_t Lis302dl_ReadXYZ(void)
{
	uint8_t stat, dat0, dat1, dat2, dat3;
	uint32_t temp;
	
	Lis302dl_Read(LIS302DL_STATUS_REG_R, &stat, 1); //read lis302dl's status.
	//X-axis.
	if((stat & 0x01) == 0x01)
	{
		Lis302dl_Read(LIS302DL_OUTX_R, &dat0, 1); //read lis302dl's X-axis.
	}
	//Y-axis.
	if((stat & 0x02) == 0x02)
	{
		Lis302dl_Read(LIS302DL_OUTY_R, &dat1, 1); //read lis302dl's Y-axis.
	}
	//Z-axis.
	if((stat & 0x04) == 0x04)
	{
		Lis302dl_Read(LIS302DL_OUTZ_R, &dat2, 1); //read lis302dl's Z-axis.
	}
	
	//who am i?
	Lis302dl_Read(LIS302DL_WHO_AM_I_R, &dat3, 1); //read lis302dl's identification register.
	
	temp = (uint32_t)((dat3<<24)|(dat2<<16)|(dat1<<8)|dat0); 
	return temp; //Z-Y-X.
}
	
/* click event--single&double */
void Lis302dl_ClickConfig(void)
{	
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;	
	
	Lis302dl_Write(LIS302DL_CTRL_REG3_RW, 0x07); //click mapping to INT1.
	
	Lis302dl_Write(LIS302DL_CLICK_CFG_RW, 0x50); //Z-axis single click.
	
	Lis302dl_Write(LIS302DL_CLICK_THSZ_RW, 0x0a); //Z-axis threshold is 10*0.5g.

	Lis302dl_Write(LIS302DL_CLICK_TIMELIMIT_RW, 0x03); //Z-axis time_limit is 3*0.5ms.
		
	Lis302dl_Write(LIS302DL_CLICK_LATENCY_RW, 0x32); //Z-axis latency time is 50*1ms.
	
	Lis302dl_Write(LIS302DL_CLICK_WINDOW_RW, 0x64); //Z-axis window time is 200*1ms.

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE); //open syscfg clock.
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE, EXTI_PinSource0); //PE0--EXTI0.	
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure); //PE0 rising interrupt. 
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x04;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}




