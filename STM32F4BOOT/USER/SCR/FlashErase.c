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
 *         ��������: 
 *
 *         ��������: 
 *
 *         ��ڲ���: 
 *
 *         ���ڲ���: ��
 *
 *             ˵��:
 *
 *******************************************************************************************************/

FLASH_Status FlashErase(uint32_t EraseSectorSize)
{
    FLASH_Status  flash_state;
#if 0 
    FLASH_Unlock();	//����
    FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR| FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
    if(EraseSectorSize < 16)
    {
        flash_state = FLASH_EraseSector(FLASH_Sector_2,VoltageRange_3); //����
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2��2���ֽ�,16bit   
        }
        
    }
    else if(EraseSectorSize < 32)
    {
        flash_state = FLASH_EraseSector(FLASH_Sector_2,VoltageRange_3); //����
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2��2���ֽ�,16bit   
        }
        
        flash_state = FLASH_EraseSector(FLASH_Sector_3,VoltageRange_3); //���� 
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2��2���ֽ�,16bit   
        }        
    }
    else if(EraseSectorSize < 96)
    {
        flash_state = FLASH_EraseSector(FLASH_Sector_2,VoltageRange_3); //����
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2��2���ֽ�,16bit   
        }
        
        flash_state = FLASH_EraseSector(FLASH_Sector_3,VoltageRange_3); //����
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2��2���ֽ�,16bit   
        }
        
        flash_state = FLASH_EraseSector(FLASH_Sector_4,VoltageRange_3); //����   
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2��2���ֽ�,16bit   
        }        
    }
    else if(EraseSectorSize < 224)
    {
        flash_state = FLASH_EraseSector(FLASH_Sector_2,VoltageRange_3); //����
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2��2���ֽ�,16bit   
        }
        
        flash_state = FLASH_EraseSector(FLASH_Sector_3,VoltageRange_3); //����
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2��2���ֽ�,16bit   
        }
        
        flash_state = FLASH_EraseSector(FLASH_Sector_4,VoltageRange_3); //���� 
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2��2���ֽ�,16bit   
        }
        
        flash_state = FLASH_EraseSector(FLASH_Sector_5,VoltageRange_3); //���� 
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2��2���ֽ�,16bit   
        }        
    }
    FLASH_Lock() ;                //����
#endif    
    
    
    return  flash_state;   
}





