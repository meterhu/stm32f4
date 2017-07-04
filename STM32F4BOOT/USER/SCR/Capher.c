#include <stdio.h>
#include <stdarg.h>
#include "stm32f4xx.h"
#include "stm32f4xx_fsmc.h"
#include "mystd.h"
#include "m_config.h"
#include "AppCRC.h"
#include "FramePack.h"
#include "CommHandle.h"
#include "FlashOpt.h"
#include "CPUID.h"
#include "UserLib.h"
#include "Capher.h"
#include "WRProtect.h"




//uint8_t  StartCipherKey[16] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F};
uint8_t  StartCipherKey[16] = {0x1B,0x41,0x62,0x73,0x84,0x95,0xA1,0xCF,0xE2,0x53,0x2A,0x39,0xCD,0xB7,0xA8,0x90};

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
int16_t  StartDesProc(COMM_FRAME *pcf)
{
    uint8_t    key_buff[20];
    uint8_t    buff[4];
    uint8_t    i;
    uint8_t    match_key_state;
    
    CPU_INFO  CPUInfo;
    
    for(i = 0; i < pcf->len - 1; i++)
    {
         key_buff[i] = pcf->param[i + 1];      
    }
    
    //匹配启动加密密码  
    match_key_state = MemoryCmp((uint8_t *)key_buff, (uint8_t *)StartCipherKey, KEY_LEN);
    if(match_key_state == TRUE)
    {
        //返回应答帧MCU唯一标识码
        ReadCPUID(&CPUInfo);
        SendFrame(UNIPUE_ID_CW, (uint8_t *)&CPUInfo, sizeof(CPU_INFO));   //2即2个字节,16bit
    }
    else
    {
        //0      应答命令字
        //1      应答结果
        //2-3    应答参数
        buff[0]  = STA_DES_CW;
        buff[1]  = KEY_CODE_MATCH_ERR;  //加密密码错误
        buff[2]  = 0x00;
        buff[3]  = 0x00;
        SendFrame(UNIVER_ACK_CW, (uint8_t *)buff, 4);   //2即2个字节,16bit        
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
int16_t  DesKeyProc(COMM_FRAME *pcf)
{
    uint8_t  app_bin_flag_buff[4];
    uint8_t  cipher_key[16];    
    uint8_t  random_buff[2048];
    uint16_t i;
    uint8_t  rt;
    uint8_t  buff[4];

    //app.bin 标识
    for (i = 0; i < 4; i++)
    {
         app_bin_flag_buff[i] = pcf->param[i + 1];
    }
    //128bit的加密密码
    for(i = 0; i < 16; i++)
    {
         cipher_key[i] = pcf->param[i + 5];      
    }
    
    //InvCipher(cipher_key, Key, out_text);

    //1008bytes的随机数
    for(i = 0; i < 2048; i++)
    {
         random_buff[i] = pcf->param[i + 21];      
    } 
    
    //把cipher_key , app_bin_flag 分布存在random_num 里，上电时取出来进行解密
    //第一个数据存储在256，每隔10 存一个数据
    //StoreKey(random_buff, cipher_key);
    StoreKey(random_buff, app_bin_flag_buff, APP_KEY_INDEX, sizeof (app_bin_flag_buff));
    StoreKey(random_buff, cipher_key, DES_KEY_INDEX, sizeof (cipher_key));
    
    FlashWRPUnlock(3);
    
    __disable_irq();
    rt = ProgramFlashErase(STORE_KEY_ADDR, STORE_KEY_BYTES);
    __enable_irq();
    
    if (!rt)
    {
        //发送扇区擦除错误
        buff[1] = ERASE_SECTOR_ERR;
        SendFrame(UNIVER_ACK_CW, (uint8_t *)buff, 4); //2即2个字节,16bit          
    }  
    
    Data2Flash(STORE_KEY_ADDR, random_buff, STORE_KEY_BYTES);
    
    FlashWRPLock(3);
    
    //0      应答命令字
    //1      应答结果
    //2-3    应答参数
    buff[0]  = DES_KEY_CW;
    buff[1]  = 0x00; 
    buff[2]  = 0x00;
    buff[3]  = 0x00;  

    SendFrame(UNIVER_ACK_CW, (uint8_t *)buff, 4);   //2即2个字节,16bit
    
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
void StoreKey(uint8_t *random_buff, uint8_t *key_out_buff, uint16_t store_index, uint8_t store_len)
{
    uint16_t   i,j;
     
    //第一个数据存储在256，每隔10 存一个数据
    for(i = 0, j = 0; i < store_len; i++)
    {
        random_buff[store_index + j]= key_out_buff[i];
        j += 10;
        
    }
    
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
void TakeOutKey(uint8_t *key_out_buff, uint16_t take_out_index, uint8_t take_out_len)
{
    uint16_t   i;
    uint8_t *pKeyAddr;
    pKeyAddr = (uint8_t *)STORE_KEY_ADDR + take_out_index;
     
    //第一个数据存储在256，每隔10 存一个数据
    for(i = 0; i < take_out_len; i++)
    {
        key_out_buff[i] = *pKeyAddr;
        pKeyAddr += 10;
        
    }
    
}

