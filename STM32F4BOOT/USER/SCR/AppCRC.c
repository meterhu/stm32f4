#include <stdio.h>
#include <stdarg.h>
#include "stm32f4xx.h"
#include "stm32f4xx_fsmc.h"
#include "mystd.h"
#include "m_config.h"
#include "AppCRC.h"
#include "CommHandle.h"
#include "FlashOpt.h"
#include "CPUID.h"
#include "UserLib.h"
#include "Capher.h"
#include "MX25L.h"



extern uint32_t  DataPackedNums;
extern uint32_t  ProgramDataLen;
extern uint32_t  Check_A;
extern uint32_t  Check_B; 
extern uint8_t   Key[16];

extern void InvCipher(unsigned char *intext, unsigned char * key, unsigned char * outtext);

uint8_t CheckCipherValidID(void);
uint8_t Key1[4]={0x16,0x69,0x92,0xF1};
extern uint8_t Key2[];
extern uint8_t Key3[];
extern uint8_t Key4[];
//uint8_t Key2[4]={0xE2,0x60,0xAB,0xC3};
//uint8_t Key3[4]={0xAC,0x8D,0xE7,0x38};
//uint8_t Key4[4]={0x90,0xAF,0xE2,0x34};

uint8_t AppKey1[4]={0x16,0x69,0x92,0xF1};
// extern uint8_t Key2[];
// extern uint8_t Key3[];
// extern uint8_t Key4[];
uint8_t AppKey2[4]={0xE2,0x60,0xAB,0xC3};
uint8_t AppKey3[4]={0xAC,0x8D,0xE7,0x38};
uint8_t AppKey4[4]={0x90,0xAF,0xE2,0x34};

void  GoToAddr(uint32_t  addr);
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
uint8_t AppCRC(void)
{
    uint8_t  app_type_id[4] = {0};  
    uint8_t *paddr;   
    paddr = (uint8_t *)APP1_START_ADDR;
    
    //从Flash读出app.bin 类型 标识字
    TakeOutKey(app_type_id, APP_KEY_INDEX, sizeof (app_type_id));    

    if(*(paddr + 10) == APP_VALID_FLAG_BYTE1)
    if(*(paddr + 11) == APP_VALID_FLAG_BYTE2) 
    if(*(paddr + 12) == APP_VALID_FLAG_BYTE3)
    if(*(paddr + 13) == APP_VALID_FLAG_BYTE4)
    {     
        printf("line=%d\n", __LINE__);
        if (CheckProgramData(APP_VALID_ADDR, *(uint32_t *)paddr, *(paddr + 8), *(paddr + 9)))  
        {
            //判断应用程序类型是否匹配
            if (*(paddr + 4) == app_type_id[0])
            if (*(paddr + 5) == app_type_id[1])
            if (*(paddr + 6) == app_type_id[2])
            if (*(paddr + 7) == app_type_id[3])
            {           
                printf("line=%d\n", __LINE__);
                //解密通过置为有效ID状态
                if (CheckCipherValidID())
                {
                   //RunApp(); 
                   //GOTO(0x0800C39D);
                   //GoToAddr(APP1_START_ADDR + APP_RESV_BYTES);
                   printf("-------------start to run App------------\n");
                   printf("-----------------------------------------\n");
                   printf("-----------------------------------------\n");
                   GoToAddr(APP_VALID_ADDR);
                }
                return 1;
            }
             
        } 

    }
    else
    {
      return 0;   
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
uint8_t CheckSPIFlashID(void)
{
    uint32_t spi_flash_id;
    
    InitSimuSPI();
    ReadFlashID(&spi_flash_id);   
    printf("\nspi flash id = 0x%x\n", spi_flash_id);
    if (spi_flash_id == FlashID)
    {
        printf("spi flash is MX25L6406E\n");   
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
uint8_t CheckCipherValidID(void)
{
    uint8_t ciphe_text[16] = {0};
    uint8_t out_text[16] = {0};
    uint8_t rt;    
    CPU_INFO  CPUInfo;
    volatile uint8_t  key[16];
    
    //App 加密密码
    key[0] = Key1[0];
    key[1] = Key1[1];
    key[2] = Key1[2];
    key[3] = Key1[3];
    
    key[4] = Key2[0];
    key[5] = Key2[1];
    key[6] = Key2[2];
    key[7] = Key2[3];
    
    key[8]  = Key3[0];
    key[9]  = Key3[1];
    key[10] = Key3[2];
    key[11] = Key3[3];
    
    key[12] = Key4[0];
    key[13] = Key4[1];
    key[14] = Key4[2];
    key[15] = Key4[3];
    

    //读出FLASH_START_ADDR   0x08008000处的数据,进行解密操作
    //密码存放在第256个字节开始，每隔10 存一个数据
    //TakeOutKey(ciphe_text);
    TakeOutKey(ciphe_text, DES_KEY_INDEX, sizeof(ciphe_text)) ;
    InvCipher(ciphe_text, (uint8_t *)key, out_text);
    
    ReadCPUID(&CPUInfo);
    
    rt = MemoryCmp(out_text, (uint8_t *)&CPUInfo, sizeof(out_text));

    return rt;

    
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
void InvCipherAppData(uint8_t *cipher_data, uint8_t *incipher_data)
{
    uint16_t i;
    volatile uint8_t  key[16];
    
    
    key[0] = AppKey1[0];
    key[1] = AppKey1[1];
    key[2] = AppKey1[2];
    key[3] = AppKey1[3];
    
    key[4] = AppKey2[0];
    key[5] = AppKey2[1];
    key[6] = AppKey2[2];
    key[7] = AppKey2[3];
    
    key[8]  = AppKey3[0];
    key[9]  = AppKey3[1];
    key[10] = AppKey3[2];
    key[11] = AppKey3[3];
    
    key[12] = AppKey4[0];
    key[13] = AppKey4[1];
    key[14] = AppKey4[2];
    key[15] = AppKey4[3];
    
    //解密数据 ,每128bit 解密一次 (16byte)  ,解密128次
    for (i = 0; i < 2048 / 16; i++)
    {
        InvCipher(cipher_data, (uint8_t *)key, incipher_data);
        cipher_data += 16;
        incipher_data += 16;
    }
    
}

typedef  void (*pFunction)(void);
pFunction Jump_To_Application;
uint32_t JumpAddress;
uint32_t FlashProtection = 0;

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
void  GoToAddr(uint32_t  addr)
{
  
  /* Jump to user application */
  JumpAddress = *(__IO uint32_t*) (addr + 4);
  Jump_To_Application = (pFunction) JumpAddress;
  /* Initialize user application's Stack Pointer */
  __set_MSP(*(__IO uint32_t*) addr);
  Jump_To_Application();
  
    
    
    
    
}





