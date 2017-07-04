#include <stdio.h>
#include <stdarg.h>
//#include <absacc.h>
#include "stm32f4xx.h"
//#include "ucos_ii.h"
#include "mystd.h"
#include "SysAppConfig.h"
#include "main.h"
#include "m_config.h"
#include "InitHW.h"
#include "UserLib.h"
#include "CommHandle.h"
#include "WRProtect.h"
#include "FlashOpt.h"

extern uint32_t  DataPackedNums;
extern uint32_t  ProgramDataLen;
extern uint32_t  Check_A;
extern uint32_t  Check_B;

uint32_t GetSector(uint32_t Address);
uint16_t GetSectorSize(uint32_t addr, uint32_t size_t, FLASH_SECTOR_SIZE *pz);

FLASH_INFO  FlashInfo[12] = 
{
    {FLASH_Sector_0,  ADDR_FLASH_SECTOR_0,  0x4000,  BOOT},
    {FLASH_Sector_1,  ADDR_FLASH_SECTOR_1,  0x4000,  RESERVE},
    {FLASH_Sector_2,  ADDR_FLASH_SECTOR_2,  0x4000,  APP},
    {FLASH_Sector_3,  ADDR_FLASH_SECTOR_3,  0x4000,  UNUSED},
    {FLASH_Sector_4,  ADDR_FLASH_SECTOR_4,  0x10000, UNUSED},
    {FLASH_Sector_5,  ADDR_FLASH_SECTOR_5,  0x20000, UNUSED},   
    {FLASH_Sector_6,  ADDR_FLASH_SECTOR_6,  0x20000, UNUSED},
    {FLASH_Sector_7,  ADDR_FLASH_SECTOR_7,  0x20000, UNUSED},
    {FLASH_Sector_8,  ADDR_FLASH_SECTOR_8,  0x20000, UNUSED},
    {FLASH_Sector_9,  ADDR_FLASH_SECTOR_9,  0x20000, UNUSED},
    {FLASH_Sector_10, ADDR_FLASH_SECTOR_10, 0x20000, UNUSED},
    {FLASH_Sector_11, ADDR_FLASH_SECTOR_11, 0x20000, RESERVE},      
    
    
};
//======================================================================================================
/*******************************************************************************************************
 *
 *         函数名称: 
 *
 *         函数功能: 
 *
 *         入口参数: 
 *
 *         出口参数: 1:写Flash正确
 *                   0:写Flash失败
 *
 *             说明:
 *
 *******************************************************************************************************/
uint16_t  Data2Flash(uint32_t addr, uint8_t *buff, uint32_t size_t)
{
    int i;
    uint32_t  wr_buff;
    FLASH_Status flash_state;
    FLASH_SECTOR_SIZE   sector_size;
    FLASH_SECTOR_SIZE   *ps;
    
    ps = &sector_size;
    FLASH_Unlock();	//解锁
    FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR| FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR); 

    GetSectorSize(addr, size_t, ps);

    //解锁写保护相应扇区
    for(i = sector_size.front; i <= sector_size.near; i++)
    {
        FlashWRPUnlock(i); 
    }
    
    for(i = 0; i < size_t / 4; i++)
    {

        wr_buff = (buff[3] << 24)+(buff[2] << 16) + (buff[1] << 8) + (buff[0] << 0);
        
        
        __disable_irq();
        //flash_state = FLASH_ProgramByte(addr++, *buff++);//写入数据
        flash_state = FLASH_ProgramWord(addr, wr_buff);
        __enable_irq(); 
        
        addr += 4;
        buff += 4;
        if(flash_state != FLASH_COMPLETE)
        {
            FLASH_Lock() ;  //上锁  
            return FALSE;
        }
        
    }
    
    //加锁写保护相应扇区    
    for(i = sector_size.front; i <= sector_size.near; i++)
    { 
        FlashWRPLock(i);
    }
    
    FLASH_Lock() ;  //上锁  
    
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
uint16_t CheckProgramData(uint32_t addr, uint32_t len, uint8_t ca, uint8_t cb)
{
    uint8_t  check_a = 0;
    uint8_t  check_b = 0;
    int      i;
    uint8_t *paddr;
    //uint8_t tmp_data;
    paddr = (uint8_t *)(addr);  //0x08008000  APP1_START_ADDR + APP_RESV_BYTES
        
    for(i = 0; i < len; i++)
    {
         //tmp_data = *paddr++;
         check_a = check_a + *paddr++;
         check_b = check_b + check_a;
         //printf("%d ", tmp_data);
    }
    
    if((check_a == ca) && (check_b == cb))
    {
        return 1;  
    }
    
    
    return 0;
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
uint16_t ProgramFlashErase(uint32_t addr, uint32_t size_t)
{
    uint8_t  i;
    
    FLASH_Status  flash_state;
    FLASH_SECTOR_SIZE   sector_size;
    FLASH_SECTOR_SIZE   *ps;
    
    ps = &sector_size;
    GetSectorSize(addr, size_t, ps);
        
    FLASH_Unlock();	//解锁
    FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR| FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);    

    for(i = sector_size.front; i <= sector_size.near; i++)
    {
        FlashWRPUnlock(i);
        flash_state = FLASH_EraseSector(FlashInfo[i].flash_sector, VoltageRange_3); //擦除  
        FlashWRPLock(i);
    }
    
    FLASH_Lock() ;                //上锁
    
    if(flash_state != FLASH_COMPLETE)
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
uint16_t GetSectorSize(uint32_t addr, uint32_t size_t, FLASH_SECTOR_SIZE *pz)
{
    uint32_t sector_front = 0;
    uint32_t sector_near = 0;
    uint8_t  i;
    
    //FLASH_SECTOR_SIZE   sector_size;

    sector_front = GetSector(addr);
    sector_near  = GetSector(addr + size_t); 
    for(i = 0; i < 12; i++)
    {
        if(sector_front == FlashInfo[i].flash_sector)
        {
            pz->front = i;  
        }
        
        if (sector_near == FlashInfo[i].flash_sector)
        {
            pz->near = i;
        }
            
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
uint32_t GetSector(uint32_t Address)
{
    uint32_t sector = 0;

    if((Address < ADDR_FLASH_SECTOR_1) && (Address >= ADDR_FLASH_SECTOR_0))
    {
        sector = FLASH_Sector_0;  
    }
    else if((Address < ADDR_FLASH_SECTOR_2) && (Address >= ADDR_FLASH_SECTOR_1))
    {
        sector = FLASH_Sector_1;  
    }
    else if((Address < ADDR_FLASH_SECTOR_3) && (Address >= ADDR_FLASH_SECTOR_2))
    {
        sector = FLASH_Sector_2;  
    }
    else if((Address < ADDR_FLASH_SECTOR_4) && (Address >= ADDR_FLASH_SECTOR_3))
    {
        sector = FLASH_Sector_3;  
    }
    else if((Address < ADDR_FLASH_SECTOR_5) && (Address >= ADDR_FLASH_SECTOR_4))
    {
        sector = FLASH_Sector_4;  
    }
    else if((Address < ADDR_FLASH_SECTOR_6) && (Address >= ADDR_FLASH_SECTOR_5))
    {
        sector = FLASH_Sector_5;  
    }
    else if((Address < ADDR_FLASH_SECTOR_7) && (Address >= ADDR_FLASH_SECTOR_6))
    {
        sector = FLASH_Sector_6;  
    }
    else if((Address < ADDR_FLASH_SECTOR_8) && (Address >= ADDR_FLASH_SECTOR_7))
    {
        sector = FLASH_Sector_7;  
    }
    else if((Address < ADDR_FLASH_SECTOR_9) && (Address >= ADDR_FLASH_SECTOR_8))
    {
        sector = FLASH_Sector_8;  
    }
    else if((Address < ADDR_FLASH_SECTOR_10) && (Address >= ADDR_FLASH_SECTOR_9))
    {
        sector = FLASH_Sector_9;  
    }
    else if((Address < ADDR_FLASH_SECTOR_11) && (Address >= ADDR_FLASH_SECTOR_10))
    {
        sector = FLASH_Sector_10;  
    }
    else/*(Address < FLASH_END_ADDR) && (Address >= ADDR_FLASH_SECTOR_11))*/
    {
        sector = FLASH_Sector_11;  
    }

    return sector;    
    
}





