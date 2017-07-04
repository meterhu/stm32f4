#include <stdio.h>
#include <stdarg.h>
#include "stm32f4xx.h"
#include "stm32f4xx_fsmc.h"
#include "mystd.h"
#include "m_config.h"
#include "CPUID.h"
#include "UserLib.h"
#include "CheckCapherIDValid.h"



//uint8_t Key[]="huang_yang_li:13";

uint8_t Key1[4]={0x16,0x69,0x92,0xF1};
extern uint8_t Key2[];
extern uint8_t Key3[];
extern uint8_t Key4[];
//uint8_t Key2[4]={0xE2,0x60,0xAB,0xC3};
//uint8_t Key3[4]={0xAC,0x8D,0xE7,0x38};
//uint8_t Key4[4]={0x90,0xAF,0xE2,0x34};

extern void InvCipher(unsigned char *intext, unsigned char * key, unsigned char * outtext);
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
#if 0
uint8_t CheckCipherValidID1(void)
{
    uint8_t ciphe_text[16] = {0};
    uint8_t out_text[16] = {0};
    uint8_t rt;    
    CPU_INFO  CPUInfo;
    
    //读出FLASH_START_ADDR   0x08008000处的数据,进行解密操作
    //密码存放在第256个字节开始，每隔10 存一个数据
    TakeOutKey(ciphe_text);
    InvCipher(ciphe_text, Key, out_text);
    
    ReadCPUID(&CPUInfo);
    
    rt = MemoryCmp(out_text, (uint8_t *)&CPUInfo, sizeof(out_text));

    return rt;

    
}
#endif

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
    TakeOutKey(ciphe_text);
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
void TakeOutKey(uint8_t *key_out_buff)
{
    uint16_t   i;
    uint8_t *pKeyAddr;
    pKeyAddr = (uint8_t *)STORE_KEY_ADDR + 256;
     
    //第一个数据存储在256，每隔10 存一个数据
    for(i = 0; i < 16; i++)
    {
        key_out_buff[i] = *pKeyAddr;
        pKeyAddr += 10;
        
    }
    
}



