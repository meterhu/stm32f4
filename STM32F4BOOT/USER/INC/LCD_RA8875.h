/*
*********************************************************************************************************
*
*	ģ������ : RA8875оƬ����ģ��������ģ��
*	�ļ����� : LCD_RA8875.h
*	��    �� : V1.0
*	˵    �� : ͷ�ļ�
*
*	Copyright (C), 2010-2011, ���������� www.armfly.com
*
*********************************************************************************************************
*/


#ifndef _LCD_RA8875_H
#define _LCD_RA8875_H

///#include "bsp_tft_lcd.h"

#define  LCD_WHITE   0x0000

/* ѡ��RA8875�Ľӿ�ģʽ, �����Ӳ��ƥ�� */
//#define IF_SPI_EN			/* SPI�ӿ� */
#define IF_8080_EN		/* 8080 ���߽ӿ� */


#ifdef IF_SPI_EN	/* 4-Wire SPI ���� (��Ҫ�ı�RA8875���ϵ�2������λ��) */
/*
��1��������STM32-X2������ + 4.0���7����ʾģ�飨��ģ��Ӳ������ΪSPI�ӿ�ģʽ��
	PA8   --> TP_CS
	PD3   --> TP_BUSY (δ��,���ò�ѯ״̬�Ĵ����ķ�ʽʵ��)
	PA5   --> TP_SCK
	PA6   --> TP_MISO
	PA7   --> TP_MOSI
	PC7   --> TP_PEN_INT (δ�ã����ò�ѯ�Ĵ����ķ�ʽ)
*/
	#define RA8875_CS_0()	GPIOA->BRR = GPIO_Pin_8
	#define RA8875_CS_1()	GPIOA->BSRR = GPIO_Pin_8

	#define SPI_WRITE_DATA	0x00
	#define SPI_READ_DATA	0x40
	#define SPI_WRITE_CMD	0x80
	#define SPI_READ_STATUS	0xC0
#else		/* 8080���� ��������RA8875��ȱʡģʽ�� */

	/*
		������ STM32-X2 ���İ���߷�����

		LCDģ��� RS ����     �� PD13/FSMC_A18
		LCDģ��� CS Ƭѡ���� �� PD7/FSMC_NE1
		������ NWE, NOE, D15~D0 �Ӷ�Ӧ��FSMC���߼��ɡ�
	*/
	#define RA8875_BASE		((uint32_t)(0x60000000 | 0x00000000))

	#define RA8875_REG		*(__IO uint16_t *)(RA8875_BASE +  (1 << 17))
	#define RA8875_RAM		*(__IO uint16_t *)(RA8875_BASE)
#endif


enum
{
	RA_FONT_16 = 0,		/* RA8875 ���� 16���� */
	RA_FONT_24 = 1,		/* RA8875 ���� 24���� */
	RA_FONT_32 = 2		/* RA8875 ���� 32���� */
};

/* ���ַŴ���� */
enum
{
	RA_SIZE_X1 = 0,		/* ԭʼ��С */
	RA_SIZE_X2 = 1,		/* �Ŵ�2�� */
	RA_SIZE_X3 = 2,		/* �Ŵ�3�� */
	RA_SIZE_X4 = 3		/* �Ŵ�4�� */
};


/* ����LCD��ʾ����ķֱ��� */
///#define LCD_30_HEIGHT	240		/* 3.0����� �߶ȣ���λ������ */
//#define LCD_30_WIDTH	400		/* 3.0����� ��ȣ���λ������ */

#define LCD_43_HEIGHT	272		/* 4.3����� �߶ȣ���λ������ */
#define LCD_43_WIDTH	480		/* 4.3����� ��ȣ���λ������ */

#define LCD_70_HEIGHT	480		/* 7.0����� �߶ȣ���λ������ */
#define LCD_70_WIDTH	800		/* 7.0����� ��ȣ���λ������ */

#define CHIP_STR_5420	"SPFD5420A"
#define CHIP_STR_4001	"OTM4001A"
#define CHIP_STR_61509	"R61509V"
#define CHIP_STR_8875	"RA8875"

/* ��������� */
#define BRIGHT_MAX		255
#define BRIGHT_MIN		0
#define BRIGHT_DEFAULT	200
#define BRIGHT_STEP		5

#define   BLUE0     0xEE56      //ǳ��ɫ
#define   BLACK    0x0000      	//��ɫ
#define   BLUE     0xF800      	//��ɫ
#define   RED      0x001F      	//��ɫ
#define   GRAY0    0xCE9A   	//��ɫ0 3165 00110 001011 00101
#define   GRAY1    0x8410      	//��ɫ1      00000 000000 00000
#define   GRAY2    0x4208      	//��ɫ2  1111111111011111
#define   WHITE    0xFFFF		//��ɫ   110010 110100 110100
#define   GREEN    0x07E0		//��ɫ   110010 110100 110100
#define   YELLOW   0xF800      	//��ɫ07FF



#define  RGB2(r,g,b) (((r)  << 16) + ((g)  << 8) + (b))
#define  RGB(r,g,b) (((b)  << 16) + ((g)  << 8) + (r))
#define  RGB1(r,g,b) (((r)  & 0x1F) << (6 + 5) + ((g) & 0x3F) << 6 + ((b) & 0x1F))

#define  LCD_COLOR2SYS(color)

enum
{
	IC_5420		= 0x5420,
	IC_4001		= 0x4001,
	IC_61509 	= 0xB509,
	IC_8875 	= 0x0075
};

/* �ɹ��ⲿģ����õĺ��� */
void RA8875_InitHard(void);
uint16_t RA8875_ReadID(void);
uint8_t RA8875_IsBusy(void);
void RA8875_Layer1Visable(void);
void RA8875_Layer2Visable(void);
void RA8875_DispOn(void);
void RA8875_DispOff(void);
void RA8875_PutPixel(uint16_t _usX, uint16_t _usY, uint16_t _usColor);
uint16_t RA8875_GetPixel(uint16_t _usX, uint16_t _usY);
void RA8875_SetFrontColor(uint16_t _usColor);
void RA8875_SetBackColor(uint16_t _usColor);
void RA8875_SetFont(uint8_t _ucFontType, uint8_t _ucLineSpace, uint8_t _ucCharSpace);
void RA8875_SetTextZoom(uint8_t _ucHSize, uint8_t _ucVSize);
void RA8875_DispAscii(uint16_t _usX, uint16_t _usY, char *_ptr);
void RA8875_DispStr(uint16_t _usX, uint16_t _usY, char *_ptr);
void RA8875_ClrScr(uint16_t _usColor);
void RA8875_DrawBMP(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t *_ptr);
void RA8875_DrawLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usY2 , uint16_t _usColor);
void RA8875_DrawRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor);
void RA8875_FillRect(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t _usColor);
void RA8875_DrawCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint16_t _usColor);
void RA8875_FillCircle(uint16_t _usX, uint16_t _usY, uint16_t _usRadius, uint16_t _usColor);
void RA8875_SetDispWin(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth);
void RA8875_QuitWinMode(void);
void RA8875_SetBackLight(uint8_t _bright);
void RA8875_SetCursor(uint16_t _usX, uint16_t _usY);
void LCD8875_DrawHLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usX2 , uint16_t _usColor);
void LCD8875_DrawVLine(uint16_t _usX1 , uint16_t _usY1 , uint16_t _usY2 , uint16_t _usColor);

void BTE_SetTarBlock(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint8_t _ucLayer);
void BTE_SetOperateCode(uint8_t _ucOperate);
void BTE_Wait(void);
void BTE_Start(void);

void RA8875_TouchInit(void);
uint16_t RA8875_TouchReadX(void);
uint16_t RA8875_TouchReadY(void);
void RA8875_WriteCmd(uint8_t _ucRegAddr);
void RA8875_SetCursor(uint16_t _usX, uint16_t _usY);
void RA8875_WriteData(uint8_t _ucRegValue);
uint8_t RA8875_ReadData(uint8_t _ucRegAddr);

void RA8875_InitSPI(void);
uint16_t RA8875_TouchReadXY(void);
void RA8875_DrawBMPLine(uint16_t _usX, uint16_t _usY, uint16_t _usHeight, uint16_t _usWidth, uint16_t *_ptr,const uint16_t *pTrans);

#endif


