#include "stm32f4xx.h"
#include "mystd.h"  
#include "m_config.h"
#include "MX25L.h"
#include "UserLib.h"
//#include "mystd.h"


//assign {sfdi,sfcs,sfclk} = {ADDR[1],ADDR[2], CS0};
//sssign INT1 = sfdo;

    
//SFDI  -- A1   -- PC4
//SFCS  -- A2   -- PB10 
//SFCLK -- CS0  -- PD7
//SFDO  -- INT1 -- PC1

#define  MX25_IO_DELAY()    Delay(500)

/**********************************************************************
 * Function:       Initial_Spi
 * Arguments:      None
 * Description:    Initial spi flash state and wait flash warm-up
 *                 (enable read/write).
 * Return Message: None
 **********************************************************************/
void InitSimuSPI(void)
{

    /**************************************************
    *   SPI1_NSS	    PC4	        
    *   SPI1_SCK	    PD7	         
    *   SPI1_MISO	    PC1         
    *   SPI1_MOSI	    PB10     	    
    *********************************************/ 
    //SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    
    //RCC_APB1PeriphClockCmd( RCC_APB1Periph_SPI2,  ENABLE);
    //////RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_GPIOB,  ENABLE);

    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4;
    GPIO_Init(GPIOC, &GPIO_InitStructure);    
   
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7;
    GPIO_Init(GPIOD, &GPIO_InitStructure); 
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_Init(GPIOB, &GPIO_InitStructure); 
    
    
    GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_1;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    
      
    MX25_CS_HI();
    MX25_IO_DELAY();
    
    MX25_CLK_HI();
    MX25_IO_DELAY();
    MX25_SI_LO();   
    MX25_IO_DELAY();    
    // Wait flash warm-up
    Wait_Flash_WarmUp();
    
    
}

/****************************************************************
*Function：CS_Low
*Arguments：void
*Description：片选信号拉低
*Return Message：void
****************************************************************/
void CS_Low(void)
{
     MX25_CS_LO();
}

/****************************************************************
*Function：CS_High
*Arguments：void
*Description：片选信号拉高
*Return Message：void
****************************************************************/
void CS_High(void)
{
	MX25_CS_HI();
}

/***************************************************************
 * Function:       Wait_Flash_WarmUp
 * Arguments:      None.
 * Description:    Wait some time until flash read / write enable.
 * Return Message: None.
 ****************************************************************/
void Wait_Flash_WarmUp(void)
{
    uint32_t time_cnt = FlashFullAccessTime;
    while( time_cnt > 0 )
    {
        time_cnt--;
    }
}

/****************************************************************
*Function：SentByte
*Arguments：byte,发送给Flash的数据
*Description：向Flash发送一个字节数据
*Return Message：void
****************************************************************/
void SendByte(uint8_t byte)
{
	uint16_t i;
	
    MX25_IO_DELAY();
	for (i = 0; i < 8; i++ )
	{
		if((byte & IO_MASK) == 0x80)
		{
			MX25_SI_HI();
            
		}
		else
		{
			MX25_SI_LO();
		}
		
        MX25_IO_DELAY();
        
		MX25_CLK_LO(); 
        
		MX25_IO_DELAY();
        
        byte <<= 1;                    //高位在前，低位在后
        
		MX25_CLK_HI();	               //上升沿通知MX25L 输入下一个串行数据
        
        MX25_IO_DELAY();
        
	}
}

/****************************************************************
*Function：SentByte
*Arguments：void
*Description：从Flash接收一个字节数据
*Return Message：8 bit data
****************************************************************/
uint8_t GetByte(void)
{
	uint16_t i;
	uint8_t data = 0;
	
    MX25_IO_DELAY();
    
	MX25_SO_HI();                     //设置MX25_SO 线为输入口，需要上拉至高电平
	
	for (i = 0; i < 8; i++)
	{
		MX25_IO_DELAY();
        MX25_CLK_LO();               //下降沿通知MX25L 输入下一个串行数据
        
        MX25_IO_DELAY();
		if (MX25_SO())
		{
			data = (data | (0x80 >> i));
		}
        
        MX25_IO_DELAY();
		MX25_CLK_HI();
        
	}
	
    MX25_IO_DELAY();

	return data;
}

/****************************************************************
*Function：InsertDummyCycle
*Arguments：dummy_cycle, 需要插入的虚拟时钟周期
*Description：插入虚拟时钟周期
*Return Message：void
****************************************************************/
void InsertDummyCycle(uint8_t dummy_cycle)
{
	uint8_t i;
	
	for (i = 0; i < dummy_cycle; i++)
	{
		MX25_IO_DELAY();
        MX25_CLK_LO();
        MX25_IO_DELAY();
		MX25_CLK_HI();
        MX25_IO_DELAY();
	}
}

/****************************************************************
*Function：IsFlashBusy
*Arguments：void
*Description：判断Flash是否处于忙状态
*Return Message：8 bit data
****************************************************************/
BOOL IsFlashBusy(void)
{
	uint8_t flash_status;
	CMD_RDSR(&flash_status);
	
	if ((flash_status & FLASH_WIP_MASK) == FLASH_WIP_MASK)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}

}

/****************************************************************
*Function：WaitFlashReady
*Arguments：wait_time,等待时间
*Description：等待Flash准备
*Return Message：8 bit data
****************************************************************/
BOOL WaitFlashReady(uint32_t wait_time)
{
	uint32_t temp = 0;
	
	while(IsFlashBusy())
	{
		if (temp > wait_time)
		{
			return FALSE;
		}
        
		temp++;
		
	}
	
	return TRUE;

}
/**************************************************************************
 * Function:       WaitRYBYReady
 * Arguments:      wait_time, expected time-out value of flash operations.
 *                 No use at non-synchronous IO mode.
 * Description:    Synchronous IO:
 *                 If flash is ready return TRUE.
 *                 If flash is time-out return FALSE.
 *                 Non-synchronous IO:
 *                 Always return TRUE
 * Return Message: TRUE, FALSE
 ***************************************************************************/
BOOL WaitRYBYReady( uint32_t wait_time )
{
	uint32_t temp = 0;
	
	while(MX25_SO())
	{
		if(temp > wait_time)
		{
			return FALSE;
		}
		temp++;
	}
	
	return TRUE;
}
/****************************************************************
 * Function:       IsFlashQIO
 * Arguments:      None.
 * Description:    If flash QE bit = 1: return TRUE
 *                                 = 0: return FALSE.
 * Return Message: TRUE, FALSE
 ***************************************************************/
BOOL IsFlashQIO( void )
{
#ifdef FLASH_NO_QE_BIT
    return TRUE;
#else
    uint8_t  qe;
    CMD_RDSR( &qe );
    if( (qe & FLASH_QE_MASK) == FLASH_QE_MASK )
        return TRUE;
    else
        return FALSE;
#endif
}
/*******************************************************************
 * Function:       IsFlash4Byte
 * Arguments:      None
 * Description:    Check flash address is 3-byte or 4-byte.
 *                 If flash 4BYTE bit = 1: return TRUE
 *                                    = 0: return FALSE.
 * Return Message: TRUE, FALSE
 ********************************************************************/
BOOL IsFlash4Byte( void )
{
#ifdef FLASH_CMD_RDSCUR
	#ifdef FLASH_4BYTE_ONLY 
		return TURE;	
	#elif  FLASH_3BYTE_ONLY
		return FALSE;	
	#else
		uint8_t security;
		CMD_RDSCUR(&security);
		if ((security & FLASH_4BYTE_MASK) == FLASH_4BYTE_MASK)
			return TRUE;
		else
			return FALSE;	
	#endif
#else
	return FALSE;
#endif

}
/***********************************************************************
 * Function:       SendFlashAddr
 * Arguments:      flash_address, 32 bit flash memory address
 *                 addr_4byte_mode,
 * Description:    Send flash address with 3-byte or 4-byte mode.
 * Return Message: None
 ***********************************************************************/
void SendFlashAddr( uint32_t flash_address, BOOL addr_4byte_mode )
{
	//发送地址，高位在前，低位在后
	if(addr_4byte_mode == TRUE)             
	{
		SendByte(flash_address >> 24);
	}
	
	SendByte(flash_address >> 16);
	SendByte(flash_address >> 8);
	SendByte(flash_address >> 0);

} 
/****************************************************************
*Function：CMD_WREN
*Arguments：void
*Description：Set Write Enable Latch(WEL) Bit
*Return Message：FlashOperationSuccess
****************************************************************/
ReturnMsg CMD_WREN(void)
{
	CS_Low();                                     //CS 拉低，开始SI输入命令
	
	SendByte(FLASH_CMD_WREN);                     //SI 发送命令
 	
	CS_High();                                    //CS 拉高，读取数据结束	
	
	return FlashOperationSuccess;
}

/****************************************************************
 * Function:       CMD_WRDI
 * Arguments:      void.
 * Description:    Reset Write Enable Latch (WEL) bit.
 * Return Message: FlashOperationSuccess
 ****************************************************************/
ReturnMsg CMD_WRDI(void)
{
	CS_Low();                                     //CS 拉低，开始SI输入命令
	
	SendByte(FLASH_CMD_WRDI);                     //SI 发送命令
 	
	CS_High();                                    //CS 拉高，读取数据结束	
	
	return FlashOperationSuccess;
}

/****************************************************************
*Function：CMD_RDID
*Arguments：Id，32 bit buff to store id
*Description：读取MX25LxID号,第一个字节为厂商ID（manufacturer ID）
*				后两个字节为设备ID（ Device ID）
*Return Message：FlashOperationSuccess
****************************************************************/
ReturnMsg CMD_RDID(uint32_t *Id)
{
    uint32_t temp;
	uint8_t  gIdBuff[3];
	
	CS_Low();                                     //CS 拉低，开始SI输入命令
	
	SendByte(FLASH_CMD_RDID);                     //SI 发送命令
 
	gIdBuff[0] = GetByte();                       //SO 接收一个字节数据
	gIdBuff[1] = GetByte();                       //SO 接收一个字节数据
	gIdBuff[2] = GetByte();                       //SO 接收一个字节数据
	
	CS_High();                                    //CS 拉高，读取数据结束
	
	temp = gIdBuff[0];                            //厂商ID(manufacturer ID)
	temp = (temp << 8) | gIdBuff[1];              //设备ID(Device ID)
	temp = (temp << 8)| gIdBuff[2];                //设备ID(Device ID)
	
	*Id = temp;                                   //存储ID号
	
	return FlashOperationSuccess;

}

/****************************************************************
*Function：CMD_RES
*Arguments：ElectricId，8 bit buff to store id
*Description：读取电气 ID号,（electric ID）
*Return Message：FlashOperationSuccess
****************************************************************/
ReturnMsg CMD_RES(uint8_t *ElectricId)
{
	CS_Low();                                     //CS 拉低，开始SI输入命令
	
	SendByte(FLASH_CMD_RES);                     //SI 发送命令
	InsertDummyCycle(24);                         //插入3 Byte 的虚拟CLK时钟
	
	*ElectricId = GetByte();                      //SO 接收一个字节数据
	CS_High();                                    //CS 拉高，读取数据结束

	return FlashOperationSuccess;	
}

/****************************************************************
*Function：CMD_REMS
*Arguments：REMS_Id，16 bit buff to store id
*           pFS，point to FalshStatus
*Description：读取RWMS ID号
*Return Message：FlashOperationSuccess
****************************************************************/
ReturnMsg CMD_REMS(uint16_t *REMS_Id, FlashStatus *pFS)
{
	uint8_t  gIdBuff[2] = {0};
    
	CS_Low();                                     //CS 拉低，开始SI输入命令
	
	SendByte(FLASH_CMD_REMS);                     //SI 发送命令
	InsertDummyCycle(16);                         //插入2 Byte 的虚拟CLK时钟
	SendByte(pFS->ArrangeOpt);                    //SI 发送 ArrangeOpt操作选项，ArrangeOpt == 0：manufacturer's ID first, ArrangeOpt == 1: Device' ID first
	
	gIdBuff[0] = GetByte();                          //SO 接收一个字节数据
	gIdBuff[1] = GetByte();                          //SO 接收一个字节数据
	
	CS_High();                                    //CS 拉高，读取数据结束

	*REMS_Id = (gIdBuff[0] << 8) | gIdBuff[1];
	
	return FlashOperationSuccess;	

}

/****************************************************************
*Function：CMD_RDSR
*Arguments：StatusReg，8 bit buff to store id
*Description：读取状态寄存器
*Return Message：FlashOperationSuccess
****************************************************************/
ReturnMsg CMD_RDSR(uint8_t *StatusReg)
{
	CS_Low();                                     //CS 拉低，开始SI输入命令
	
	SendByte(FLASH_CMD_RDSR);                     //SI 发送命令
	
	*StatusReg = GetByte();                       //SO 接收一个字节数据
	CS_High();                                    //CS 拉高，读取数据结束

	return FlashOperationSuccess;	
}

/****************************************************************
*Function：CMD_WRSR
*Arguments：UpdateValue，8 bit buff to write
*Description：改变状态寄存器
*Return Message：FlashOperationSuccess
****************************************************************/
ReturnMsg CMD_WRSR(uint8_t UpdateValue)
{
	if (IsFlashBusy())        //检查Flash是否处于忙状态
	{
		return FlashIsBusy;
	}
	
	CMD_WREN();                                   //WP 使能
	
	CS_Low();                                     //CS 拉低，开始SI输入命令
	
	SendByte(FLASH_CMD_WRSR);                     //SI 发送命令
	SendByte(UpdateValue);                        //SI 发送状态寄存器更新值

	CS_High();                                    //CS 拉高，读取数据结束

	if (WaitFlashReady(WriteStatusRegCycleTime))  //等待Flash操作完成
	{
		return FlashOperationSuccess;	
	}
	else
	{
		return FlashTimeOut;
	}

}

/****************************************************************
*Function：CMD_RDSCUR
*Arguments：SecurityReg，8 bit buff to store id
*Description：读取安全寄存器
*Return Message：FlashOperationSuccess
****************************************************************/
ReturnMsg CMD_RDSCUR(uint8_t *SecurityReg)
{
	CS_Low();                                     //CS 拉低，开始SI输入命令
	
	SendByte(FLASH_CMD_RDSCUR);                   //SI 发送命令
	
	*SecurityReg = GetByte();                     //SO 接收一个字节数据
	CS_High();                                    //CS 拉高，读取数据结束

	return FlashOperationSuccess;	

}

/****************************************************************
*Function：CMD_WRSCUR
*Arguments：void
*Description：写安全寄存器
*Return Message：FlashOperationSuccess
****************************************************************/
ReturnMsg CMD_WRSCUR(void)
{
	uint8_t scurity;
	
	if (IsFlashBusy())        //检查Flash是否处于忙状态
	{
		return FlashIsBusy;
	}
	
	CMD_WREN();                                   //WP 使能
	
	CS_Low();                                     //CS 拉低，开始SI输入命令
	
	SendByte(FLASH_CMD_WRSCUR);                   //SI 发送命令

	CS_High();                                    //CS 拉高，读取数据结束
	
	if (WaitFlashReady(WriteSecuRegCycleTime))
	{
		CMD_RDSCUR(&scurity);
		if((scurity & FLASH_LDSO_MASK) == FLASH_LDSO_MASK)   //检查写入是否正确
		{
			return FlashOperationSuccess;
		}
		else
		{
			return FlashWriteRegFailed;		
		}
	
	}

	return FlashTimeOut;	

}

/****************************************************************************************
 * Function:       CMD_READ
 * Arguments:      flash_address, 32 bit flash memory address
 *                 target_address, buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 * Description:    The READ instruction is for reading data out.
 * Return Message: FlashAddressInvalid, FlashOperationSuccess
 ****************************************************************************************/
ReturnMsg CMD_READ( uint32_t flash_address, uint8_t *target_address, uint32_t byte_length )
{
	uint32_t i;
	uint8_t addr_4byte_mode;
	
	if (flash_address > FlashSize)
	{
		return FlashAddressInvalid;
	}
	
	if (IsFlash4Byte())
	{
		addr_4byte_mode = TRUE;     //4字节模式
	}
	else
	{
		addr_4byte_mode = FALSE;    //3字节模式
	}
	
	CS_Low();                                          //CS 拉低，开始SI输入命令
	
	SendByte(FLASH_CMD_READ);                          //SI 发送命令
	SendFlashAddr(flash_address, addr_4byte_mode);     //SI 地址
	
	for (i = 0; i < byte_length; i++)
	{
		*(target_address + i)= GetByte();              //SO 接收一个字节数据
	}

	CS_High();                                        //CS 拉高，读取数据结束	
	
	return FlashOperationSuccess;
}

/*********************************************************************************
 * Function:       CMD_FASTREAD
 * Arguments:      flash_address, 32 bit flash memory address
 *                 target_address, buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 * Description:    The FASTREAD instruction is for quickly reading data out.
 * Return Message: FlashAddressInvalid, FlashOperationSuccess
 ********************************************************************************/
ReturnMsg CMD_FASTREAD( uint32_t flash_address, uint8_t *target_address, uint32_t byte_length )
{
	uint32_t i;
	uint8_t addr_4byte_mode;
	
	if (flash_address > FlashSize)           //检查Flash地址是否超界
	{
		return FlashAddressInvalid;
	}
	
	if (IsFlash4Byte())                      //检查是否是4byte模式
	{
		addr_4byte_mode = TRUE;     //4字节模式
	}
	else
	{
		addr_4byte_mode = FALSE;    //3字节模式
	}
	
	CS_Low();                                          //CS 拉低，开始SI输入命令
	
	SendByte(FLASH_CMD_READ);                          //SI 发送命令
	SendFlashAddr(flash_address, addr_4byte_mode);     //SI 地址
	InsertDummyCycle(8);                               //插入1个字节的虚拟时钟
	
	for (i = 0; i < byte_length; i++)
	{
		*(target_address + i)= GetByte();              //SO 接收一个字节数据
	}

	CS_High();                                    //CS 拉高，读取数据结束	
	
	return FlashOperationSuccess;


}
/*****************************************************************************************
 * Function:       CMD_RDSFDP
 * Arguments:      flash_address, 32 bit flash memory address
 *                 target_address, buffer address to store returned data
 *                 byte_length, length of returned data in byte unit
 * Description:    RDSFDP can retrieve the operating characteristics, structure
 *                 and vendor-specified information such as identifying information,
 *                 memory size, operating voltages and timinginformation of device
 * Return Message: FlashAddressInvalid, FlashOperationSuccess
 ******************************************************************************************/
ReturnMsg CMD_RDSFDP( uint32_t flash_address, uint8_t *target_address, uint32_t byte_length )
{
	uint32_t i;
	uint8_t addr_4byte_mode;
	
	if (flash_address > FlashSize)           //检查Flash地址是否超界
	{
		return FlashAddressInvalid;
	}
	
	if (IsFlash4Byte())                      //检查是否是4byte模式
	{
		addr_4byte_mode = TRUE;     //4字节模式
	}
	else
	{
		addr_4byte_mode = FALSE;    //3字节模式
	}
		
	CS_Low();                                          //CS 拉低，开始SI输入命令
	
	SendByte(FLASH_CMD_READ);                          //SI 发送命令
	SendFlashAddr(flash_address, addr_4byte_mode);     //SI 地址
	InsertDummyCycle(8);                               //插入1个字节的虚拟时钟
	
	for (i = 0; i < byte_length; i++)
	{
		*(target_address + i)= GetByte();              //SO 接收一个字节数据
	}

	CS_High();                                        //CS 拉高，读取数据结束	
	
	return FlashOperationSuccess;
	
}
/**************************************************************************************
 * Function:       CMD_PP
 * Arguments:      flash_address, 32 bit flash memory address
 *                 source_address, buffer address of source data to program
 *                 byte_length, byte length of data to programm
 * Description:    The PP instruction is for programming
 *                 the memory to be "0".
 *                 The device only accept the last 256 byte ( or 32 byte ) to program.
 *                 If the page address ( flash_address[7:0] ) reach 0xFF, it will
 *                 program next at 0x00 of the same page.
 *                 Some products have smaller page size ( 32 byte )
 * Return Message: FlashAddressInvalid, FlashIsBusy, FlashOperationSuccess,
 *                 FlashTimeOut
 ***************************************************************************************/
ReturnMsg CMD_PP( uint32_t flash_address, uint8_t *source_address, uint32_t byte_length )
{
	uint8_t i;
	uint8_t addr_4byte_mode;
	
	if (flash_address > FlashSize)                    //检查Flash地址是否超界
	{
		return FlashAddressInvalid;
	}
	
	if (IsFlashBusy())                                 //检查Flash是否处于忙状态
	{
		return FlashIsBusy;
	}
	
	if (IsFlash4Byte())                                //检查是否是4byte模式
	{
		addr_4byte_mode = TRUE;     //4字节模式
	}
	else
	{
		addr_4byte_mode = FALSE;    //3字节模式
	}
		
	CMD_WREN();                                        //WP 使能
	
	CS_Low();                                          //CS 拉低，开始SI输入命令
	
	SendByte(FLASH_CMD_PP);                            //SI 发送命令
	SendFlashAddr(flash_address, addr_4byte_mode);     //SI 地址
	
	for (i = 0; i < byte_length; i++)
	{
		SendByte(*(source_address + i)); 	
	}
	
	CS_High();                                        //CS 拉高，读取数据结束		

	if (WaitFlashReady(PageProgramCycleTime))        //等待Flash操作完成
	{
		return FlashOperationSuccess;
	}
	else
	{
		return FlashTimeOut;
	}
}

/****************************************************************************
 * Erase Command
 ***************************************************************************/

/***************************************************************************
 * Function:       CMD_SE
 * Arguments:      flash_address, 32 bit flash memory address
 * Description:    The SE instruction is for erasing the data
 *                 of the chosen sector (4KB) to be "1".
 * Return Message: FlashAddressInvalid, FlashIsBusy, FlashOperationSuccess,
 *                 FlashTimeOut
 ***************************************************************************/
ReturnMsg CMD_SE( uint32_t flash_address )
{
	uint8_t addr_4byte_mode;
	
	if (flash_address > FlashSize)                    //检查Flash地址是否超界
	{
		return FlashAddressInvalid;
	}
	
	if (IsFlashBusy())                                 //检查Flash是否处于忙状态
	{
		return FlashIsBusy;
	}
	
	if (IsFlash4Byte())                                //检查是否是4byte模式
	{
		addr_4byte_mode = TRUE;     //4字节模式
	}
	else
	{
		addr_4byte_mode = FALSE;    //3字节模式
	}
		
	CMD_WREN();                                        //WP 使能
	
	CS_Low();                                          //CS 拉低，开始SI输入命令
	
	SendByte(FLASH_CMD_SE);                            //SI 发送命令
	SendFlashAddr(flash_address, addr_4byte_mode);     //SI 地址
	
	CS_High();                                         //CS 拉高，读取数据结束		

	if (WaitFlashReady(SectorEraseCycleTime))         //等待Flash操作完成
	{
		return FlashOperationSuccess;
	}
	else
	{
		return FlashTimeOut;
	}
}

/*******************************************************************************
 * Function:       CMD_BE
 * Arguments:      flash_address, 32 bit flash memory address
 * Description:    The BE instruction is for erasing the data
 *                 of the chosen sector (64KB) to be "1".
 * Return Message: FlashAddressInvalid, FlashIsBusy, FlashOperationSuccess,
 *                 FlashTimeOut
 *******************************************************************************/
ReturnMsg CMD_BE( uint32_t flash_address )
{
	uint8_t addr_4byte_mode;
	
	if (flash_address > FlashSize)                    //检查Flash地址是否超界
	{
		return FlashAddressInvalid;
	}
	
	if (IsFlashBusy())                                 //检查Flash是否处于忙状态
	{
		return FlashIsBusy;
	}
	
	if (IsFlash4Byte())                                //检查是否是4byte模式
	{
		addr_4byte_mode = TRUE;     //4字节模式
	}
	else
	{
		addr_4byte_mode = FALSE;    //3字节模式
	}
		
	CMD_WREN();                                        //WP 使能
	
	CS_Low();                                          //CS 拉低，开始SI输入命令
	
	SendByte(FLASH_CMD_BE);                            //SI 发送命令
	SendFlashAddr(flash_address, addr_4byte_mode);     //SI 地址
	
	CS_High();                                         //CS 拉高，读取数据结束		

	if (WaitFlashReady(BlockEraseCycleTime))           //等待Flash操作完成
	{
		return FlashOperationSuccess;
	}
	else
	{
		return FlashTimeOut;
	}

}

/*******************************************************************************
 * Function:       CMD_CE
 * Arguments:      None.
 * Description:    The CE instruction is for erasing the data
 *                 of the whole chip to be "1".
 * Return Message: FlashIsBusy, FlashOperationSuccess, FlashTimeOut
 ******************************************************************************/
ReturnMsg CMD_CE( void )
{
	if (IsFlashBusy())                                 //检查Flash是否处于忙状态
	{
		return FlashIsBusy;
	}
	
	CMD_WREN();                                        //WP 使能
	
	CS_Low();                                          //CS 拉低，开始SI输入命令
	
	SendByte(FLASH_CMD_CE);                            //SI 发送命令
	
	CS_High();                                         //CS 拉高，读取数据结束		

	if (WaitFlashReady(ChipEraseCycleTime))            //等待Flash操作完成
	{
		return FlashOperationSuccess;
	}
	else
	{
		return FlashTimeOut;
	}	
}

/******************************************************************************
 * Mode setting Command
 ******************************************************************************/

/******************************************************************************
 * Function:       CMD_DP
 * Arguments:      None.
 * Description:    The DP instruction is for setting the
 *                 device on the minimizing the power consumption.
 * Return Message: FlashOperationSuccess
 ******************************************************************************/
ReturnMsg CMD_DP(void)
{
	CS_Low();                                          //CS 拉低，开始SI输入命令
	
	SendByte(FLASH_CMD_DP);                            //SI 发送命令
	
	CS_High();                                         //CS 拉高，读取数据结束

	return FlashOperationSuccess;    
}

/*******************************************************************************
 * Function:       CMD_RDP
 * Arguments:      None.
 * Description:    The Release from RDP instruction is
 *                 putting the device in the Stand-by Power mode.
 * Return Message: FlashOperationSuccess
 *******************************************************************************/
ReturnMsg CMD_RDP( void )
{
	CS_Low();                                          //CS 拉低，开始SI输入命令
	
	SendByte(FLASH_CMD_RDP);                           //SI 发送命令
	
	CS_High();                                         //CS 拉高，读取数据结束

	return FlashOperationSuccess;  
}

/*******************************************************************************
 * Function:       CMD_ENSO
 * Arguments:      None.
 * Description:    The ENSO instruction is for entering the secured OTP mode.
 * Return Message: FlashOperationSuccess
 *******************************************************************************/
ReturnMsg CMD_ENSO( void )
{
	CS_Low();                                          //CS 拉低，开始SI输入命令
	
	SendByte(FLASH_CMD_ENSO);                          //SI 发送命令
	
	CS_High();                                         //CS 拉高，读取数据结束

	return FlashOperationSuccess;  
}

/*********************************************************************************
 * Function:       CMD_EXSO
 * Arguments:      None.
 * Description:    The EXSO instruction is for exiting the secured OTP mode.
 * Return Message: FlashOperationSuccess
 **********************************************************************************/
ReturnMsg CMD_EXSO( void )
{
	CS_Low();                                          //CS 拉低，开始SI输入命令
	
	SendByte(FLASH_CMD_EXSO);                          //SI 发送命令
	
	CS_High();                                         //CS 拉高，读取数据结束

	return FlashOperationSuccess; 
}












