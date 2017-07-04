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
 *
 *                作者: 杨卫华
 *
 *******************************************************************************************************/
//======================================================================================================
#include "stm32f4xx.h"
#include "ucos_ii.h"
#include "mystd.h"  
#include "m_config.h"
#include "UserLib.h"
#include "SysAppConfig.h"
#include "Queue.h"
#include "Buff.h"
#include "UART.h"
#include "FramePack.h"
#include "CommHandle.h"
#include "main.h"
#include "tasks.h"
#include "CommTaskProc.h"
#include "RevAdvAngle.h"
#include "PWM.h"
#include "OS_Trace.h"
#include "SimuI2C.h"
#include "ADS123x.h"
#include "CPUID.h"


MCU_INFO  muc_info;
#define  CPU_ID_BASE_ADDR   0x1FFF7A10
#define  FLASH_START_ADDR   0x08008000
#define  FLASH_SIZE         0xD8000    // 16K + 16K + 64K + 128K * 6
#define  SECTOR_NUM         9

uint16_t  TotalFileLen;
uint16_t  DataPackAll;
int16_t   DataPackLen;
int8_t    CHECK_A;
int8_t    CHECK_B;   

static   uint16_t  EraseSectorSize;
const uint8_t    mcu_key_buff[16] = {0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01,0x01};    


extern     SAVE_PARAM_ONE  save_para_one;

extern TASK_INFO   TaskInfo[];
u16 Bit_Buff[12] = {BIT0,BIT1,BIT2,BIT3,BIT4,BIT5,BIT6,BIT7,BIT8,BIT9,BIT10,BIT11};	   //量缸号

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
int16_t  BootFirmwUpdProc(COMM_FRAME *pcf)
{      
    uint32_t   date_num;
    uint8_t    buff[4];
    //FLASH_Status  flash_state;
    
    TotalFileLen  = MAKE_HWORD((pcf)->param[2], (pcf)->param[1]);    
    DataPackAll   = GET_DATA0_HWORD(pcf);
    DataPackLen   = GET_DATA1_HWORD(pcf);
    CHECK_A       = pcf->param[7];
    CHECK_B       = pcf->param[8];
    
    date_num = DataPackAll * DataPackLen;
    //计算需要擦除的扇区
    EraseSectorSize = TotalFileLen / 1024;
    
    //0-3    MCU ID  
    //4-7    flash容量
    //8-9    扇区数量
    //10-13  Flash起始地址
    
    //muc_info.id               = *(volatile uint32_t *)(CPU_ID_BASE_ADDR);
    muc_info.id               = DBGMCU_IDCODE_DEV_ID | DBGMCU_IDCODE_REV_ID;
    muc_info.flash_size       = FLASH_SIZE;
    muc_info.sector_num       = SECTOR_NUM;
    muc_info.flash_start_addr = FLASH_START_ADDR;
    
    //0      应答命令字
    //1      应答结果
    //2-3    应答参数
    buff[0]  = BOOT_FIRM_UPD_CW;
    buff[1]  = 0x01;  //错误应答
    buff[2]  = 0x00;
    buff[3]  = 0x00;
#if 0    
    if(EraseSectorSize < 16)
    {
        flash_state = FLASH_EraseSector(FLASH_Sector_2,VoltageRange_3); //擦除
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2即2个字节,16bit   
        }
        
    }
    else if(EraseSectorSize < 32)
    {
        flash_state = FLASH_EraseSector(FLASH_Sector_2,VoltageRange_3); //擦除
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2即2个字节,16bit   
        }
        
        flash_state = FLASH_EraseSector(FLASH_Sector_3,VoltageRange_3); //擦除 
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2即2个字节,16bit   
        }        
    }
    else if(EraseSectorSize < 96)
    {
        flash_state = FLASH_EraseSector(FLASH_Sector_2,VoltageRange_3); //擦除
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2即2个字节,16bit   
        }
        
        flash_state = FLASH_EraseSector(FLASH_Sector_3,VoltageRange_3); //擦除
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2即2个字节,16bit   
        }
        
        flash_state = FLASH_EraseSector(FLASH_Sector_4,VoltageRange_3); //擦除   
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2即2个字节,16bit   
        }        
    }
    else if(EraseSectorSize < 224)
    {
        flash_state = FLASH_EraseSector(FLASH_Sector_2,VoltageRange_3); //擦除
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2即2个字节,16bit   
        }
        
        flash_state = FLASH_EraseSector(FLASH_Sector_3,VoltageRange_3); //擦除
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2即2个字节,16bit   
        }
        
        flash_state = FLASH_EraseSector(FLASH_Sector_4,VoltageRange_3); //擦除 
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2即2个字节,16bit   
        }
        
        flash_state = FLASH_EraseSector(FLASH_Sector_5,VoltageRange_3); //擦除 
        if(flash_state != FLASH_COMPLETE)
        {
            SendFrame(EVENT_REPOT_CW, (uint8_t *)&ERASE_SECTOR_ERR, 4);   //2即2个字节,16bit   
        }        
    }

#endif
    
    if(date_num > muc_info.flash_size)
    {
        SendFrame(UNIVER_ACK_CW, (uint8_t *)buff, 4);   //2即2个字节,16bit    
    }
    else
    {
        SendFrame(MCU_INFO_CW, (uint8_t *)&muc_info, 14);   //2即2个字节,16bit   
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
int16_t  FirmwDateProc(COMM_FRAME *pcf)
{    
    uint8_t    buff[4];
    uint8_t    event_report[1];
    uint8_t    i;
    volatile uint16_t    data_pack_cur_num;
    volatile uint16_t    data_len;
    
    //FLASH_Status  flash_state;
    
    static uint8_t check_a;
    static uint8_t check_b;
    
    static uint32_t *paddr;   //定义一个指向开始地址的指针
    paddr = (uint32_t *)FLASH_START_ADDR;
    
    data_pack_cur_num = MAKE_HWORD((pcf)->param[2], (pcf)->param[1]);
    data_len          = GET_DATA0_HWORD(pcf);;
    
    //0      应答命令字
    //1      应答结果
    //2-3    应答参数
    buff[0]  = FIRMW_DATA_CW;
    buff[1]  = 0x00;  //错误应答
    buff[2]  = data_pack_cur_num & 0xFF;
    buff[3]  = (data_pack_cur_num >> 8) & 0xFF;

    FLASH_Unlock();	//解锁
    FLASH_ClearFlag(FLASH_FLAG_EOP|FLASH_FLAG_OPERR|FLASH_FLAG_WRPERR| FLASH_FLAG_PGAERR | FLASH_FLAG_PGPERR|FLASH_FLAG_PGSERR);
    
    for(i = 0; i < data_len - 2; i++)
    {
        check_a = check_a + pcf->param[i + 3];
        check_b = check_b + check_a;

        #if 0
        flash_state = FLASH_ProgramWord(*paddr++, (pcf)->param[i + 3]);//写入数据
                                                                              
        if(flash_state != FLASH_COMPLETE)
        {
            buff[1]  = 0x01;
            event_report[0] = WRITE_SECTOR_ERR;
            SendFrame(EVENT_REPOT_CW, (uint8_t *)event_report, 4); //2即2个字节,16bit
            //SendFrame(EVENT_REPOT_CW, (uint8_t *)&WRITE_SECTOR_ERR, 4);   //2即2个字节,16bit    
        }
        #endif
        
    }

    FLASH_Lock() ;  //上锁

    if(data_pack_cur_num == DataPackAll - 1)  //接收完成
    {
        if((check_a == CHECK_A) && (check_b == CHECK_B))   //计算出的校验码与PC端给定的校验码相符
        {
            event_report[0] = FIRMW_DATA_CRC_MATCH;
            SendFrame(EVENT_REPOT_CW, (uint8_t *)event_report, 4);   //2即2个字节,16bit
            //SendFrame(EVENT_REPOT_CW, (uint8_t *)&FIRMW_DATA_CRC_MATCH, 4);   //2即2个字节,16bit 
            //程序复位
            
        }
        else
        {
            event_report[0] = FIRMW_DATA_CRC_ERR;
            SendFrame(EVENT_REPOT_CW, (uint8_t *)event_report, 4);   //2即2个字节,16bit
            //SendFrame(EVENT_REPOT_CW, (uint8_t *)&FIRMW_DATA_CRC_ERR, 4);   //2即2个字节,16bit
            //将已经写入的固件做无效处理，防止错误的固件运行
            
        }

    }
    
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
int16_t  StartDesProc(COMM_FRAME *pcf)
{
    uint8_t    key_buff[20];
    uint8_t    i;
    uint8_t    match_key_state;
    
    CPU_INFO  CPUInfo;
    
    for(i = 0; i < pcf->len - 1; i++)
    {
         key_buff[i] = pcf->param[i + 1];      
    }
    
    //匹配密码
    match_key_state = MemoryCmp((uint8_t *)key_buff, (uint8_t *)mcu_key_buff, 16);
    if(match_key_state == TRUE)
    {
        //返回应答帧MCU唯一标识码
        ReadCPUID(&CPUInfo);
        SendFrame(UNIPUE_ID_CW, (uint8_t *)&CPUInfo, 16);   //2即2个字节,16bit
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
    uint8_t  encrypt_key[16];
    uint8_t  random_num[1008];
    uint16_t i;
    uint8_t  buff[4];
    
    //128bit的加密密码
    for(i = 0; i < 16; i++)
    {
         encrypt_key[i] = pcf->param[i + 1];      
    }
    
    //1008bytes的随机数
    for(i = 0; i < 1008; i++)
    {
         random_num[i] = pcf->param[i + 17];      
    }
    
    //MCU 加密操作
    //.....
    
    
    
    //0      应答命令字
    //1      应答结果
    //2-3    应答参数
    buff[0]  = DES_KEY_CW;
    buff[0]  = 0x00;  //正确应答
    buff[2]  = 0x00;
    buff[3]  = 0x00;
    SendFrame(UNIVER_ACK_CW, (uint8_t *)buff, 4);   //2即2个字节,16bit
    
    return 1;    
}




