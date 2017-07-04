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
#include "includes.h"
#include <stdio.h>
#include <stdarg.h>
#include "stm32f4xx.h"
#include "stm32f4xx_fsmc.h"
#include "mystd.h"
#include "m_config.h"
#include "InitHW.h"
#include "UART.h"
#include "stm324xg_eval_i2c_ee.h"



#define sEE_WRITE_ADDRESS1        0x00
#define sEE_READ_ADDRESS1         0x00
#define BUFFER_SIZE1             (countof(Tx1_Buffer)-1)
#define BUFFER_SIZE2             (countof(Tx2_Buffer)-1)
#define sEE_WRITE_ADDRESS2       (sEE_WRITE_ADDRESS1 + BUFFER_SIZE1)
#define sEE_READ_ADDRESS2        (sEE_READ_ADDRESS1 + BUFFER_SIZE1)

/* Private macro -------------------------------------------------------------*/
#define countof(a) (sizeof(a) / sizeof(*(a)))

/* Private variables ---------------------------------------------------------*/


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
void  TestEEPROM(void)
{
    uint8_t Tx1_Buffer[] = "SZW-STM32F40xx I2C buffer 1 transfer";
    uint8_t Rx1_Buffer[50] = "";
    volatile uint16_t NumDataRead = 0;
    
    sEE_Init();

    sEE_WriteBuffer(Tx1_Buffer, sEE_WRITE_ADDRESS1, BUFFER_SIZE1); 
    
    /* Wait for EEPROM standby state */
    sEE_WaitEepromStandbyState(); 
    NumDataRead = BUFFER_SIZE1;
    sEE_ReadBuffer(Rx1_Buffer, sEE_READ_ADDRESS1, (uint16_t *)(&NumDataRead)); 
    
    
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
uint32_t sEE_TIMEOUT_UserCallback(void)
{
    
  while (1)
  { 
      ;      
  } 
  
}

