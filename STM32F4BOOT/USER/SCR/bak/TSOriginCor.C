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
 *
 *******************************************************************************************************/
//======================================================================================================
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "mystd.h"
#include  "TSCalibrate.h"
#include "stm324xg_eval_i2c_ee.h"
#include "TSOriginCor.h"
#include <absacc.h>
#include "SimuI2C.h"



///uint16_t  EEPROM_DMA_Buff[9]  __attribute__ ((at(0x20001000)));



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
void  SaveTSCalibrateValue11(TS_COR *pt)
{
    uint16_t  data_len;
    uint16_t  EEPROM_DMA_Buff[9];
    
    data_len = 10;
  
    ////sEE_ReadBuffer((uint8_t *)EEPROM_DMA_Buff, 0, (uint16_t *)(&EEPROM_DMA_Buff[8])); 
        
    EEPROM_DMA_Buff[0] = pt->x0;
    EEPROM_DMA_Buff[1] = pt->x1;
    EEPROM_DMA_Buff[2] = pt->y0;
    EEPROM_DMA_Buff[3] = pt->y1;
    EEPROM_DMA_Buff[4] = 0xA5A5;  //有效标识
    
    sEE_WriteBuffer((uint8_t *)EEPROM_DMA_Buff, 0, data_len); 
    
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
int  ReadTSCalibrateValue11(uint16_t *pbuff)
{
    uint16_t  EEPROM_DMA_Buff[9];
    uint16_t  data_len = 10;
    
    
    sEE_ReadBuffer((uint8_t *)EEPROM_DMA_Buff, 0, &data_len); 
    
    pbuff[0] = EEPROM_DMA_Buff[0];
    pbuff[1] = EEPROM_DMA_Buff[1];
    pbuff[2] = EEPROM_DMA_Buff[2];
    pbuff[3] = EEPROM_DMA_Buff[3];
    
    if (EEPROM_DMA_Buff[4] != 0xA5A5)
    {

        return 0;
        
    }
    
    if ((EEPROM_DMA_Buff[0] < 1) || (EEPROM_DMA_Buff[0] > 200)) //x0
    {
        return 0;   
        
    }
    
    if ((EEPROM_DMA_Buff[1] < 500) || (EEPROM_DMA_Buff[1] > 1024)) //x1
    {
        return 0;   
        
    }
    
    if ((EEPROM_DMA_Buff[2] < 1) || (EEPROM_DMA_Buff[2] > 200)) //y0
    {
        return 0;   
        
    }
    
    if ((EEPROM_DMA_Buff[3] < 500) || (EEPROM_DMA_Buff[3] > 1024)) //y1
    {
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
void  SaveTSCalibrateValue(TS_COR *pt)
{
 
   //sEE_ReadBuffer((uint8_t *)EEPROM_DMA_Buff, 0, (uint16_t *)(&EEPROM_DMA_Buff[8])); 
 
    
    RTC_WriteBackupRegister(RTC_BKP_DR4, pt->x0);
    RTC_WriteBackupRegister(RTC_BKP_DR5, pt->x1);
    RTC_WriteBackupRegister(RTC_BKP_DR6, pt->y0);
    RTC_WriteBackupRegister(RTC_BKP_DR7, pt->y1);
    RTC_WriteBackupRegister(RTC_BKP_DR8, 0xA5A5);
    
    InitSimuI2C();
    ////I2C_Simu_BuffWrite(wbuff, 0, 5);
    //I2C_Simu_BuffRead(rbuff,0,5);

   
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
int  ReadTSCalibrateValue(uint16_t *buff)
{
      
    //sEE_ReadBuffer((uint8_t *)EEPROM_DMA_Buff, 0, (uint16_t *)(&EEPROM_DMA_Buff[8])); 
    buff[0] = RTC_ReadBackupRegister(RTC_BKP_DR4);
    buff[1] = RTC_ReadBackupRegister(RTC_BKP_DR5);
    buff[2] = RTC_ReadBackupRegister(RTC_BKP_DR6);
    buff[3] = RTC_ReadBackupRegister(RTC_BKP_DR7);
    buff[4] = RTC_ReadBackupRegister(RTC_BKP_DR8);
    
    if (buff[4] != 0xA5A5)
    {
        
        return 0;
        
    }
    
    if ((buff[0] < 1) || (buff[0] > 200)) //x0
    {
        return 0;   
        
    }
    
    if ((buff[1] < 500) || (buff[1] > 1024)) //x1
    {
        return 0;   
        
    }
    
    if ((buff[2] < 1) || (buff[2] > 200)) //y0
    {
        return 0;   
        
    }
    
    if ((buff[3] < 500) || (buff[3] > 1024)) //y1
    {
        return 0;   
        
    }
         
    return 1;
    
       
}




