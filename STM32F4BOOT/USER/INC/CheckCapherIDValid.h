#ifndef   __CHECK_CAPHER_ID_VALID_H
#define   __CHECK_CAPHER_ID_VALID_H


//=================================================================================================
//*************************************************************************************************
//宏定义
#define  MCU_ID              0
#define  FLASH_SIZE          0xD8000    // 16K + 16K + 64K + 128K * 6
#define  SECTOR_NUM          9

#define  STORE_KEY_ADDR     0x08008000 
#define  STORE_KEY_BYTES     2048

#define  APP1_START_ADDR    0x0800C000  
#define  APP_RESV_BYTES     512


#define  APP_VALID_ADDR     APP1_START_ADDR + APP_RESV_BYTES

#define APP_VALID_FLAG_BYTE1  0xAA
#define APP_VALID_FLAG_BYTE2  0x55
#define APP_VALID_FLAG_BYTE3  0xA5
#define APP_VALID_FLAG_BYTE4  0x5A

#define KEY_LEN        16

#define CAPHER_ID_INVALID    0xA3
#define CAPHER_ID_VALID      0x5A

//=================================================================================================
//*************************************************************************************************
//数据类型定义



//=================================================================================================
//*************************************************************************************************
//变量定义

//========================================================================================
//


//=================================================================================================
//*************************************************************************************************
//外部函数声明
extern uint8_t CheckCipherValidID(void);
extern void TakeOutKey(uint8_t *key_out_buff);

#endif   //__CHECK_CAPHER_ID_VALID_HS



