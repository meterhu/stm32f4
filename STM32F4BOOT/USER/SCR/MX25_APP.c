/*
 * COPYRIGHT (c) 2010-2012 MACRONIX INTERNATIONAL CO., LTD
 * SPI Flash Low Level Driver (LLD) Sample Code
 *
 * Application program of SPI flash
 * This sample code provides a reference, not recommand for directing using.
 *
 * $Id: MX25_APP.c,v 1.4 2012/01/03 08:00:06 modelqa Exp $
 */

#include    <stdlib.h>
#include "stm32f4xx.h" 
#include "mystd.h"  
#include "m_config.h"
//#include    "MX25_CMD.h"
#include  "MX25L.h"

#define  TRANS_LENGTH  16
#define  RANDOM_SEED   106
#define  FLASH_TARGET_ADDR  0x00000000

#define  Error_inc(x)  x = x + 1;

/* function prototype */
uint8_t  FlashID_Test( void );
uint8_t  FlashReadWrite_Test( void );

uint32_t FlashID_EXT ;


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
int Flash_Read(uint32_t flash_addr, uint8_t* pb, uint32_t buff_size)
{
    return CMD_READ(flash_addr, pb, buff_size);
        
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
int Flash_Write(uint32_t flash_addr, uint8_t* pb, uint32_t buff_size)
{
    //4K一个扇区
    //MX25L6406E
    //64M bit
    //2048 Equal Sectors with 4K byte each
    //Any Sector can be erased individually
    
    ///CMD_READ(flash_addr, pb, buff_size);
    
    CMD_SE(flash_addr);
    
    return CMD_PP(flash_addr, pb, buff_size);

        
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
void TestMX25LPin(void)
{    
    MX25_CS_LO();
    MX25_CS_HI();
  
  //  MX25_SO_LO();
  //  MX25_SO_HI();

    MX25_SI_LO();
    MX25_SI_HI();
    
    MX25_CLK_LO();
    MX25_CLK_HI();

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
uint8_t ReadFlashID(uint32_t  *id)
{
    ReturnMsg  msg;

    //Read flash device id 
    msg =  CMD_RDID(id);
    if( msg != (ReturnMsg)FlashOperationSuccess )
        return FALSE;
      
    return TRUE;
    
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
uint8_t FlashID_Test(void)
{
    uint32_t  flash_id = 0;
    uint16_t  error_cnt = 0;
    uint16_t  rems_id;
    uint8_t   electric_id = 0;
    uint8_t   security_reg = 0xAA;
    FlashStatus  flash_state = {0};
    ReturnMsg  msg;
    uint32_t  flash_addr = 0x000000;
    uint32_t  trans_len = 0x40;
    uint8_t   unique_id_buff[0x40] = {0};

    /* Read flash device id */
    msg =  CMD_RDID( &flash_id );
    if( msg != (ReturnMsg)FlashOperationSuccess )
        return FALSE;

    msg = CMD_RES( &electric_id );
    if( msg != (ReturnMsg)FlashOperationSuccess )
        return FALSE;
    
    /* Decide rems_id order. 0: { manufacturer id, device id }
                             1: { device id,  manufacturer id } */
    flash_state.ArrangeOpt = 0;

    msg = CMD_REMS( &rems_id, &flash_state );
    if( msg != (ReturnMsg)FlashOperationSuccess )
        return FALSE;
    

    msg = CMD_RDSCUR(&security_reg);
    if( msg != (ReturnMsg)FlashOperationSuccess )
        return FALSE;

    /* Read flash memory data to memory buffer */
    CMD_READ( flash_addr, unique_id_buff, trans_len ); 
    
    /* Compare to expected value */
    if( flash_id != FlashID )
        Error_inc( error_cnt );

    if( electric_id != ElectronicID )
        Error_inc( error_cnt );

    if( flash_state.ArrangeOpt )
    {
        if( rems_id != RESID1 )
           Error_inc( error_cnt );
    }else
    {
        if( rems_id != RESID0 )
           Error_inc( error_cnt );
    }

    if( error_cnt != 0 )
        return FALSE;
    else
        return TRUE;
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
uint8_t FlashReadWrite_Test(void)
{
    ////ReturnMsg  message= FlashOperationSuccess;
    ////FlashStatus  flash_state = {0};

    uint32_t  flash_addr;
    uint32_t  trans_len = 0;
    uint16_t  i=0, error_cnt = 0;
    uint16_t  seed = 0;
    ////uint8_t   st_reg = 0;
    uint8_t   memory_addr[TRANS_LENGTH] = {0};
    uint8_t   memory_addr_cmp[TRANS_LENGTH] = {0};

    /* Assign initial condition */
    flash_addr = FLASH_TARGET_ADDR;
    trans_len = TRANS_LENGTH;
    seed = RANDOM_SEED;

    /* Prepare data to transfer */
    srand( seed );
    for( i=0; i< trans_len; i=i+1 ){
        memory_addr[i] = rand()%256;   // generate random byte data
    }

    /* Erase 4K sector of flash memory
       Note: It needs to erase dirty sector before program */
    CMD_SE(flash_addr);

    /* Program data to flash memory */
    CMD_PP(flash_addr, memory_addr, trans_len );

    /* Read flash memory data to memory buffer */
    CMD_READ( flash_addr, memory_addr_cmp, trans_len );

    /* Compare original data and flash data */
    for( i=0; i < (trans_len); i=i+1 )
    {
        if( memory_addr[i] != memory_addr_cmp[i] )
            Error_inc( error_cnt );
    }

    /* Erase 4K sector of flash memory */
    CMD_SE( flash_addr );

    if( error_cnt != 0 )
        return FALSE;
    else
        return TRUE;

}


