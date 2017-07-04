//===============================================================================================
/************************************************************************************************
 *                                                                                                     
 *            文件名称:              
 *
 *                摘要: 
 *
 *            创建时间: 2013-07-24  
 *
 *            修改时间: 2013-07-24
 *            修改原因:
 *
 *            
 *
 *                作者: 杨卫华
 *
 *************************************************************************************************/
 //===============================================================================================
#ifndef _SYS_PARAM_RW_H
#define _SYS_PARAM_RW_H


////#define  EEPROM_SPI       0

extern uint16_t EE2NumByteToRead;

//==================================================================================
#define   SYS_PARAM_USE_SPI               1    //后备存储器使用SPI/I2C

//==================================================================================
#if  SYS_PARAM_USE_SPI == 1
    #include "MX25L.h"
#else
    #include "stm324xg_eval_i2c_ee.h"
#endif

//==================================================================================
#define   FLASH_TRANSDU_ADDR              10 * 4096  //每扇区4096 bytes,前10个扇区保留
#define   FLASH_OIL_PARAM_ADDR            11 * 4096   
#define   FLASH_PID_PARAM_ADDR            12 * 4096 

//==================================================================================
#define   E2_TRANSDU_ADDR                 128
#define   E2_PID_PARAM_ADDR               256
#define   E2_OIL_PARAM_ADDR               0




//========================================================================================
//
 
#if  SYS_PARAM_USE_SPI == 1 
   #define  INIT_PARAM_FLASH()    InitSimuSPI()
#else
   #define  INIT_PARAM_FLASH()    sEE_Init()
#endif  

#if  SYS_PARAM_USE_SPI == 1
    #define  PARAM_ADDR(addr_var_name)     FLASH_##addr_var_name
#else
    #define  PARAM_ADDR(addr_var_name)     E2_##addr_var_name
#endif

//==================================================================================
#if  SYS_PARAM_USE_SPI == 1  
    #define SYS_PARAM_READ(addr, param, size_t)  Flash_Read(PARAM_ADDR(addr), (u8 *)param, size_t);
#else
    #define SYS_PARAM_READ(addr, param, size_t)  do{\
                                                    EE2NumByteToRead = size_t; \
                                                    sEE_ReadBuffer((u8*)param, PARAM_ADDR(addr), &EE2NumByteToRead);\
                                                 }while(0)
#endif  


//==================================================================================
#if  SYS_PARAM_USE_SPI == 1  
    #define SYS_PARAM_WRITE(addr, param, size_t)  Flash_Write(PARAM_ADDR(addr), (u8 *)param, size_t);
#else
    #define SYS_PARAM_WRITE(addr, param, size_t)  sEE_WriteBuffer((u8*)param, PARAM_ADDR(addr), size_t);
#endif 

 
                                               
           
//==================================================================================


#endif //_SYS_PARAM_RW_H





