#include <stdio.h>
#include <stdarg.h>
#include <absacc.h>
#include "stm32f4xx.h"
//#include "ucos_ii.h"
#include "mystd.h"
#include "SysAppConfig.h"
#include "main.h"
#include "m_config.h"
#include "InitHW.h"
#include "UserLib.h"
#include "FlashErase.h"


//======================================================================================================
/******************************************************************************************************
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

FLASH_Status FlashErase(uint32_t EraseSectorSize)
{
    FLASH_Status  flash_state;
#if 0 
    FLASH_Unlock();	//解锁
    FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR| FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
    if(EraseSectorSize < 16)
    {
        flash_state = FLASH_EraseSector(FLASH_Sector_2,VoltageRange_3); //擦除
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2即2个字节,16bit   
        }
        
    }
    else if(EraseSectorSize < 32)
    {
        flash_state = FLASH_EraseSector(FLASH_Sector_2,VoltageRange_3); //擦除
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2即2个字节,16bit   
        }
        
        flash_state = FLASH_EraseSector(FLASH_Sector_3,VoltageRange_3); //擦除 
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2即2个字节,16bit   
        }        
    }
    else if(EraseSectorSize < 96)
    {
        flash_state = FLASH_EraseSector(FLASH_Sector_2,VoltageRange_3); //擦除
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2即2个字节,16bit   
        }
        
        flash_state = FLASH_EraseSector(FLASH_Sector_3,VoltageRange_3); //擦除
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2即2个字节,16bit   
        }
        
        flash_state = FLASH_EraseSector(FLASH_Sector_4,VoltageRange_3); //擦除   
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2即2个字节,16bit   
        }        
    }
    else if(EraseSectorSize < 224)
    {
        flash_state = FLASH_EraseSector(FLASH_Sector_2,VoltageRange_3); //擦除
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2即2个字节,16bit   
        }
        
        flash_state = FLASH_EraseSector(FLASH_Sector_3,VoltageRange_3); //擦除
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2即2个字节,16bit   
        }
        
        flash_state = FLASH_EraseSector(FLASH_Sector_4,VoltageRange_3); //擦除 
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2即2个字节,16bit   
        }
        
        flash_state = FLASH_EraseSector(FLASH_Sector_5,VoltageRange_3); //擦除 
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2即2个字节,16bit   
        }        
    }
    FLASH_Lock() ;                //上锁
#endif    
    
    
    return  flash_state;   
}





