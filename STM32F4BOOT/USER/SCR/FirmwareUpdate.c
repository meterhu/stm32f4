#include <stdio.h>
#include <stdarg.h>
#include "stm32f4xx.h"
#include "stm32f4xx_fsmc.h"
#include "mystd.h"
#include "m_config.h"
#include "SysAppConfig.h"
#include "AppCRC.h"
#include "FramePack.h"
#include "CommHandle.h"
#include "FlashOpt.h"
#include "CPUID.h"
#include "UserLib.h"
#include "FirmwareUpdate.h"
#include "FlashOpt.h"
#include "WRProtect.h"
#include "Capher.h"




uint32_t  DataPackedNums;
uint32_t  ProgramDataLen;
uint8_t   AppType[4] = {0};
uint32_t  Check_A;
uint32_t  Check_B;

uint32_t    CurFlashWAddr;

uint8_t   AppMatchBoot = APP_BIN_INVALID;

// uint8_t AppKey1[4]={0x16,0x69,0x92,0xF1};
// // extern uint8_t Key2[];
// // extern uint8_t Key3[];
// // extern uint8_t Key4[];
// uint8_t AppKey2[4]={0xE2,0x60,0xAB,0xC3};
// uint8_t AppKey3[4]={0xAC,0x8D,0xE7,0x38};
// uint8_t AppKey4[4]={0x90,0xAF,0xE2,0x34}

extern void InvCipher(unsigned char *intext, unsigned char * key, unsigned char * outtext);
//================================================================================================
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
int16_t  StartFireWareUpdate(COMM_FRAME *pcf)
{      

    volatile uint16_t  ctrl_w;
    volatile int16_t   rev;
    uint32_t cnt  = cnt;
    uint8_t  *p;
    uint16_t rt;
    
    uint8_t  buff[20];
    uint8_t  app_type_id[4] = {0};
    uint8_t  match_type = 0;

    MCU_INFO  McuInof;
    
    //最后一帧打开写保护，如果最后一帧不来，上电重启打开写保护，擦除之后关闭写保护直到写入数据完成  
    p = &(pcf)->param[1];
    ProgramDataLen  = MAKE_4BYTE_WORD(p[3], p[2], p[1], p[0]);
    DataPackedNums  = MAKE_HWORD(p[5], p[4]);
    //四个字节的应用程序型号
    AppType[0]      = p[8];
    AppType[1]      = p[9];
    AppType[2]      = p[10];
    AppType[3]      = p[11];
    Check_A         = p[12];
    Check_B         = p[13];
   
    buff[0] = START_FIRE_WARE_UPDATE_CW;
    
    //从Flash读出app.bin 类型 标识字
    TakeOutKey(app_type_id, APP_KEY_INDEX, sizeof (app_type_id));    
    //判断应用程序类型是否匹配
    match_type = MemoryCmp(AppType, app_type_id, sizeof (app_type_id));
    //如果型号不匹配
    if (!match_type)
    {
        buff[1] = APP_MATCH_BOOT_ERR;
        SendFrame(UNIVER_ACK_CW, (uint8_t *)buff, 4); //2即2个字节,16bit
        AppMatchBoot = APP_BIN_INVALID;
        return 0;
    }
    //判断数据长度       
    if(ProgramDataLen > FLASH_SIZE)
    {
        buff[1] = OVER_FLASH_SIZE;
        SendFrame(UNIVER_ACK_CW, (uint8_t *)buff, 4); //2即2个字节,16bit 
        AppMatchBoot = APP_BIN_INVALID;
        return 0;
    }

    CurFlashWAddr = APP_VALID_ADDR;
    //发送MCU信息帧
    //0-3     MCU ID
    //4-7     FLASH 容量
    //8-9     扇区数量
    //10-13   FLASH起始地址
    McuInof.id = MCU_ID;
    McuInof.flash_size = FLASH_SIZE;
    McuInof.sector_num = SECTOR_NUM;
    McuInof.flash_start_addr = APP_VALID_ADDR;
    //Delay(200000);
    SendFrame(MCU_INFO_CW, (uint8_t *)&McuInof, sizeof(MCU_INFO)); //2即2个字节,16bit  
    
    __disable_irq();
    rt = ProgramFlashErase(APP_VALID_ADDR, ProgramDataLen);
    __enable_irq();
    
    if (!rt)
    {
        //发送扇区擦除错误
        buff[1] = ERASE_SECTOR_ERR;
        SendFrame(UNIVER_ACK_CW, (uint8_t *)buff, 4); //2即2个字节,16bit
        AppMatchBoot = APP_BIN_INVALID;
    }
    else
    { 
        buff[1] = ERASE_SECTOR_SUCCESS;
        SendFrame(UNIVER_ACK_CW, (uint8_t *)buff, 4); //2即2个字节,16bit 
        AppMatchBoot = APP_BIN_VALID;
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
int16_t  FirmwDateUpdate(COMM_FRAME *pcf)
{
    volatile uint16_t    data_pack_cur_num;
    volatile uint16_t    data_len;   
    uint8_t    match_state;
    
    uint8_t    buff[20];
    uint8_t    i;

    uint8_t    data_buff[512] = {0};
    uint8_t    incipher_data_buff[2048];
          
    data_pack_cur_num = MAKE_HWORD((pcf)->param[2], (pcf)->param[1]);
    data_len          = pcf->len - 1;
    
    buff[0] = FIRMW_DATA_CW; 
    buff[2] = data_pack_cur_num; 
    buff[3] = data_pack_cur_num >> 8; 

    // AppMatchBoot 默认为有效值，如果检测到第一包的标志字不匹配标记为无效状态直接返回 
    if (AppMatchBoot != APP_BIN_VALID)
    {
        return 0;
    }

    //解密数据包，每128bit(16byte) 解密一次
    InvCipherAppData(&pcf->param[3], incipher_data_buff);        
    
    //BootMatchApp 正确，开始写入flash
    //把正确的数据写入到Flash
    if (!Data2Flash(CurFlashWAddr, incipher_data_buff, data_len - 2))
    {   
        buff[1] = WRITE_SECTOR_ERR;
    }     
    else
    {
        CurFlashWAddr +=  data_len - 2;
        buff[1] = 0; 
    }        
    
    SendFrame(UNIVER_ACK_CW, (uint8_t *)buff, 4); //2即2个字节,16bit
    printf("pack_cur_num= %d\n", data_pack_cur_num);
    
    if(!buff[1] && (data_pack_cur_num == DataPackedNums - 1))  //接收完成
    {

        if (!CheckProgramData(APP_VALID_ADDR, ProgramDataLen, Check_A, Check_B))
        {
            buff[1] = FIRMW_DATA_CRC_ERR;
            SendFrame(UNIVER_ACK_CW, (uint8_t *)buff, 4); //2即2个字节,16bit
        }
        else
        {
            //存储总文件长度和校验字和程序有效标志
            data_buff[0] = ProgramDataLen >> 0;
            data_buff[1] = ProgramDataLen >> 8;
            data_buff[2] = ProgramDataLen >> 16;
            data_buff[3] = ProgramDataLen >> 24;
            //应用程序类型
            data_buff[4] = AppType[0];
            data_buff[5] = AppType[1];
            data_buff[6] = AppType[2];
            data_buff[7] = AppType[3];
            
            data_buff[8] = Check_A;
            data_buff[9] = Check_B;
            
            data_buff[10] = 0xAA;
            data_buff[11] = 0x55;
            data_buff[12] = 0xA5;
            data_buff[13] = 0x5A;
            
            Data2Flash(APP1_START_ADDR, data_buff, APP_RESV_BYTES);
            
            buff[1] = FIRMW_DATA_WRITE_SUCCES;
            SendFrame(UNIVER_ACK_CW, (uint8_t *)buff, 4); //2即2个字节,16bit
            
            
            //SystemReset();
            
            
        }       
        
        
    }  //最后一包判断

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
uint8_t MatchAppBin(uint8_t *incipher_data)
{
    uint8_t    match_incipher = 0;
    uint8_t    match_type = 0;
    uint8_t    app_type_id[4] = {0};
    unsigned char AESHeader_ID[]={'A','B','C','P','R','O','G','R','A','M'};


    //从Flash读出app.bin 类型 标识字
    TakeOutKey(app_type_id, APP_KEY_INDEX, sizeof (app_type_id));

    //先判断加密标志10字节的头部    
    match_incipher = MemoryCmp(incipher_data, AESHeader_ID, sizeof (AESHeader_ID));

    //判断程序型号后4个字节 ,型号标志判断 
    match_type = MemoryCmp(&incipher_data[10], app_type_id, sizeof (app_type_id));   

    //存储Check_A，Check_B
    Check_A         = incipher_data[14];
    Check_B         = incipher_data[15];

    //返回判断结果
    if(match_incipher && match_type)
    {
         return TRUE;
    } 
    
    return FALSE ;
  
}

