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
 *******************************************************************************************************/
//======================================================================================================

#include "stm32f4xx.h"
#include "Flash.h"

//======================================================================================================

volatile FLASH_Status  FlashStatus;

u8 ReadByte[1024] = {0};
u32 Address = 0,SectorCounter = 0;
/**************************************************************************
*函数名称: InitFlash
*参数：void
*返回值:void
*函数功能: 初始化Flash
*****************************************************************************/
void InitFlash(void)
{
  //Unlocks the FLASH Program Erase Controller.
  FLASH_Unlock();

  //Clear All pending flags
  FLASH_ClearFlag(FLASH_FLAG_BSY|FLASH_FLAG_PGAERR|FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR|FLASH_FLAG_WRPERR|FLASH_FLAG_EOP);
  

}

/*****************************************************************************
*函数名称: ModifyBytes
*参数：void
*返回值:void
*函数功能: 修改某一地址的数据
*****************************************************************************/
void ModifyBytes(u32 page_adr,u16 byte_num,u8 modify_data )
{

   ReadFlashPageByte(page_adr,ReadByte);

   ModifyFlashPageByte(byte_num,modify_data);

	 SectorCounter = GetSector(page_adr);
	 FlashStatus = FLASH_EraseSector(SectorCounter, VoltageRange_3);
   //FlashStatus =FLASH_ErasePage(page_adr);
    
   WriteFlashPageByte(page_adr,ReadByte);

}

/****************************************************************************
*函数名称: ReadFlashPageByte
*参数：void
*返回值:void
*函数功能: 读取flash页内字节
*****************************************************************************/
void ReadFlashPageByte(u32 ad,u8 *rd)
{
	u32 page_ad;

	page_ad = ad;

	while ( ad < page_ad + (u32)FLASH_PAGE_SIDE)
	{
	  *rd++ = (*(vu8*)ad);
	  ad++;
	
	}

}
/******************************************************************************
*函数名称: ReadFlashFlag
*参数：void
*返回值:void
*函数功能: 读flash标记
*****************************************************************************/
u8 ReadFlashFlag(u32 ad_flag)
{
	u8 tmp_flag;

	tmp_flag = (*(vu8*)ad_flag);
	return tmp_flag;

}

/****************************************************************************
*函数名称: WriteFlashPageByte
*参数：void
*返回值:void
*函数功能: 向flash写字节
*****************************************************************************/
void WriteFlashPageByte(u32 ad,u8 *data)
{
   u32 page_ad;
   u16 tmp;

   page_ad = ad;

   //FlashStatus =FLASH_ErasePage(ad);
   SectorCounter = GetSector(ad);
	 FlashStatus = FLASH_EraseSector(SectorCounter, VoltageRange_3);
	
   while((ad < page_ad + (u32)FLASH_PAGE_SIDE ) && (FlashStatus == FLASH_COMPLETE))
   {
   		tmp = (*data++);
		tmp = tmp + ((*data++)<< 8); 
   
		FlashStatus = FLASH_ProgramHalfWord(ad, tmp);//按半字写入
        ad += 2;
   }

} 

/****************************************************************************
*函数名称: ModifyFlashPageByte
*参数：void
*返回值:void
*函数功能: 修改一定数目的字节
*****************************************************************************/
void ModifyFlashPageByte(u16 byte_num,u8 data)
{
   ReadByte[byte_num-1] = data;

}

/*****************************************************************************
*函数名称: ModifyFlashPageArrayByte
*参数：void
*返回值:void
*函数功能: 修改数组字节
*****************************************************************************/
void ModifyFlashPageArrayByte(u8 *data,u16 byte_len,u16 byte_num)
{
   u8 i;

   for(i =0;i< byte_len;i++)
   {
   	  ReadByte[byte_num-i] = *data++;
   }

}

/****************************************************************************
*函数名称: WriteFlashPageByte
*参数：void
*返回值:void
*函数功能: 
*****************************************************************************/
void ModifyArray(u16 byte_num,u8 *data,u16 byte_len,u32 page_adr)
{
   ReadFlashPageByte(page_adr,ReadByte);

   ModifyFlashPageArrayByte(data,byte_len,byte_num);

   //FlashStatus =FLASH_ErasePage(page_adr);
    
   WriteFlashPageByte(page_adr,ReadByte);

}
/**
  * @brief  Gets the sector of a given address
  * @param  None
  * @retval The sector of a given address
  */
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
