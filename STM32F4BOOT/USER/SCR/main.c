//======================================================================================================
/******************************************************************************************************
 *
 *            文件名称:
 *
 *                摘要:
 *
 *            创建时间: 2012-05-xx
 *
 *            修改时间: 2012-05-xx
 *            修改原因:
 *
 *
 *
 *                作者:
 *
 *******************************************************************************************************/
//======================================================================================================
#include <stdio.h>
#include <stdarg.h>
//#include "absacc.h"
#include "stm32f4xx.h"
//#include "ucos_ii.h"
#include "mystd.h"
#include "SysAppConfig.h"
#include "main.h"
#include "m_config.h"
#include "InitHW.h"
#include "UserLib.h"
#include "UART.h"
#include "CommHandle.h"
//#include "LCD_RA8875.h"
#include "rtc.h"
#include "ExINT.h"
#include "Timer.h"
#include "CPUID.h"
#include "SimuI2C.h"
#include "AppCRC.h"
#include "FlashOpt.h"



//const CPU_INFO  thisCPUInfo = {0x28002e,0x33314706,0x32343034,0};
const CPU_INFO  thisCPUInfo = {0x28002e,0x33314706,0x32343034,0};
extern int   CheckID(void);

#define  FLASH_START_ADDR1   0x08008000
#define  FLASH_START_ADDR    0x0800C000

extern uint8_t CheckCipherValidID(void);
extern uint32_t FirmwareUpdateCnt;
uint8_t  FirmwareUpdateFlag = 0;

extern void FlashWRDPUnlock(void);
extern void FlashWRP(void);

uint8_t Key4[4]= {0x90,0xAF,0xE2,0x34};

#define  APP_DW_COMM_TM_OUT   1000 * 1000 * 3 //4s

//======================================================================================================
/*******************************************************************************************************
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
 *******************************************************************************************************/
int main(void)
{
    __IO  uint32_t tm_out_cnt;


    SystemInit();
    //FlashWRP();
    //FlashWRDPUnlock();

    InitApp();
    InitBoardDev();
    printf("----boot up-----\n");

    TIM_SetCounter(TIM5, 0x0);

    while (TRUE_P) {
        FrameParseExec();

        if (!FirmwareUpdateFlag) {
            //未来数据帧命令，而且超时四秒，执行校验程序
            tm_out_cnt =  TIM_GetCounter(TIM5);

            if (tm_out_cnt >= APP_DW_COMM_TM_OUT) {
                //如果校验错误，跳出循环，执行解析帧
                if (!AppCRC()) {
                    break;

                }

            }

        }


    }

    while (TRUE_P) {
        FrameParseExec();
    }



}


//======================================================================================================
/*******************************************************************************************************
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
 *******************************************************************************************************/

void  InitBoardDev(void)
{

    GPIO_Config();

    //LED
    GPIO_ResetBits(GPIOD, GPIO_Pin_3);

    InitUARTQBuff();
    USART_DeInit(USART1);
    InitUART(USART1,115200);
    //printf("USART1 test\n");

    USART_DeInit(USART2);
    InitUART(USART2,115200);
    //printf("program is running\n");

    ///InitTimer2();

    ///Delay(1000);

    ///Delay(1000);
    InitTimer5();

    //InitRTC();
    //RTC_AlarmConfig();

    ///InitSimuI2C();

    //InitTimer3();
    //InitTimer4();
    //InitTimer7();

    //CheckID(); //检查ID，检查返回值，并作相应处理



}


//======================================================================================================
/*******************************************************************************************************
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
 *******************************************************************************************************/
void InitApp(void)
{

    InitComm();

}

//======================================================================================================
/*******************************************************************************************************
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
 *******************************************************************************************************/
int  CheckID(void)
{
    CPU_INFO  CPUInfo;

    ReadCPUID(&CPUInfo);
    printf("CPU ID is %x,%x,%x,%x\n", CPUInfo.id[0],CPUInfo.id[1],CPUInfo.id[2],CPUInfo.flash_info);

    if (!MemoryCmp(&CPUInfo, &thisCPUInfo,sizeof(CPU_INFO) - 4)) {
        return 0;
    }

    return 1;

}


//======================================================================================================
/*******************************************************************************************************
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
 *******************************************************************************************************/
void SysRunLED(void)
{



}


//======================================================================================================
/*******************************************************************************************************
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
 *******************************************************************************************************/
void LCD_FSMCConfig(void)
{
    FSMC_NORSRAMInitTypeDef  FSMC_NORSRAMInitStructure;
    FSMC_NORSRAMTimingInitTypeDef  p;

    GPIO_InitTypeDef GPIO_InitStructure;

    RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD | RCC_AHB1Periph_GPIOG | RCC_AHB1Periph_GPIOE |
                           RCC_AHB1Periph_GPIOF, ENABLE);

    /* Enable FSMC clock */
    RCC_AHB3PeriphClockCmd(RCC_AHB3Periph_FSMC, ENABLE);


    /*-- GPIO Configuration ------------------------------------------------------*/
    /* SRAM Data lines configuration */
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1 | GPIO_Pin_8 | GPIO_Pin_9 |GPIO_Pin_4 |GPIO_Pin_5|
                                  GPIO_Pin_10 |GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
    GPIO_Init(GPIOD, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOD, GPIO_PinSource0,  GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource1,  GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource4,  GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource5,  GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource8,  GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource9,  GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource10, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource11, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource12, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource14, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOD, GPIO_PinSource15, GPIO_AF_FSMC);


    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7  | GPIO_Pin_8  | GPIO_Pin_9  | GPIO_Pin_10 |
                                  GPIO_Pin_11 | GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 |
                                  GPIO_Pin_15;
    GPIO_Init(GPIOE, &GPIO_InitStructure);

    GPIO_PinAFConfig(GPIOE, GPIO_PinSource7, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource8, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource9, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource10, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource11, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource12, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource13, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_FSMC);
    GPIO_PinAFConfig(GPIOE, GPIO_PinSource15, GPIO_AF_FSMC);

    /*-- FSMC Configuration ------------------------------------------------------*/

#ifdef VECT_TAB_SRAM
    p.FSMC_AddressSetupTime = 2;//0
    p.FSMC_DataSetupTime = 3;   //2
#else
    p.FSMC_AddressSetupTime = 3;//0
    p.FSMC_DataSetupTime = 5;   //2
#endif
    p.FSMC_AddressHoldTime = 0; //0
    p.FSMC_BusTurnAroundDuration = 1; //0
    p.FSMC_CLKDivision = 0;//0
    p.FSMC_DataLatency = 0;//0
    ///p.FSMC_AccessMode = FSMC_AccessMode_A;
    p.FSMC_AccessMode = FSMC_AccessMode_B;

    ///FSMC配置：ADDSET = 2; DATSET= 3; BUSTURN=1;


    //FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM3;
    FSMC_NORSRAMInitStructure.FSMC_Bank = FSMC_Bank1_NORSRAM1;
    //FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Enable;
    FSMC_NORSRAMInitStructure.FSMC_DataAddressMux = FSMC_DataAddressMux_Disable;
    FSMC_NORSRAMInitStructure.FSMC_MemoryType = FSMC_MemoryType_SRAM;
    FSMC_NORSRAMInitStructure.FSMC_MemoryDataWidth = FSMC_MemoryDataWidth_16b;
    FSMC_NORSRAMInitStructure.FSMC_BurstAccessMode = FSMC_BurstAccessMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalPolarity = FSMC_WaitSignalPolarity_Low;
    FSMC_NORSRAMInitStructure.FSMC_WrapMode = FSMC_WrapMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignalActive = FSMC_WaitSignalActive_BeforeWaitState;
    FSMC_NORSRAMInitStructure.FSMC_WriteOperation = FSMC_WriteOperation_Enable;
    FSMC_NORSRAMInitStructure.FSMC_WaitSignal = FSMC_WaitSignal_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ExtendedMode = FSMC_ExtendedMode_Disable;
    FSMC_NORSRAMInitStructure.FSMC_WriteBurst = FSMC_WriteBurst_Disable;
    FSMC_NORSRAMInitStructure.FSMC_ReadWriteTimingStruct = &p;
    FSMC_NORSRAMInitStructure.FSMC_WriteTimingStruct = &p;

    FSMC_NORSRAMInit(&FSMC_NORSRAMInitStructure);

    /* Enable FSMC Bank1_SRAM Bank */
    //FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM3, ENABLE);
    FSMC_NORSRAMCmd(FSMC_Bank1_NORSRAM1, ENABLE);

}


